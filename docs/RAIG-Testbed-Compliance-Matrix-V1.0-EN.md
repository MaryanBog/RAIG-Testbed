# Real Artificial Intelligence Genesis (RAIG)
## Testbed Compliance Matrix — Version 1.0 (EN)

---

## 1. Purpose of the Compliance Matrix

### 1.1 Objective

The Compliance Matrix provides a **formal, auditable mapping** between:

- theoretical and experimental requirements,
- their concrete implementation mechanisms,
- the methods used to verify compliance,
- the current compliance status.

The matrix exists to prevent:

- implicit assumption drift,
- undocumented deviations,
- post-hoc reinterpretation of results.

---

### 1.2 Scope

This matrix covers compliance with:

- RAIG Testbed Specification,
- RAIG Testbed Design,
- RAIG Testbed Implementation,
- Static Analysis requirements,
- Integration constraints,
- Theoretical Validation criteria.

Compliance is evaluated per requirement, not per component.

---

## 2. Compliance Classification

Each requirement is classified as:

- **MUST** — mandatory for experimental validity  
- **SHOULD** — strongly recommended, deviation must be justified  
- **MAY** — optional, does not affect validity  

Only MUST-level violations invalidate an experiment.

---

## 3. Compliance Matrix

| ID | Requirement | Source Document | Level | Verification Method | Evidence | Status |
|----|-------------|----------------|-------|---------------------|----------|--------|
| C-01 | Single persistent instance per run | Specification §3 | MUST | Deployment inspection | Deployment config | PENDING |
| C-02 | No state reset or rollback | Specification §4 | MUST | Code inspection + tests | Core source | PENDING |
| C-03 | Single structural state ontology | Design §1, §5 | MUST | Architecture audit | Design doc | PENDING |
| C-04 | Opaque internal structural state | Design §5 | MUST | Header boundary audit | Core headers | PENDING |
| C-05 | No external control or feedback | Design §2, §6 | MUST | Integration tests | Integration report | PENDING |
| C-06 | Deterministic evolution operator | Implementation §4 | MUST | Replay tests | Telemetry diff | PENDING |
| C-07 | World independence | Design §4 | MUST | Stimulus replay | World logs | PENDING |
| C-08 | Telemetry is observational only | Design §6 | MUST | Data flow audit | Code review | PENDING |
| C-09 | Memory irreversibility (M monotonic) | Theoretical Validation §3 | MUST | Telemetry trend check | Analysis plots | PENDING |
| C-10 | Contractivity boundedness | Theoretical Validation §4 | MUST | Telemetry analysis | κ time series | PENDING |
| C-11 | Fail-fast termination semantics | Design §8 | MUST | Fault injection | Exit traces | PENDING |
| C-12 | No adaptive runtime parameters | Specification §3 | MUST | Config audit | Startup config | PENDING |
| C-13 | Strict IEEE-754 compliance | Static Analysis §5 | MUST | Compiler flags + tests | Build logs | PENDING |
| C-14 | Absence of UB | Static Analysis §2 | MUST | UBSan/ASan | Sanitizer logs | PENDING |
| C-15 | Single-threaded core | Implementation §9 | MUST | Thread audit | Code inspection | PENDING |
| C-16 | Telemetry append-only storage | Design §6 | MUST | FS audit | Log samples | PENDING |
| C-17 | No internal semantic encoding | Specification §2 | MUST | Code & data audit | Source review | PENDING |
| C-18 | Collapse treated as valid outcome | Specification §7 | MUST | Termination tests | Death records | PENDING |
| C-19 | No intelligence claims in system | Specification §10 | MUST | Documentation audit | Docs | PENDING |
| C-20 | Reproducible build | Implementation §9 | SHOULD | Rebuild diff | Build hashes | PENDING |

---

## 4. Evidence Requirements

Each compliance item requires:

- a concrete artifact (log, config, source reference),
- a verification timestamp,
- a verifier identifier.

Assertions without evidence are invalid.

---

## 5. Compliance Status Lifecycle

Compliance status values:

- **PENDING** — not yet verified  
- **PASS** — verified and compliant  
- **FAIL** — verified and non-compliant  
- **WAIVED** — deviation accepted with justification  

Only PASS or WAIVED are admissible for experimental runs.

---

## 6. Handling Non-Compliance

If a MUST-level requirement is marked FAIL:

- the experiment is invalid,
- no interpretation of results is permitted,
- corrective action is required,
- re-verification is mandatory.

SHOULD-level failures require documented justification.

---

## 7. Traceability

Each compliance item must trace to:

- a specific document section,
- a specific implementation artifact,
- a specific verification method.

Traceability ensures reproducibility and accountability.

---

## 8. Compliance as Experimental Gate

The Compliance Matrix is a **hard gate**.

No experimental run may be initiated until:

- all MUST items are PASS or WAIVED,
- evidence is documented,
- the matrix is reviewed and signed.

Compliance is a property of the experiment, not the code alone.