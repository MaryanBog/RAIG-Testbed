#include "genesis_core/genesis_core.hpp"
#include "genesis_core/structural_state_internal.hpp"

#include <cmath>
#include <limits>

namespace raig {

static inline bool is_finite(double x) noexcept { return std::isfinite(x); }
static inline double clamp01(double x) noexcept {
    x = std::fmax(0.0, x);
    x = std::fmin(1.0, x);
    return x;
}

// fixed params
static constexpr double kAlphaStim   = 0.0020;
static constexpr double kBetaDamp    = 0.0008;
static constexpr double kGammaPhi    = 0.0400;
static constexpr double kEtaStimPhi  = 0.0100;
static constexpr double kMuMemD      = 0.0006;
static constexpr double kNuMemPhi    = 0.0002;
static constexpr double kCkPhi       = 0.00015;
static constexpr double kCkMem       = 0.00001;

static constexpr double kPhiMax      = 1e6;
static constexpr double kKappaEps    = 1e-12;

DeathReason evolve(StructuralState& X, const WorldStimulus& s) {
    // 0) Non-finite input => death
    if (!is_finite(s.a) || !is_finite(s.b)) {
        X.kappa = 0.0;
        return DeathReason::NonFinite;
    }

    const double sa = s.a;
    const double sb = s.b;

    const double stim_dir = (sa - sb);
    const double stim_pow = (sa * sa + sb * sb);

    // 2) Δ
    const double delta_new = X.delta + (kAlphaStim * stim_dir) - (kBetaDamp * X.kappa * X.delta);

    // 3) Φ
    const double target_phi = (delta_new * delta_new) + (kEtaStimPhi * stim_pow);
    const double phi_new = (1.0 - kGammaPhi) * X.phi + kGammaPhi * target_phi;

    // 4) M monotonic
    const double d_abs   = std::fabs(delta_new);
    const double phi_abs = std::fabs(phi_new);
    const double mem_inc = (kMuMemD * d_abs) + (kNuMemPhi * phi_abs);
    const double m_new   = X.m + mem_inc;

    // 5) κ bounded
    double kappa_new = X.kappa - (kCkPhi * phi_new) - (kCkMem * mem_inc);
    kappa_new = clamp01(kappa_new);

    // 6) Domain checks => death reasons (NO abort)
    if (!is_finite(delta_new) || !is_finite(phi_new) || !is_finite(m_new) || !is_finite(kappa_new)) {
        X.kappa = 0.0;
        return DeathReason::NonFinite;
    }
    if (phi_new > kPhiMax) {
        X.kappa = 0.0;
        return DeathReason::PhiOverflow;
    }
    if (kappa_new <= kKappaEps) {
        X.kappa = 0.0;
        return DeathReason::KappaZero;
    }

    // commit
    X.delta = delta_new;
    X.phi   = phi_new;
    X.m     = m_new;
    X.kappa = kappa_new;

    return DeathReason::None;
}

} // namespace raig
