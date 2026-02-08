# Real Artificial Intelligence Genesis (RAIG)
## Testbed Run Log — Version 1.0

---

## 1. Purpose

This document records a **single concrete experimental run** of the RAIG Testbed,
binding an actual execution to the previously defined specification,
implementation, and validation framework.

This log contains **facts only**.
No interpretation, intelligence claims, or theoretical extrapolation are included.

---

## 2. System Identification

- Project: RAIG Testbed
- Version: v1.0.0
- Repository path: Flexion-Core/RAIG
- Execution type: Single-instance autonomous run

---

## 3. Source State

- Git branch: `main`
- Git tag: `v1.0.0` (baseline)
- Working tree: clean at execution time

---

## 4. Execution Environment

- Operating system: Windows 11
- Shell: Git Bash
- Toolchain:
  - MSYS2 UCRT64
  - Compiler: `g++ (ucrt64)`
  - C++ standard: C++20
- Build system:
  - CMake
  - Ninja
- Build type: default (no instrumentation)

---

## 5. Runtime Configuration

- Initial conditions:
  - Genesis seed: `0`
- World configuration:
  - World seed: `1`
- Runtime parameters:
  - `max_steps = 1000000`
  - `pace_ms = 0`
- Telemetry output:
  - File: `telemetry_long.jsonl`
  - Mode: append-only

---

## 6. Execution Summary

- Execution start: successful
- Execution end: **endogenous termination**
- Total steps executed: `878149`

### Termination State

- `alive`: `false`
- `death_reason`: `3 (KappaZero)`
- Final structural indicators:
  - `kappa = 0`
  - `mmag ≈ 13.6858`
  - `phi ≈ 0.0069`
  - `dmag ≈ 0.0082`

Termination occurred strictly according to documented viability collapse semantics.

---

## 7. Telemetry Integrity

- Telemetry file size: ~95 MB
- Format: JSONL (one record per step)
- Properties verified:
  - step monotonicity
  - append-only storage
  - single death record
  - no post-death evolution

### Sticky Death Verification

- Number of records with `"alive": false`: **1**
- This record is the final telemetry entry.

---

## 8. Test and Gate Status

All mandatory gates passed **prior to execution**:

- Build: PASS
- Auto-Test Suite: PASS
- Determinism checks: PASS
- Invariant enforcement: PASS
- Integration constraints: PASS

This run is admissible under the RAIG Testbed v1.0 framework.

---

## 9. Relation to Governing Documents

This run is governed by the following documents (v1.0):

- RAIG Testbed Specification
- RAIG Testbed Design
- RAIG Testbed Implementation
- RAIG Static Analysis
- RAIG Integration
- RAIG Auto-Test Suite
- RAIG Theoretical Validation
- RAIG Compliance Matrix
- RAIG Development & Certification Lifecycle

---

## 10. Interpretation Boundary

This run log **does not** claim:

- intelligence emergence
- cognitive behavior
- learning or adaptation
- success or failure in an evaluative sense

The recorded outcome represents a **valid negative experimental result**
within the defined structural framework.

---

## 11. Status

This experimental run is **complete, closed, and archived**.

Any further execution constitutes a **new run** and requires
a separate Run Log document.