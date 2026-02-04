#pragma once
#include <cstdint>

namespace raig {

struct RuntimeConfig final {
    std::uint64_t max_steps = 1000; // placeholder; long-run later
    int pace_ms = 0;               // 0 = no sleep
};

class RuntimeSupervisor final {
public:
    explicit RuntimeSupervisor(const RuntimeConfig& cfg);
    bool should_continue(std::uint64_t step) const;
    void pace() const;

private:
    RuntimeConfig cfg_;
};

} // namespace raig
