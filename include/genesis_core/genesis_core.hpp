#pragma once
#include <cstdint>

namespace raig {

struct InitialConditions final {
    std::uint64_t seed = 0;
};

struct WorldStimulus final {
    double a = 0.0;
    double b = 0.0;
};

enum class DeathReason : std::uint8_t {
    None        = 0,
    NonFinite   = 1,
    PhiOverflow = 2,
    KappaZero   = 3
};

struct DiagnosticSnapshot final {
    std::uint64_t step = 0;
    double kappa = 0.0;
    double phi   = 0.0;
    double dmag  = 0.0;
    double mmag  = 0.0;

    bool alive = true;
    DeathReason death_reason = DeathReason::None;
};

struct StructuralState;

class GenesisCore final {
public:
    explicit GenesisCore(const InitialConditions& init);
    ~GenesisCore();
    GenesisCore(const GenesisCore&) = delete;
    GenesisCore& operator=(const GenesisCore&) = delete;

    // returns true if step executed; false if already dead
    bool step(const WorldStimulus& stimulus);

    bool alive() const noexcept { return alive_; }
    DiagnosticSnapshot diagnostics() const;

private:
    StructuralState* X_ = nullptr;
    std::uint64_t step_ = 0;

    bool alive_ = true;
    DeathReason death_reason_ = DeathReason::None;
};

} // namespace raig

