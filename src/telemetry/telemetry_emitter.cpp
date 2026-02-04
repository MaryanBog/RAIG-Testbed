#include "telemetry/telemetry_emitter.hpp"
#include <fstream>

namespace raig {

TelemetryEmitter::TelemetryEmitter(const TelemetryConfig& cfg) : cfg_(cfg) {}

void TelemetryEmitter::emit(const DiagnosticSnapshot& s) {
    std::ofstream out(cfg_.out_path, std::ios::app);
    out
      << "{"
      << "\"step\":" << s.step << ","
      << "\"kappa\":" << s.kappa << ","
      << "\"phi\":" << s.phi << ","
      << "\"dmag\":" << s.dmag << ","
      << "\"mmag\":" << s.mmag << ","
      << "\"alive\":" << (s.alive ? "true" : "false") << ","
      << "\"death_reason\":" << static_cast<int>(s.death_reason)
      << "}\n";
}

} // namespace raig
