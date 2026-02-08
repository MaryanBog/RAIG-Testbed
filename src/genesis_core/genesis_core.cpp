#include "genesis_core/genesis_core.hpp"
#include "genesis_core/structural_state_internal.hpp"

#include <stdexcept>

namespace raig {

// evolution returns DeathReason::None if ok, otherwise collapse reason
DeathReason evolve(StructuralState& X, const WorldStimulus& s);

GenesisCore::GenesisCore(const InitialConditions&)
: X_(new StructuralState{}), step_(0), alive_(true), death_reason_(DeathReason::None) {}

GenesisCore::~GenesisCore() {
    delete X_;
    X_ = nullptr;
}

bool GenesisCore::step(const WorldStimulus& stimulus) {
    if (!X_) throw std::runtime_error("GenesisCore: null state");
    if (!alive_) return false;

    const DeathReason r = evolve(*X_, stimulus);
    ++step_;

    if (r != DeathReason::None) {
        alive_ = false;
        death_reason_ = r;
    }
    return true;
}

DiagnosticSnapshot GenesisCore::diagnostics() const {
    if (!X_) throw std::runtime_error("GenesisCore: null state");

    DiagnosticSnapshot snap{};
    snap.step  = step_;
    snap.kappa = X_->kappa;
    snap.phi   = X_->phi;
    snap.dmag  = (X_->delta >= 0.0) ? X_->delta : -X_->delta;
    snap.mmag  = X_->m;

    snap.alive = alive_;
    snap.death_reason = death_reason_;
    return snap;
}

} // namespace raig

