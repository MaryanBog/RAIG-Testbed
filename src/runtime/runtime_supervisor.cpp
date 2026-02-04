#include "runtime/runtime_supervisor.hpp"

namespace raig {

RuntimeSupervisor::RuntimeSupervisor(const RuntimeConfig& cfg) : cfg_(cfg) {}

bool RuntimeSupervisor::should_continue(std::uint64_t step) const {
    return step < cfg_.max_steps;
}

} // namespace raig
