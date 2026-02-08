#include "runtime/runtime_supervisor.hpp"
#include <thread>
#include <chrono>

namespace raig {

void RuntimeSupervisor::pace() const {
    if (cfg_.pace_ms > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(cfg_.pace_ms));
    }
}

} // namespace raig
