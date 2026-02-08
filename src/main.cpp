#include <cstdint>
#include <iostream>

#include "genesis_core/genesis_core.hpp"
#include "runtime/runtime_supervisor.hpp"
#include "telemetry/telemetry_emitter.hpp"
#include "world/world_interface.hpp"

int main() {
    raig::InitialConditions init{ .seed = 0 };
    raig::WorldConfig wcfg{ .seed = 1 };
    raig::TelemetryConfig tcfg{ .out_path = "telemetry_long.jsonl" };
    raig::RuntimeConfig rcfg{ .max_steps = 1000000, .pace_ms = 0 };

    std::cout << "max_steps=" << rcfg.max_steps << std::endl;

    raig::GenesisCore core(init);
    raig::WorldInterface world(wcfg);
    raig::TelemetryEmitter telemetry(tcfg);
    raig::RuntimeSupervisor runtime(rcfg);

    std::uint64_t step = 0;
while (runtime.should_continue(step) && core.alive()) {
    const auto stimulus = world.next();
    core.step(stimulus);

    const auto snap = core.diagnostics();
    telemetry.emit(snap);

    ++step;
    runtime.pace();
}

    return 0;
}