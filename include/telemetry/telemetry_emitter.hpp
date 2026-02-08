#pragma once
#include <string>
#include "genesis_core/genesis_core.hpp"

namespace raig {

struct TelemetryConfig final {
    std::string out_path = "telemetry.jsonl";
};

class TelemetryEmitter final {
public:
    explicit TelemetryEmitter(const TelemetryConfig& cfg);
    void emit(const DiagnosticSnapshot& snapshot);

private:
    TelemetryConfig cfg_;
};

} // namespace raig
