#pragma once
#include <cstdint>
#include "genesis_core/genesis_core.hpp"

namespace raig {

struct WorldConfig final {
    std::uint64_t seed = 1;
};

class WorldInterface final {
public:
    explicit WorldInterface(const WorldConfig& cfg);
    WorldStimulus next();

private:
    std::uint64_t state_ = 0;
};

} // namespace raig
