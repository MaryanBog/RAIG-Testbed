#include "world/world_interface.hpp"

namespace raig {

static std::uint64_t xorshift64(std::uint64_t& x) {
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    return x;
}

WorldInterface::WorldInterface(const WorldConfig& cfg) : state_(cfg.seed ? cfg.seed : 1) {}

WorldStimulus WorldInterface::next() {
    // Minimal stimulus: deterministic PRNG in world only.
    auto r1 = xorshift64(state_);
    auto r2 = xorshift64(state_);
    WorldStimulus s;
    s.a = static_cast<double>(r1 % 1000) / 1000.0;
    s.b = static_cast<double>(r2 % 1000) / 1000.0;
    return s;
}

} // namespace raig
