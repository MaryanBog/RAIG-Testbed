# Real Artificial Intelligence Genesis (RAIG)
## Testbed Implementation — Version 1.0 (EN)

---

## 1. Implementation Targets and Principles

### 1.1 Implementation Objective

The objective of the RAIG Testbed implementation is to **faithfully realize the Specification and Design documents** without introducing additional semantics, control logic, or interpretive layers.

The implementation must:

- preserve the ontological assumptions of the system,
- enforce architectural boundaries at the code level,
- remain minimal, auditable, and deterministic,
- avoid any mechanism not explicitly required by the design.

Correctness with respect to the documents takes absolute precedence over performance, extensibility, or convenience.

---

### 1.2 Language and Standard

The implementation language is **C++20**.

The codebase must:

- compile under ISO C++20 without extensions,
- avoid reliance on undefined or implementation-defined behavior,
- adhere strictly to documented build flags and toolchain constraints.

Later language standards are not permitted unless the Implementation document is revised.

---

### 1.3 Scope of This Document

This Implementation document defines:

- source code structure and layout,
- concrete data types and ownership rules,
- function-level responsibilities and interfaces,
- execution sequencing and error handling strategy,
- determinism and reproducibility guarantees.

This document does **not** contain full source code.  
It provides an unambiguous blueprint from which code can be written without design decisions.

---

### 1.4 Mapping to Design Components

Each implementation unit corresponds directly to a Design component:

- Genesis Core → `genesis_core`
- World Interface → `world`
- Telemetry Emitter → `telemetry`
- Runtime Supervisor → `runtime`

No additional top-level components may be introduced.

---

### 1.5 Single-Instance Assumption

The implementation assumes:

- exactly one process instance per experiment,
- exactly one internal structural state,
- exactly one execution loop.

Any code path enabling duplication, restart, or continuation violates this assumption and is prohibited.

---

### 1.6 Determinism Requirement

Given:

- identical source code,
- identical compiler and flags,
- identical initial configuration,
- identical world stimulus seed,

the implementation must produce identical telemetry output sequences.

All sources of nondeterminism must be explicitly identified and isolated.

---

### 1.7 Error Handling Philosophy

The implementation distinguishes between:

- **structural termination** (detected by the Genesis Core),
- **runtime failure** (detected by the implementation).

Both cases result in immediate process termination after death recording.

No recovery, retry, or fallback logic is permitted.

---

### 1.8 Implementation Non-Goals

The implementation explicitly avoids:

- dynamic plugin systems,
- runtime reflection,
- hot-reload mechanisms,
- scripting languages,
- embedded interpreters,
- dynamic code generation.

The executable behavior must remain static throughout the lifetime of an instance.

---

### 1.9 Implementation Readiness Criteria

The implementation phase is considered complete only when:

- all components are implemented exactly as specified,
- static analysis reports no violations,
- integration tests confirm architectural isolation,
- compliance matrix entries for implementation are satisfied.

No code may be executed in an experimental run before these criteria are met.

---

## 2. Repository Structure and Directory Layout

### 2.1 Repository Organization Principles

The repository structure must reflect the architectural boundaries defined in the Design document.

The layout must:

- make component boundaries explicit,
- prevent accidental coupling between modules,
- support static analysis and auditability,
- discourage feature creep and hidden dependencies.

Directory structure is treated as part of the system design, not a convenience.

---

### 2.2 Top-Level Repository Layout

The top-level repository layout is defined as follows:

    raig-testbed/
    ├── CMakeLists.txt
    ├── README.md
    ├── LICENSE
    ├── docs/
    │   ├── RAIG-Testbed-Specification-V1.0-EN.md
    │   ├── RAIG-Testbed-Design-V1.0-EN.md
    │   └── RAIG-Testbed-Implementation-V1.0-EN.md
    ├── src/
    │   ├── genesis_core/
    │   ├── world/
    │   ├── telemetry/
    │   ├── runtime/
    │   └── main.cpp
    ├── include/
    │   ├── genesis_core/
    │   ├── world/
    │   ├── telemetry/
    │   └── runtime/
    ├── tests/
    │   ├── unit/
    │   ├── integration/
    │   └── properties/
    ├── tools/
    │   ├── static_analysis/
    │   └── build/
    └── scripts/

No additional top-level directories may be introduced without revising this document.

---

### 2.3 Source and Header Separation

Public interfaces are declared in the `include/` directory.

Implementation details reside exclusively in the `src/` directory.

Rules:

- headers in `include/` must not include implementation headers,
- headers must avoid exposing internal data layouts,
- implementation files must not be included outside their component.

This separation enforces encapsulation at the build-system level.

---

### 2.4 Genesis Core Layout

The Genesis Core component layout is:

    include/genesis_core/
    └── genesis_core.hpp

    src/genesis_core/
    ├── genesis_core.cpp
    ├── structural_state.cpp
    └── evolution_operator.cpp

Constraints:

- `structural_state.cpp` contains the full definition of the internal state,
- no header exposes the layout of the structural state,
- the evolution operator is isolated from telemetry and world logic.

---

### 2.5 World Component Layout

The World component layout is:

    include/world/
    └── world_interface.hpp

    src/world/
    ├── world_interface.cpp
    ├── world_state.cpp
    └── stimulus_generators.cpp

World logic must not include or reference Genesis Core headers beyond the declared interface.

---

### 2.6 Telemetry Component Layout

The Telemetry component layout is:

    include/telemetry/
    └── telemetry_emitter.hpp

    src/telemetry/
    ├── telemetry_emitter.cpp
    ├── serializers.cpp
    └── log_rotation.cpp

Telemetry code must not include Genesis Core implementation headers.

---

### 2.7 Runtime Component Layout

The Runtime component layout is:

    include/runtime/
    └── runtime_supervisor.hpp

    src/runtime/
    ├── runtime_supervisor.cpp
    ├── pacing.cpp
    └── termination.cpp

Runtime logic must not inspect or modify internal structural semantics.

---

### 2.8 Entry Point

The entry point of the system is:

    src/main.cpp

Responsibilities of `main.cpp` are limited to:

- reading immutable configuration,
- constructing components,
- starting the execution loop,
- delegating control to the Runtime Supervisor.

No experimental logic is permitted in the entry point.

---

### 2.9 Test Directory Structure

The `tests/` directory is structured as:

    tests/unit/        // component-level tests
    tests/integration/ // cross-component isolation tests
    tests/properties/  // invariant and determinism tests

Test code must never be compiled into production binaries.

---

### 2.10 Documentation Co-Location

All authoritative documents are stored in `docs/` and versioned alongside the code.

Code comments must reference document sections where relevant.

The documentation, not the code, is the primary source of truth.

---

## 3. Data Types and Ownership Model

### 3.1 Data Type Design Principles

All data types in the RAIG Testbed must adhere to the following principles:

- explicit ownership and lifetime,
- minimal surface area,
- no implicit sharing,
- no semantic interpretation.

Data types are structural carriers only.  
They do not encode meaning, goals, or interpretation.

---

### 3.2 Structural State Type

The internal structural state type represents the complete internal condition of the Genesis Core.

Properties:

- defined only in implementation files,
- opaque to all external components,
- non-copyable and non-movable,
- owned exclusively by the Genesis Core.

The structural state must not expose public fields or accessors.

---

### 3.3 Structural Variable Representation

Structural variables (Δ, Φ, M, κ):

- are represented using fixed-width floating-point or scalar types,
- follow strict IEEE-754 semantics,
- are updated only by the evolution operator.

No structural variable may be directly modified outside the evolution step.

---

### 3.4 World Stimulus Type

The world stimulus type represents external perturbation.

Properties:

- small and trivially copyable,
- contains no references or pointers,
- encodes magnitude and direction only,
- contains no semantic labels.

Stimulus types must be immutable after creation.

---

### 3.5 Diagnostic Snapshot Type

The diagnostic snapshot is a reduced projection of the internal state.

Properties:

- contains only admissible telemetry signals,
- does not contain structural topology,
- does not contain historical memory,
- is safe to serialize and persist.

The snapshot must be immutable once produced.

---

### 3.6 Ownership Boundaries

Ownership boundaries are strictly enforced:

- Genesis Core owns the structural state,
- World Interface owns its internal world state,
- Telemetry Emitter owns serialization buffers,
- Runtime Supervisor owns execution control flow.

No component may assume ownership of another component’s internal data.

---

### 3.7 Lifetime Rules

Lifetime rules are as follows:

- all long-lived objects are constructed at startup,
- no component is destroyed before process termination,
- destruction order follows reverse construction order,
- no dynamic object resurrection is permitted.

All lifetimes are bounded by the lifetime of the process.

---

### 3.8 Prohibited Data Patterns

The following patterns are prohibited:

- shared mutable state,
- reference-counted ownership across components,
- global variables with dynamic lifetime,
- hidden static locals with state.

These patterns introduce implicit coupling and nondeterminism.

---

### 3.9 Const-Correctness

Const-correctness is mandatory.

- read-only access must be expressed explicitly,
- mutation must be localized and visible,
- diagnostic data must be const after creation.

Const violations are treated as design violations.

---

### 3.10 Auditing Data Ownership

Data ownership rules must be auditable by:

- inspection of type definitions,
- inspection of constructor/destructor paths,
- static analysis of reference and pointer usage.

Any ambiguity in ownership invalidates the implementation.

---

## 4. Core APIs and Evolution Functions

### 4.1 Core API Objectives

The Core APIs define the **only permissible interaction surface** with the Genesis Core.

Their objectives are to:

- expose a minimal and auditable interface,
- prevent external inspection or mutation of internal structure,
- enforce a single evolution pathway,
- ensure deterministic progression of the structural state.

No API may expose semantics beyond what is strictly required.

---

### 4.2 Genesis Core Public API

The public API of the Genesis Core is intentionally minimal.

Permitted public operations are limited to:

- construction with immutable initial conditions,
- advancement of the structural state by one step,
- emission of a reduced diagnostic snapshot.

No other public methods are allowed.

---

### 4.3 Construction and Initialization

Construction of the Genesis Core:

- occurs exactly once per instance,
- initializes the internal structural state,
- binds the evolution operator permanently.

Initialization must not:

- depend on runtime measurements,
- embed semantic structure,
- encode goals or priors.

All initial conditions are treated as structural parameters, not knowledge.

---

### 4.4 Evolution Step Function

The evolution step function advances the structural state by exactly one step.

Properties of the evolution step:

- deterministic with respect to current state and stimulus,
- side-effect free outside the structural state,
- free of branching on outcomes,
- invariant in structure across runtime.

The evolution step must not:

- read telemetry,
- access wall-clock time,
- allocate unbounded memory,
- modify control flow.

---

### 4.5 Evolution Operator Structure

The evolution operator is implemented as a pure transformation over the structural state.

Conceptually, it performs:

- admissible deformation induced by stimulus,
- internal structural update,
- enforcement of invariants.

The operator must be isolated in its own implementation unit to prevent accidental coupling.

---

### 4.6 Invariant Enforcement

Structural invariants are enforced inside the evolution operator.

Examples include:

- monotonicity of memory accumulation,
- boundedness of contractivity,
- numerical stability constraints.

Invariant violations are treated as **structural failure conditions** and must trigger termination.

---

### 4.7 Diagnostic Snapshot Generation

Diagnostic snapshots are generated after the evolution step.

Snapshot generation:

- derives reduced indicators from the internal state,
- performs no mutation,
- discards structural topology and history.

Snapshot generation must be computationally lightweight and deterministic.

---

### 4.8 API Error Semantics

The Core API does not return recoverable errors.

All error conditions are classified as:

- structural failure, or
- runtime failure.

Both conditions result in immediate process termination after death recording.

No error codes, exceptions, or retries are permitted in the Core API.

---

### 4.9 Prohibited Core API Extensions

The following API extensions are prohibited:

- state inspection or debugging hooks,
- partial state updates,
- rollback or checkpoint interfaces,
- adaptive parameter modification,
- introspective or reflective APIs.

Any such extension invalidates the implementation.

---

### 4.10 Auditing Core APIs

Core APIs must be auditable through:

- inspection of public headers,
- verification of minimal method set,
- static analysis ensuring no hidden entry points.

Any additional externally callable path into the Genesis Core invalidates the design.

---

## 5. World Stimulus Algorithms

### 5.1 World Algorithm Objectives

World stimulus algorithms are responsible for generating external perturbations that introduce variation into the RAIG Testbed without embedding semantics, intent, or feedback.

Their objectives are to:

- prevent trivial isolation of the Genesis Core,
- expose the system to structured uncertainty,
- remain independent of system behavior.

World algorithms do not attempt to guide, teach, or optimize the system.

---

### 5.2 Algorithm Selection Constraints

All world stimulus algorithms must satisfy the following constraints:

- no access to Genesis Core internals,
- no access to telemetry data,
- no adaptation based on observed behavior,
- no dependence on system state.

Algorithm selection is fixed at initialization and must not change during runtime.

---

### 5.3 Deterministic Pseudorandom Sources

Stochastic stimulus generation must rely on deterministic pseudorandom number generators.

Requirements:

- explicit seeding at startup,
- reproducible sequences given identical seeds,
- isolation from other system components.

The PRNG must not be shared with the Genesis Core.

---

### 5.4 Background Noise Algorithm

The background noise algorithm generates continuous low-amplitude variation.

Characteristics:

- stationary statistical properties,
- bounded magnitude,
- zero-mean or symmetric distribution.

This mode provides persistent minimal perturbation without structure.

---

### 5.5 Regime-Based Algorithms

Regime-based algorithms generate temporally extended periods with consistent statistical behavior.

Characteristics:

- predefined regime parameters,
- deterministic regime transitions,
- independence from system behavior.

Regimes may differ in variance, correlation, or frequency content.

---

### 5.6 Impulse Algorithms

Impulse algorithms generate rare, high-amplitude perturbations.

Characteristics:

- low occurrence probability,
- high but bounded magnitude,
- no warning or preparatory signal.

Impulses must not be synchronized with system states or telemetry.

---

### 5.7 Drift Algorithms

Drift algorithms introduce slow, continuous changes in stimulus characteristics.

Characteristics:

- gradual parameter evolution,
- long time scales,
- bounded parameter ranges.

Drift must be predefined and not responsive to system behavior.

---

### 5.8 Composition of Algorithms

Multiple stimulus algorithms may be composed.

Composition rules:

- linear or predefined nonlinear combination,
- fixed composition structure,
- no adaptive weighting.

Composition must not introduce feedback paths.

---

### 5.9 Algorithm Parameterization

Algorithm parameters:

- are specified at initialization,
- remain constant during runtime,
- are documented in configuration metadata.

Parameter tuning during runtime is prohibited.

---

### 5.10 Algorithm Auditing

World stimulus algorithms must be auditable through:

- inspection of algorithm definitions,
- verification of state independence,
- deterministic replay under fixed seeds.

Any adaptive or feedback-driven behavior invalidates the implementation.

---

## 6. Telemetry Serialization and Storage

### 6.1 Telemetry Implementation Objectives

The telemetry subsystem must provide a **faithful, append-only record** of admissible diagnostic projections without influencing system behavior.

Implementation objectives:

- guarantee write-only telemetry flow,
- ensure long-lived persistence,
- preserve ordering and integrity,
- avoid any read-back or feedback path.

Telemetry exists for observation only.

---

### 6.2 Serialization Strategy

Telemetry serialization must be:

- deterministic,
- schema-driven,
- stable across versions.

Each telemetry record is serialized independently and appended to persistent storage.

Serialization must not:

- allocate unbounded memory,
- perform complex transformations,
- depend on external libraries with hidden behavior.

---

### 6.3 Telemetry Record Structure

Each telemetry record must include:

- a monotonic step counter,
- a monotonic internal time indicator,
- admissible diagnostic values,
- schema version identifier,
- build and instance identifiers.

Records must not include:

- raw internal state values,
- memory contents,
- intermediate computation artifacts.

---

### 6.4 Storage Medium

Telemetry storage is implemented as an append-only log.

Permissible storage targets include:

- local files on persistent volumes,
- append-only streams redirected to external collectors.

The storage medium must not allow in-place modification of existing records.

---

### 6.5 Log Rotation Policy

Log rotation is required for long-lived execution.

Rotation policy:

- size-based or time-based,
- creation of new log segments without deleting old ones,
- monotonically increasing segment identifiers.

Rotation must not truncate or overwrite historical data.

---

### 6.6 Failure Handling During Telemetry

Telemetry failures are handled as follows:

- serialization failures → runtime failure,
- storage unavailability → continue execution if possible,
- partial writes → record failure and terminate.

Telemetry integrity is prioritized over continued execution.

---

### 6.7 Write Isolation

Telemetry writing must be isolated from the Genesis Core.

Isolation guarantees:

- no shared buffers between core and storage,
- no blocking of the evolution step,
- no synchronization primitives that affect core timing.

If asynchronous writing is used, it must not reorder records.

---

### 6.8 Versioning and Compatibility

Telemetry schemas must be versioned explicitly.

Schema changes require:

- incremented schema version,
- documentation update,
- compatibility notes.

Old telemetry must remain interpretable after schema updates.

---

### 6.9 Metadata Emission

Telemetry logs must include a metadata header containing:

- implementation version,
- compiler and build flags,
- platform identifiers,
- world generator configuration summary.

Metadata is emitted once at startup and never modified.

---

### 6.10 Auditing Telemetry Implementation

Telemetry implementation must be auditable by:

- inspecting serialization code paths,
- verifying append-only semantics,
- confirming absence of read-back operations.

Any telemetry feedback path invalidates the implementation.

---

## 7. Timing, Scheduling, and Pacing

### 7.1 Timing Objectives

Timing in the RAIG Testbed serves **operational pacing only**.

The timing subsystem must:

- regulate wall-clock execution rate,
- avoid resource exhaustion,
- preserve determinism of structural evolution.

Timing must never influence internal dynamics, state transitions, or decision logic.

---

### 7.2 Internal vs Wall-Clock Time

Two notions of time exist:

- **Internal Time**  
  Defined implicitly by the number of completed evolution steps.

- **Wall-Clock Time**  
  Used solely to pace execution and manage resource usage.

Internal time is authoritative for all experimental semantics.  
Wall-clock time has no semantic meaning.

---

### 7.3 Pacing Strategy

Execution pacing may be implemented using:

- fixed sleep intervals,
- steady timers,
- coarse-grained scheduling primitives.

The pacing mechanism must:

- execute after telemetry emission,
- not accumulate corrective feedback,
- not skip or compress evolution steps.

---

### 7.4 Fixed-Step Execution

The execution loop advances the system by exactly one evolution step per iteration.

There is no concept of:

- variable-step integration,
- adaptive timestep selection,
- step aggregation or subdivision.

Each step is treated as an indivisible unit of evolution.

---

### 7.5 Scheduling Constraints

The RAIG Testbed must not:

- spawn background worker threads affecting core timing,
- rely on OS-level scheduling hints,
- dynamically adjust priorities.

If multithreading is used for telemetry I/O, it must be strictly isolated and must not influence the core loop cadence.

---

### 7.6 Sleep and Delay Semantics

Sleep or delay operations:

- are best-effort and non-compensatory,
- do not attempt to “catch up” if delayed,
- do not alter internal step counting.

Missed or delayed wall-clock pacing does not affect internal evolution semantics.

---

### 7.7 Clock Source Selection

Wall-clock timing must rely on monotonic clock sources.

Requirements:

- monotonic progression,
- immunity to system clock adjustments,
- sufficient resolution for pacing needs.

Time-of-day clocks must not be used for pacing logic.

---

### 7.8 Determinism Under Timing Variability

Variations in wall-clock timing must not affect:

- sequence of structural states,
- emitted telemetry values,
- termination conditions.

Given identical stimuli, internal evolution must be invariant under different pacing speeds.

---

### 7.9 Timing Failure Modes

Timing-related failures include:

- inability to sleep or delay,
- clock source failure,
- extreme scheduling starvation.

Such failures are treated as runtime failures and result in instance termination.

---

### 7.10 Auditing Timing Behavior

Timing behavior must be auditable by:

- inspection of pacing logic,
- verification of clock source usage,
- confirmation that timing does not enter evolution logic.

Any coupling between timing and structural evolution invalidates the implementation.

---

## 8. Error Model and Termination Semantics

### 8.1 Error Model Philosophy

The RAIG Testbed employs a **fail-fast, non-recoverative error model**.

Errors are not handled to preserve execution continuity.  
They are handled to preserve **experimental correctness**.

Any error that compromises determinism, structural integrity, or architectural isolation results in **immediate termination** of the current instance.

---

### 8.2 Error Categories

Errors are classified into the following categories:

- **Structural Errors**  
  Violations arising from internal structural dynamics (e.g., collapse of viability).

- **Runtime Errors**  
  Violations arising from implementation correctness (e.g., invariant violation, UB detection).

- **Infrastructure Errors**  
  External termination events imposed by the hosting environment.

All categories are terminal.

---

### 8.3 Structural Error Handling

Structural errors are detected exclusively by the Genesis Core.

Upon detection:

- the error condition is classified,
- a final diagnostic snapshot is produced if possible,
- a death record is emitted,
- the process terminates.

No attempt is made to stabilize or compensate for structural failure.

---

### 8.4 Runtime Error Handling

Runtime errors include:

- assertion failures,
- detected undefined behavior,
- numerical domain violations,
- memory safety violations.

Upon detection of a runtime error:

- execution is aborted immediately,
- a death record is emitted if feasible,
- the process exits with a non-zero status.

Runtime errors invalidate the instance.

---

### 8.5 Infrastructure Error Semantics

Infrastructure errors occur outside the control of the system.

Examples include:

- forced container termination,
- resource exhaustion enforced by the platform,
- host or VM shutdown.

Infrastructure errors are recorded externally and treated as instance death without structural attribution.

---

### 8.6 Death Record Structure

A death record must include:

- termination timestamp,
- termination category,
- last completed internal step,
- last successfully emitted telemetry reference,
- build and instance identifiers.

The death record must not include internal structural state.

---

### 8.7 Termination Ordering

The termination sequence is strictly ordered:

1. detect terminal condition,
2. emit final telemetry and death record if possible,
3. terminate process.

No cleanup logic may modify internal state prior to termination.

---

### 8.8 Exit Codes

Exit codes must be deterministic and documented.

Distinct exit codes should correspond to:

- structural termination,
- runtime failure,
- infrastructure termination.

Exit codes exist for external classification only and have no semantic meaning inside the system.

---

### 8.9 Absence of Recovery Paths

The implementation must not include:

- retry logic,
- fallback modes,
- degraded operation paths,
- silent continuation after error.

Any such mechanism invalidates the error model.

---

### 8.10 Auditing Error Handling

Error handling behavior must be auditable through:

- inspection of termination paths,
- verification of immediate exits,
- analysis of emitted death records.

Hidden recovery or suppression of errors invalidates the implementation.

---

## 9. Determinism and Reproducibility Plan

### 9.1 Determinism Objective

Determinism in the RAIG Testbed ensures that **observed differences in behavior arise solely from differences in initial conditions or external stimuli**, not from hidden implementation variability.

Determinism is a prerequisite for scientific interpretability.

---

### 9.2 Sources of Nondeterminism

Potential sources of nondeterminism include:

- uncontrolled random number generation,
- floating-point non-determinism,
- undefined or implementation-defined behavior,
- data races or concurrency effects,
- platform-dependent library behavior,
- build and toolchain variability.

All such sources must be explicitly identified and controlled.

---

### 9.3 Randomness Control

All randomness must:

- originate exclusively in the World Generator,
- use a deterministic PRNG,
- be explicitly seeded at startup,
- be isolated from other components.

The seed value must be recorded in telemetry metadata.

---

### 9.4 Floating-Point Determinism

Floating-point behavior must be deterministic.

Requirements include:

- strict IEEE-754 compliance,
- disabled fast-math optimizations,
- consistent rounding modes,
- avoidance of undefined numeric operations.

Floating-point assumptions must be validated through static analysis and testing.

---

### 9.5 Single-Threaded Core Guarantee

The Genesis Core must execute in a single thread.

If auxiliary threads are used (e.g., for telemetry I/O):

- they must not access the structural state,
- they must not affect execution order or timing,
- they must not introduce data races.

Core determinism must be provable by inspection.

---

### 9.6 Build Reproducibility

Build reproducibility requires:

- pinned compiler versions,
- pinned dependency versions,
- deterministic build flags,
- clean build environments.

Build metadata must be embedded in telemetry headers.

---

### 9.7 Platform Variability

Platform-induced variability must be minimized.

Recommendations include:

- containerized execution,
- fixed OS images,
- consistent hardware architectures.

Platform details must be recorded for each run.

---

### 9.8 Determinism Verification

Determinism must be verified by:

- repeated runs with identical configuration,
- byte-for-byte comparison of telemetry logs,
- controlled variation of stimulus seeds.

Any unexplained divergence invalidates the run.

---

### 9.9 Reproducibility Documentation

Reproducibility procedures must be documented, including:

- environment setup,
- build steps,
- execution parameters,
- verification steps.

Reproducibility is a property of the experiment, not just the code.

---

### 9.10 Limits of Reproducibility

Perfect reproducibility is not guaranteed across all platforms.

Reproducibility claims are limited to:

- identical software stacks,
- identical hardware architectures,
- identical configurations.

Deviations beyond these limits must be explicitly acknowledged.