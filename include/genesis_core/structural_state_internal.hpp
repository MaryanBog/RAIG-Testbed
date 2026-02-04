#pragma once

namespace raig {

// INTERNAL ONLY: do not include from outside genesis_core module.
struct StructuralState final {
    double delta = 0.0;
    double phi   = 0.0;
    double m     = 0.0;
    double kappa = 1.0;
};

} // namespace raig
