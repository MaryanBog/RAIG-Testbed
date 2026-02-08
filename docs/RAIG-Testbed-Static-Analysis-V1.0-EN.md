# Real Artificial Intelligence Genesis (RAIG)
## Testbed Static Analysis — Version 1.0 (EN)

---

## 1. Static Analysis Objectives

### 1.1 Purpose

Static analysis in the RAIG Testbed serves to **prove the absence of implementation-level violations** that would invalidate experimental results.

The objectives are to:

- eliminate undefined behavior,
- enforce deterministic execution,
- guarantee memory safety,
- ensure numerical correctness,
- verify architectural isolation.

Static analysis is treated as a **formal experimental prerequisite**, not as a development aid.

---

### 1.2 Scope

This document defines:

- static analysis tools and configurations,
- prohibited language and library constructs,
- verification targets per component,
- acceptance criteria for experimental execution.

No experimental run may begin before all static analysis checks pass.

---

## 2. Undefined Behavior (UB) Elimination Policy

### 2.1 UB Prohibition

All forms of undefined behavior are strictly prohibited.

This includes, but is not limited to:

- signed integer overflow,
- invalid pointer arithmetic,
- out-of-bounds access,
- use-after-free,
- uninitialized memory reads,
- strict-aliasing violations.

Any detected UB invalidates the implementation.

---

### 2.2 Compiler-Assisted UB Detection

The codebase must compile with:

- warnings enabled at maximum level,
- warnings treated as errors,
- UB-related warnings explicitly enabled.

No warning may be suppressed without documented justification.

---

## 3. Memory Safety Analysis

### 3.1 Allocation Discipline

Memory allocation must follow these rules:

- no raw ownership of heap memory without RAII,
- no custom allocators unless explicitly documented,
- no memory pooling with hidden lifetimes.

Ownership must be statically verifiable by inspection.

---

### 3.2 Pointer and Reference Usage

Pointer and reference usage must satisfy:

- no dangling references,
- no aliasing across component boundaries,
- no exposure of internal state pointers.

Smart pointers are permitted only where ownership semantics are unambiguous.

---

## 4. Concurrency and Data Race Analysis

### 4.1 Core Single-Thread Guarantee

The Genesis Core must be single-threaded.

Static analysis must verify:

- absence of shared mutable state accessed across threads,
- no atomic operations inside the core,
- no synchronization primitives affecting core execution.

---

### 4.2 Auxiliary Thread Isolation

If auxiliary threads exist (e.g., telemetry I/O):

- they must not access core state,
- communication must be one-way,
- data exchange must use immutable snapshots.

Any data race invalidates the experiment.

---

## 5. Floating-Point and Numerical Analysis

### 5.1 IEEE-754 Compliance

Floating-point operations must adhere strictly to IEEE-754 semantics.

Requirements:

- no fast-math optimizations,
- no fused multiply-add unless explicitly controlled,
- consistent rounding mode.

Static analysis must verify that compiler flags preserve numeric semantics.

---

### 5.2 Numerical Domain Safety

The code must prevent:

- NaN propagation without detection,
- division by zero,
- overflow to infinity where not explicitly allowed.

Numerical domain checks must be explicit and auditable.

---

## 6. Determinism Enforcement

### 6.1 Deterministic Constructs Only

The following constructs are prohibited:

- non-deterministic standard library facilities,
- reliance on unspecified iteration order,
- time-dependent logic,
- address-based hashing.

Static analysis must flag any such usage.

---

### 6.2 Randomness Isolation

Static analysis must confirm that:

- PRNG usage exists only in the World Generator,
- seeds are explicitly set,
- no randomness leaks into the Genesis Core.

---

## 7. Tooling Requirements

### 7.1 Static Analysis Tools

The following tools (or equivalents) must be used:

- clang-tidy with a strict ruleset,
- compiler warnings as errors,
- optional static analyzers (e.g., cppcheck) for redundancy.

Tool versions must be pinned and documented.

---

### 7.2 Sanitizer Builds (Pre-Execution)

Before any experimental run, the code must pass:

- AddressSanitizer (ASan),
- UndefinedBehaviorSanitizer (UBSan).

Sanitizer builds are for verification only and must not be used for long-lived runs.

---

## 8. Architectural Compliance Verification

### 8.1 Boundary Enforcement

Static analysis must verify:

- no forbidden include dependencies,
- no cross-component header leakage,
- no access to hidden implementation units.

Component boundaries defined in the Design document are mandatory.

---

### 8.2 Forbidden Constructs

The following are prohibited and must be flagged:

- global mutable state,
- hidden static locals with state,
- dynamic code loading,
- reflection or introspection facilities.

---

## 9. Acceptance Criteria

### 9.1 Static Analysis Pass Conditions

Static analysis is considered passed only if:

- all checks succeed with zero errors,
- no warnings are suppressed,
- all violations are resolved or explicitly justified.

---

### 9.2 Documentation of Results

Static analysis results must be documented, including:

- tool versions,
- configuration files,
- summary of findings,
- confirmation of pass status.

Documentation is required before proceeding to Integration.

---

## 10. Static Analysis as Experimental Gate

Static analysis is a **hard gate**.

Failure to satisfy static analysis requirements prohibits:

- integration testing,
- experimental deployment,
- telemetry interpretation.

No exceptions are permitted.