# Real Artificial Intelligence Genesis (RAIG)
## Testbed Auto-Test Suite — Version 1.0 (EN)

---

## 1. Auto-Test Suite Objectives

### 1.1 Purpose

The Auto-Test Suite verifies that the RAIG Testbed **preserves invariants, determinism, and architectural constraints** at the implementation level.

The test suite does **not** test intelligence, behavior, or performance.  
It tests **correctness of constraints**.

---

### 1.2 Scope

This document defines:

- categories of automated tests,
- test targets and coverage,
- pass/fail criteria,
- test execution order and gating.

All tests are mandatory unless explicitly marked otherwise.

---

## 2. Test Categories

### 2.1 Unit Tests

Unit tests validate correctness of **isolated components**.

Targets include:

- Genesis Core API boundaries,
- World stimulus generators,
- Telemetry serialization routines,
- Runtime pacing utilities.

Unit tests must not mock or inspect internal structural state.

---

### 2.2 Property Tests

Property tests validate **invariants over time**, not single outcomes.

Properties include:

- monotonicity of memory indicators,
- boundedness of contractivity,
- absence of NaN/Inf propagation,
- determinism under fixed seeds.

Property tests operate over multiple steps and long sequences.

---

### 2.3 Integration Tests

Integration tests validate **component interaction constraints**.

Targets include:

- one-way data flow enforcement,
- telemetry isolation,
- world independence,
- execution loop ordering.

Integration tests must fail if any forbidden coupling is detected.

---

## 3. Genesis Core Tests

### 3.1 API Surface Tests

Verify that:

- only documented public methods exist,
- no debug or inspection hooks are exposed,
- state mutation occurs only through the evolution step.

Any additional callable interface invalidates the test.

---

### 3.2 Invariant Enforcement Tests

Verify that:

- memory indicators never decrease,
- structural variables remain within admissible numeric domains,
- invariant violations trigger termination.

Invariant violation tests must assert immediate failure.

---

## 4. World Generator Tests

### 4.1 Deterministic Replay Tests

Verify that:

- identical seeds produce identical stimulus sequences,
- different seeds produce divergent sequences.

Stimulus replay must be bitwise identical under identical conditions.

---

### 4.2 Independence Tests

Verify that:

- world generators do not access telemetry,
- world generators do not depend on core state,
- stimulus generation remains unaffected by runtime history.

---

## 5. Telemetry Tests

### 5.1 Serialization Integrity Tests

Verify that:

- telemetry records are append-only,
- schema fields are complete and ordered,
- no internal state is leaked.

Malformed or partial records must be detected.

---

### 5.2 No-Feedback Tests

Verify that:

- telemetry emission does not affect evolution,
- telemetry failures do not modify core behavior.

Telemetry must remain observational only.

---

## 6. Determinism Tests

### 6.1 Full-Run Replay Tests

Given:

- identical initial configuration,
- identical world seed,

verify that:

- full telemetry logs are byte-for-byte identical across runs.

Any divergence is a failure.

---

### 6.2 Timing Variability Tests

Verify that:

- different wall-clock pacing produces identical telemetry,
- timing jitter does not affect evolution.

---

## 7. Failure and Termination Tests

### 7.1 Structural Failure Tests

Induce admissible structural collapse and verify that:

- termination occurs,
- final telemetry is emitted,
- death record is recorded.

---

### 7.2 Runtime Failure Tests

Induce runtime errors and verify that:

- process terminates immediately,
- no continuation occurs,
- exit codes match specification.

---

## 8. Test Isolation Rules

Tests must:

- not reuse production binaries,
- not modify production configuration,
- not persist or reuse internal state.

Each test run must be isolated and disposable.

---

## 9. Test Execution Policy

### 9.1 Execution Order

Tests must be executed in the following order:

1. Unit Tests  
2. Property Tests  
3. Integration Tests  
4. Determinism Tests  
5. Failure Tests  

Failure at any stage blocks progression.

---

### 9.2 Continuous Integration

All tests must be executed:

- on every code change,
- on every build configuration,
- with pinned toolchains.

CI failures block experimental deployment.

---

## 10. Acceptance Criteria

The Auto-Test Suite is considered passed only if:

- all mandatory tests pass,
- no flaky or nondeterministic behavior is observed,
- test results are documented and archived.

Passing the Auto-Test Suite is required before **Development & Certification Lifecycle**.