// tests/raig_tests.cpp
#include <cstdint>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "genesis_core/genesis_core.hpp"
#include "world/world_interface.hpp"
#include "runtime/runtime_supervisor.hpp"
#include "telemetry/telemetry_emitter.hpp"

static int g_failures = 0;

static void fail(const char* expr, const char* file, int line) {
    std::cerr << "FAIL: " << expr << " @ " << file << ":" << line << "\n";
    ++g_failures;
}

#define REQUIRE(expr) do { if (!(expr)) fail(#expr, __FILE__, __LINE__); } while(0)

static inline bool finite(double x) { return std::isfinite(x); }

static std::string read_file_binary(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return {};
    std::string data;
    in.seekg(0, std::ios::end);
    const auto n = in.tellg();
    if (n <= 0) return {};
    data.resize(static_cast<std::size_t>(n));
    in.seekg(0, std::ios::beg);
    in.read(data.data(), static_cast<std::streamsize>(data.size()));
    return data;
}

static void remove_if_exists(const std::string& path) {
    std::error_code ec;
    std::filesystem::remove(path, ec);
}

static bool contains(const std::string& s, const char* needle) {
    return s.find(needle) != std::string::npos;
}

struct RunResult {
    raig::DiagnosticSnapshot final_snap{};
    std::uint64_t loop_steps = 0; // runtime loop counter
};

static RunResult run_core_only(std::uint64_t init_seed,
                              std::uint64_t world_seed,
                              std::uint64_t max_steps,
                              int pace_ms)
{
    raig::InitialConditions init{ .seed = init_seed };
    raig::WorldConfig wcfg{ .seed = world_seed };
    raig::RuntimeConfig rcfg{ .max_steps = max_steps, .pace_ms = pace_ms };

    raig::GenesisCore core(init);
    raig::WorldInterface world(wcfg);
    raig::RuntimeSupervisor runtime(rcfg);

    std::uint64_t step = 0;
    double prev_m = 0.0;
    bool seen_death = false;

    while (runtime.should_continue(step) && core.alive()) {
        const auto stim = world.next();
        (void)core.step(stim);

        const auto snap = core.diagnostics();

        REQUIRE(finite(snap.kappa));
        REQUIRE(finite(snap.phi));
        REQUIRE(finite(snap.dmag));
        REQUIRE(finite(snap.mmag));

        REQUIRE(snap.kappa >= 0.0);
        REQUIRE(snap.kappa <= 1.0);

        REQUIRE(snap.mmag >= prev_m);
        prev_m = snap.mmag;

        if (!snap.alive) {
            seen_death = true;
            REQUIRE(snap.death_reason != raig::DeathReason::None);
            REQUIRE(snap.kappa == 0.0);
            break;
        }

        REQUIRE(snap.death_reason == raig::DeathReason::None);

        ++step;
        runtime.pace();
    }

    const auto final_snap = core.diagnostics();
    REQUIRE(seen_death || (final_snap.step == max_steps));

    return RunResult{ final_snap, step };
}

static RunResult run_with_telemetry(std::uint64_t init_seed,
                                   std::uint64_t world_seed,
                                   std::uint64_t max_steps,
                                   int pace_ms,
                                   const std::string& out_path)
{
    remove_if_exists(out_path);

    raig::InitialConditions init{ .seed = init_seed };
    raig::WorldConfig wcfg{ .seed = world_seed };
    raig::TelemetryConfig tcfg{ .out_path = out_path };
    raig::RuntimeConfig rcfg{ .max_steps = max_steps, .pace_ms = pace_ms };

    raig::GenesisCore core(init);
    raig::WorldInterface world(wcfg);
    raig::TelemetryEmitter telemetry(tcfg);
    raig::RuntimeSupervisor runtime(rcfg);

    std::uint64_t step = 0;

    while (runtime.should_continue(step) && core.alive()) {
        const auto stim = world.next();
        (void)core.step(stim);

        const auto snap = core.diagnostics();
        telemetry.emit(snap);

        ++step;
        runtime.pace();
    }

    // Emit final snapshot if we died between loop condition checks (safety)
    const auto final_snap = core.diagnostics();
    if (!final_snap.alive) {
        telemetry.emit(final_snap);
    }

    return RunResult{ final_snap, step };
}

// --------------------- tests ---------------------

static void test_invariants_until_death() {
    const auto r = run_core_only(/*init_seed=*/0, /*world_seed=*/1, /*max_steps=*/2000000, /*pace_ms=*/0);
    if (!r.final_snap.alive) {
        REQUIRE(r.final_snap.death_reason == raig::DeathReason::KappaZero);
    }
}

static void test_determinism_final_snapshot() {
    const auto a = run_core_only(/*init_seed=*/0, /*world_seed=*/1, /*max_steps=*/2000000, /*pace_ms=*/0).final_snap;
    const auto b = run_core_only(/*init_seed=*/0, /*world_seed=*/1, /*max_steps=*/2000000, /*pace_ms=*/0).final_snap;

    REQUIRE(a.step == b.step);
    REQUIRE(a.alive == b.alive);
    REQUIRE(a.death_reason == b.death_reason);

    REQUIRE(a.kappa == b.kappa);
    REQUIRE(a.phi   == b.phi);
    REQUIRE(a.dmag  == b.dmag);
    REQUIRE(a.mmag  == b.mmag);
}

static void test_world_replay() {
    raig::WorldConfig w1{ .seed = 123 };
    raig::WorldConfig w2{ .seed = 123 };
    raig::WorldConfig w3{ .seed = 124 };

    raig::WorldInterface A(w1);
    raig::WorldInterface B(w2);
    raig::WorldInterface C(w3);

    bool diverged = false;

    for (int i = 0; i < 1000; ++i) {
        const auto sa = A.next();
        const auto sb = B.next();
        const auto sc = C.next();

        REQUIRE(sa.a == sb.a);
        REQUIRE(sa.b == sb.b);

        if (sa.a != sc.a || sa.b != sc.b) diverged = true;
    }

    REQUIRE(diverged);
}

static void test_telemetry_replay_byte_for_byte() {
    const std::uint64_t max_steps = 5000;

    const std::string f1 = "test_telemetry_replay_a.jsonl";
    const std::string f2 = "test_telemetry_replay_b.jsonl";

    (void)run_with_telemetry(/*init_seed=*/0, /*world_seed=*/1, max_steps, /*pace_ms=*/0, f1);
    (void)run_with_telemetry(/*init_seed=*/0, /*world_seed=*/1, max_steps, /*pace_ms=*/0, f2);

    const auto a = read_file_binary(f1);
    const auto b = read_file_binary(f2);

    REQUIRE(!a.empty());
    REQUIRE(a == b);

    remove_if_exists(f1);
    remove_if_exists(f2);
}

static void test_pace_invariance() {
    const std::uint64_t max_steps = 300;

    const std::string f0 = "test_pace_0ms.jsonl";
    const std::string f1 = "test_pace_1ms.jsonl";

    (void)run_with_telemetry(/*init_seed=*/0, /*world_seed=*/1, max_steps, /*pace_ms=*/0, f0);
    (void)run_with_telemetry(/*init_seed=*/0, /*world_seed=*/1, max_steps, /*pace_ms=*/1, f1);

    const auto a = read_file_binary(f0);
    const auto b = read_file_binary(f1);

    REQUIRE(!a.empty());
    REQUIRE(a == b);

    remove_if_exists(f0);
    remove_if_exists(f1);
}

// --- NEW TESTS ---

static void test_step_monotonic_exact() {
    raig::InitialConditions init{ .seed = 0 };
    raig::WorldConfig wcfg{ .seed = 1 };

    raig::GenesisCore core(init);
    raig::WorldInterface world(wcfg);

    const auto s0 = core.diagnostics();
    std::uint64_t expected = s0.step;

    for (int i = 0; i < 5000 && core.alive(); ++i) {
        const auto stim = world.next();
        const bool ok = core.step(stim);
        REQUIRE(ok == true);

        const auto snap = core.diagnostics();
        expected += 1;
        REQUIRE(snap.step == expected);
    }
}

static void test_death_is_sticky() {
    raig::InitialConditions init{ .seed = 0 };
    raig::WorldConfig wcfg{ .seed = 1 };

    raig::GenesisCore core(init);
    raig::WorldInterface world(wcfg);

    // drive to death (or a large cap)
    for (std::uint64_t i = 0; i < 2000000 && core.alive(); ++i) {
        (void)core.step(world.next());
    }

    const auto dead = core.diagnostics();
    if (!dead.alive) {
        REQUIRE(dead.death_reason != raig::DeathReason::None);

        const auto before = core.diagnostics();
        const bool ok = core.step(world.next());
        REQUIRE(ok == false); // must refuse to step

        const auto after = core.diagnostics();
        REQUIRE(after.alive == false);
        REQUIRE(after.death_reason == before.death_reason);
        REQUIRE(after.step == before.step);
        REQUIRE(after.kappa == before.kappa);
        REQUIRE(after.phi   == before.phi);
        REQUIRE(after.dmag  == before.dmag);
        REQUIRE(after.mmag  == before.mmag);
    }
}

static void test_telemetry_schema_sanity() {
    const std::uint64_t max_steps = 2000;
    const std::string f = "test_schema.jsonl";

    const auto r = run_with_telemetry(/*init_seed=*/0, /*world_seed=*/1, max_steps, /*pace_ms=*/0, f);

    const auto data = read_file_binary(f);
    REQUIRE(!data.empty());

    // Must end with newline (JSONL)
    REQUIRE(!data.empty() && data.back() == '\n');

    // Must contain required keys somewhere
    REQUIRE(contains(data, "\"step\""));
    REQUIRE(contains(data, "\"alive\""));
    REQUIRE(contains(data, "\"death_reason\""));

    // If died, last line should include alive:false
    if (!r.final_snap.alive) {
        // crude: check last ~256 bytes
        const std::size_t tail = data.size() > 256 ? 256 : data.size();
        const std::string last = data.substr(data.size() - tail);
        REQUIRE(contains(last, "\"alive\":false"));
    }

    remove_if_exists(f);
}

int main() {
    test_invariants_until_death();
    test_determinism_final_snapshot();
    test_world_replay();
    test_telemetry_replay_byte_for_byte();
    test_pace_invariance();

    test_step_monotonic_exact();
    test_death_is_sticky();
    test_telemetry_schema_sanity();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    }
    std::cout << "TESTS FAILED: " << g_failures << "\n";
    return 1;
}
