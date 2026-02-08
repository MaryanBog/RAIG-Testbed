# Real Artificial Intelligence Genesis (RAIG)
## Testbed Design — Version 1.0 (EN)

---

## 1. Architectural Overview

### 1.1 Design Objectives

The design of the RAIG Testbed follows directly from the Specification and serves a single objective:

**to realize a minimal, long-lived, autonomous structural process while strictly preserving experimental integrity.**

The architecture must:

- preserve single-instance semantics,
- enforce strict separation between internal evolution and observation,
- prevent hidden feedback or control loops,
- remain simple enough to audit exhaustively.

Architectural simplicity is treated as a **safety and correctness requirement**, not as an optimization.

---

### 1.2 High-Level System Structure

The RAIG Testbed is composed of four primary architectural components:

1. **Genesis Core**
2. **World Interface**
3. **Telemetry Emitter**
4. **Runtime Supervisor**

Each component has a single, non-overlapping responsibility and communicates through strictly defined interfaces.

No component is permitted to assume or infer the internal structure of any other component beyond what is explicitly defined.

---

### 1.3 Genesis Core

The **Genesis Core** is the only component that contains and evolves the internal structural state.

Responsibilities:

- maintain the structural state \(X(t)\),
- apply the fixed internal evolution operator,
- accept admissible external stimuli,
- advance internal time.

The Genesis Core:

- exposes no read access to its full state,
- exposes no write access except through the world interface,
- contains no logic related to observation, logging, or deployment.

The core is designed to be **ontologically closed**.

---

### 1.4 World Interface

The **World Interface** provides the sole coupling between the external world and the Genesis Core.

Responsibilities:

- generate external stimuli,
- deliver stimuli to the Genesis Core through a fixed interface,
- remain non-adaptive and state-independent.

The World Interface:

- does not observe telemetry,
- does not inspect internal state,
- does not alter behavior based on runtime history.

It exists solely to introduce variation.

---

### 1.5 Telemetry Emitter

The **Telemetry Emitter** produces diagnostic projections of the internal process.

Responsibilities:

- receive reduced diagnostic signals from the Genesis Core,
- serialize telemetry data,
- emit telemetry to external storage or streams.

The Telemetry Emitter:

- operates asynchronously,
- has no communication path back to the Genesis Core,
- cannot influence execution order or timing.

Telemetry is strictly observational.

---

### 1.6 Runtime Supervisor

The **Runtime Supervisor** manages process-level concerns.

Responsibilities:

- initialize components at process start,
- manage the main execution loop,
- handle orderly shutdown and failure reporting,
- interface with the hosting platform.

The Runtime Supervisor:

- does not interpret telemetry,
- does not adjust runtime parameters,
- does not modify evolution logic.

Its role is infrastructural, not cognitive or experimental.

---

### 1.7 Data Flow Summary

The system data flow is strictly unidirectional:

World Interface ──► Genesis Core ──► Telemetry Emitter ──► External Storage


No reverse paths exist.

Any architectural modification introducing reverse influence invalidates the design.

---

### 1.8 Architectural Non-Goals

The architecture explicitly avoids:

- plugin systems,
- dynamic module loading,
- configuration hot-reload,
- distributed execution,
- message buses,
- service meshes.

Such mechanisms introduce complexity and implicit control channels incompatible with experimental requirements.

---

## 2. Component Interfaces and Responsibilities

### 2.1 Design Language and Constraints

The RAIG Testbed is implemented in **C++20**.

This choice enforces:

- explicit memory ownership and lifetime control,
- absence of garbage collection,
- deterministic construction and destruction,
- transparent runtime behavior,
- strict control over floating-point semantics.

All components must follow RAII principles and avoid hidden side effects during initialization and destruction.

---

### 2.2 Genesis Core

The **Genesis Core** is the only component that owns and evolves the internal structural state.

Responsibilities:
- maintain the structural state X(t),
- apply the internal evolution operator,
- accept admissible external stimuli,
- emit reduced diagnostic projections.

Constraints:
- no public access to the full structural state,
- no external mutation of internal variables,
- no semantic interpretation of state values,
- no runtime reconfiguration.

Conceptual Interface (C++20):

    class GenesisCore {
    public:
        explicit GenesisCore(const InitialConditions& init);
        void step(const WorldStimulus& stimulus);
        DiagnosticSnapshot diagnostics() const;
    private:
        StructuralState X;
    };

The StructuralState type is opaque outside the Genesis Core implementation unit.

---

### 2.3 World Interface

The **World Interface** generates external stimuli independent of the system state.

Responsibilities:
- produce admissible stimuli,
- evolve independently of Genesis Core,
- introduce variation without guidance.

Constraints:
- no access to Genesis Core internals,
- no access to telemetry,
- no adaptive behavior.

Conceptual Interface (C++20):

    class WorldInterface {
    public:
        explicit WorldInterface(const WorldConfig& config);
        WorldStimulus next();
    private:
        WorldState state;
    };

The world state must not encode semantic intent.

---

### 2.4 Telemetry Emitter

The **Telemetry Emitter** records diagnostic projections.

Responsibilities:
- receive diagnostic snapshots,
- serialize telemetry data,
- persist telemetry externally.

Constraints:
- no feedback channel to Genesis Core,
- no execution blocking,
- no semantic enrichment.

Conceptual Interface (C++20):

    class TelemetryEmitter {
    public:
        explicit TelemetryEmitter(const TelemetryConfig& config);
        void emit(const DiagnosticSnapshot& snapshot);
    };

Telemetry emission must be observational only.

---

### 2.5 Runtime Supervisor

The **Runtime Supervisor** manages process execution.

Responsibilities:
- initialize components,
- run the main execution loop,
- handle fatal errors and termination,
- interface with the hosting environment.

Constraints:
- no adaptive control logic,
- no telemetry interpretation,
- no modification of evolution behavior.

---

### 2.6 Execution Ownership and Memory Model

All components use:

- deterministic stack or RAII-managed heap allocation,
- explicit ownership semantics,
- no shared mutable global state.

Dynamic allocation is permitted only if bounded and auditable.

---

### 2.7 Data Flow Topology

The system enforces a strict one-way data flow:

    World Interface → Genesis Core → Telemetry Emitter → External Storage

No reverse or lateral communication paths are permitted.

---

### 2.8 Interface Stability

All interfaces defined in this section are **structurally stable**.

Any change to:
- method signatures,
- ownership rules,
- component boundaries

requires a new Design document version and invalidates dependent Implementation plans.

---

## 3. Execution Loop and Time Management

### 3.1 Execution Loop Purpose

The execution loop is the sole driver of structural evolution in the RAIG Testbed.

Its purpose is to:

- advance the internal structural state,
- expose the system to external variation,
- emit diagnostic telemetry,
- sustain continuous operation over long durations.

The loop is invariant across the lifetime of an instance.

---

### 3.2 Loop Invariants

The execution loop must satisfy the following invariants:

- strictly monotonic progression,
- identical structure at every iteration,
- no conditional branching based on outcomes,
- no adaptive behavior.

Any deviation from these invariants invalidates the experiment.

---

### 3.3 Conceptual Loop Structure

The conceptual structure of the execution loop is:

    while (true) {
        WorldStimulus stimulus = world.next();
        core.step(stimulus);
        DiagnosticSnapshot snapshot = core.diagnostics();
        telemetry.emit(snapshot);
        pace_execution();
    }

This structure must not be altered dynamically.

---

### 3.4 Separation of Concerns Within the Loop

Each stage of the loop has a single responsibility:

- world.next(): generates external variation,
- core.step(): advances the structural state,
- core.diagnostics(): produces diagnostic projection,
- telemetry.emit(): records observation,
- pace_execution(): regulates wall-clock pacing only.

No stage may assume the semantics or internal behavior of any other stage.

---

### 3.5 Internal Time Semantics

Internal time exists only as a consequence of structural evolution.

Internal time:

- advances once per execution step,
- has no absolute origin,
- has no defined unit outside the system.

Internal time ceases to exist when structural evolution terminates.

---

### 3.6 Wall-Clock Time Usage

Wall-clock time may be used exclusively for pacing execution.

Wall-clock mechanisms:

- must not influence structural evolution,
- must not accumulate corrective feedback,
- must not cause time dilation or compression based on state.

The system must behave identically under different wall-clock pacing conditions.

---

### 3.7 Determinism and Repeatability

Given:

- identical initial conditions,
- identical external stimulus sequence,

the execution loop must produce identical telemetry sequences.

All nondeterminism must originate exclusively from the external world generator.

---

### 3.8 Absence of Temporal Control Logic

The execution loop must not:

- slow down due to instability,
- speed up due to stagnation,
- pause for inspection or logging,
- modify frequency based on telemetry.

Temporal neutrality is mandatory.

---

### 3.9 Termination Conditions

The execution loop terminates only if:

- structural collapse is detected by the core,
- an unrecoverable runtime failure occurs,
- the hosting environment forcibly terminates the process.

Termination is final and irreversible for the instance.

---
## 4. World Generator Design (Stimulus Modes)

### 4.1 Role of the World Generator

The World Generator is responsible for producing external stimuli that introduce variation into the RAIG Testbed without encoding intent, goals, or feedback.

Its function is to prevent trivial isolation while avoiding any form of guidance or adaptation.

The World Generator is not an environment, teacher, or controller.  
It is a source of structured uncertainty.

---

### 4.2 World Generator Constraints

The World Generator must satisfy the following constraints:

- no access to the internal structural state,
- no access to telemetry or diagnostics,
- no adaptation based on system behavior,
- no semantic interpretation of outcomes.

All stimulus generation logic is independent of the Genesis Core.

---

### 4.3 Stimulus Abstraction

A stimulus is an abstract, low-level perturbation applied to the system through the World Interface.

A stimulus may influence:

- the magnitude of deviation,
- the direction of structural change,
- the rate of structural evolution.

A stimulus must not encode symbols, instructions, actions, or objectives.

---

### 4.4 Stimulus State Independence

Stimulus generation must be state-independent with respect to the Genesis Core.

The World Generator may maintain its own internal state, but this state:

- evolves independently,
- does not correlate with the system’s internal history,
- does not respond to observed behavior.

World state evolution is permitted only as an internal property of the generator.

---

### 4.5 Stimulus Modes

The World Generator may operate in one or more predefined stimulus modes.

Typical admissible modes include:

- Background Noise Mode  
  Continuous low-amplitude stochastic variation.

- Regime Mode  
  Temporally extended periods with stable statistical properties.

- Impulse Mode  
  Rare, high-amplitude perturbations introduced without warning.

- Drift Mode  
  Slow, continuous changes in stimulus characteristics over time.

Stimulus modes must be selected at initialization and must not change adaptively during runtime.

---

### 4.6 Mode Transitions

Transitions between stimulus modes, if present, must be:

- predefined,
- time-driven or internally scheduled,
- independent of system behavior.

Mode transitions must not be triggered by telemetry or internal state indicators.

---

### 4.7 Randomness and Seeding

Randomness in stimulus generation is permitted only within the World Generator.

Random sources must be:

- explicitly seeded at initialization,
- isolated from the Genesis Core,
- reproducible given the same seed.

No random number generation is permitted inside the Genesis Core.

---

### 4.8 Failure of the World Generator

If the World Generator fails or degrades:

- stimulus generation may stop or degenerate,
- the Genesis Core continues evolving under reduced or null external influence.

Such conditions are considered valid experimental scenarios unless they introduce unintended feedback or control.

---

## 5. Core Evolution Encapsulation (State Opacity Strategy)

### 5.1 Encapsulation Objective

The Genesis Core must enforce **complete opacity** of the internal structural state.

Encapsulation is not a convenience feature; it is a **foundational experimental requirement** ensuring that no external component can infer, reconstruct, or influence internal structure.

---

### 5.2 Structural State Ownership

The internal structural state X(t):

- is owned exclusively by the Genesis Core,
- exists only within the core’s memory domain,
- is never shared by reference or pointer.

No other component may hold a reference, view, or alias to any part of the structural state.

---

### 5.3 Translation Unit Isolation

The concrete definition of the structural state is confined to a single translation unit.

This implies:

- forward declarations only in public headers,
- full definition hidden in implementation files,
- no inline accessors exposing internal layout.

This prevents accidental coupling and enforces compilation-level opacity.

---

### 5.4 Controlled State Interaction

The only admissible interaction with the structural state occurs through:

- application of an external stimulus via the evolution operator,
- generation of reduced diagnostic projections.

No other read or write paths exist.

All state transitions must pass through the same fixed evolution logic.

---

### 5.5 Evolution Operator Isolation

The internal evolution operator:

- is deterministic,
- is pure with respect to external state,
- operates only on the current structural state and current stimulus.

It must not:

- access global state,
- access telemetry,
- modify execution flow,
- depend on wall-clock time.

The operator embodies the entire internal dynamics of the system.

---

### 5.6 Diagnostic Projection Barrier

Diagnostic projections are produced through a one-way transformation.

This transformation:

- reduces dimensionality,
- removes internal topology,
- discards historical detail.

The projection must be mathematically irreversible.

Given diagnostics alone, reconstruction of the internal state must be impossible in principle.

---

### 5.7 Memory Non-Extractability

The memory component of the structural state:

- cannot be serialized,
- cannot be snapshot,
- cannot be externally queried,
- cannot be reset or modified directly.

Memory exists only as an accumulated constraint on future evolution.

---

### 5.8 Absence of Internal Observers

The Genesis Core contains no internal observers, monitors, or meta-processes.

There is no internal reflection, introspection, or self-evaluation.

The core does not “know” that it is being observed.

---

### 5.9 Auditability of Encapsulation

Encapsulation guarantees must be auditable by:

- inspection of public headers,
- inspection of translation unit boundaries,
- static analysis of access paths.

Any additional access path to the structural state invalidates the design.

---

## 6. Telemetry Design (Signals, Formats, Rotation)

### 6.1 Purpose of Telemetry

Telemetry in the RAIG Testbed exists solely to support:

- experimental traceability,
- invariant verification,
- post-mortem analysis,
- theoretical validation.

Telemetry is not a control signal and must never influence system behavior.

---

### 6.2 Telemetry Minimalism Principle

Telemetry must be **minimal, coarse-grained, and lossy**.

The design goal is to expose only what is necessary to:

- confirm that the system remains within admissible bounds,
- detect regime changes or collapse,
- reconstruct the high-level lifecycle of an instance.

Telemetry must never expose internal structure, topology, or memory content.

---

### 6.3 Admissible Telemetry Signals

Admissible telemetry signals are limited to reduced indicators derived from the internal structural state.

Typical signals include:

- aggregate deviation magnitude,
- aggregate structural energy intensity,
- monotonic memory indicators,
- contractivity and viability indicators,
- first-order temporal derivatives of the above.

All telemetry signals must be scalar or low-dimensional and must not encode internal relationships.

---

### 6.4 Prohibited Telemetry Content

Telemetry must not include:

- internal state vectors or components,
- internal transition paths,
- memory topology or structure,
- intermediate computation results,
- decision traces or action histories.

Any telemetry enabling partial reconstruction of the internal state is invalid.

---

### 6.5 Telemetry Emission Semantics

Telemetry emission occurs once per execution step.

Emission semantics:

- telemetry is emitted after state evolution,
- telemetry reflects the current structural condition,
- telemetry does not affect execution timing or order.

Emission failures must not alter core execution logic.

---

### 6.6 Serialization Format

Telemetry must be serialized in a simple, append-only format.

Permissible formats include:

- line-oriented JSON (JSONL),
- CSV with fixed schema,
- binary append-only records with documented layout.

The format must be:

- stable across versions,
- explicitly versioned,
- readable without specialized tooling.

---

### 6.7 Log Rotation and Persistence

Telemetry persistence must support long-lived execution.

Log rotation:

- is time- or size-based,
- does not truncate historical data,
- does not overwrite existing records.

Persistent storage is used only for telemetry and must never contain internal state.

---

### 6.8 Isolation of Telemetry Storage

Telemetry storage is strictly isolated from the execution process.

Storage mechanisms must not:

- introduce feedback into the system,
- enable state restoration,
- provide control hooks.

Storage failure does not terminate the instance by definition.

---

### 6.9 Telemetry Integrity

Each telemetry record must include:

- a monotonic step counter or timestamp,
- a schema version identifier,
- optional build or instance identifiers.

Telemetry integrity ensures that traces can be analyzed consistently even after process termination.

---

## 7. Deployment Design (Railway / Container Semantics)

### 7.1 Deployment Objective

The deployment design must preserve the experimental assumptions of the RAIG Testbed under real infrastructure constraints.

Deployment is not treated as an optimization or scaling problem.  
It is treated as an **extension of the experimental environment**.

The primary objective is to ensure:

- single-instance execution,
- uninterrupted lifetime,
- absence of hidden lifecycle control.

---

### 7.2 Single-Instance Enforcement

Deployment must enforce exactly one running instance per experiment.

This implies:

- one container or process,
- no replicas,
- no horizontal scaling,
- no rolling updates.

Any infrastructure feature that implies redundancy or replication must be disabled.

---

### 7.3 Containerization Model

The RAIG Testbed is deployed as a single containerized service.

The container:

- contains exactly one executable,
- exposes no inbound control endpoints,
- runs the Genesis instance as PID 1,
- terminates when the process exits.

The container image must be immutable once built.

---

### 7.4 Railway-Specific Semantics

When deployed on Railway or similar platforms:

- the service must be configured with a single replica,
- automatic restarts on crash must be disabled where possible,
- health checks must not trigger restarts,
- redeployments must be manual and explicit.

Any platform-induced restart is considered an **external termination event** equivalent to instance death.

---

### 7.5 Persistent Storage Usage

Persistent storage, if used, is permitted **only** for telemetry logs.

Storage constraints:

- append-only access,
- no read-back into the running process,
- no state restoration on restart.

No part of the internal structural state may ever be written to persistent storage.

---

### 7.6 Configuration Injection

Configuration parameters may be injected at process start via:

- command-line arguments,
- environment variables,
- static configuration files baked into the image.

Configuration must be read exactly once at startup.

No runtime configuration reload is permitted.

---

### 7.7 Networking Constraints

The deployed service:

- does not expose public APIs,
- does not accept inbound network connections,
- does not communicate with external services except for telemetry export.

Outbound telemetry connections must be one-way and non-interactive.

---

### 7.8 Resource Management

Resource limits (CPU, memory) may be imposed by the hosting platform.

The system must:

- operate correctly under fixed resource bounds,
- not request dynamic resource scaling,
- treat resource exhaustion as a valid termination scenario.

---

### 7.9 Operational Intervention

Human or automated intervention is limited to:

- starting the instance,
- observing telemetry externally,
- terminating the instance.

No intervention may alter internal behavior during runtime.

---

## 8. Failure Handling and Death Recording

### 8.1 Failure Handling Philosophy

Failure handling in the RAIG Testbed is **non-recoverative**.

The system does not attempt to correct, compensate, or recover from failures.  
Any failure that compromises structural integrity or runtime correctness is treated as **terminal** for the current instance.

Failure handling exists to:

- preserve experimental validity,
- prevent silent corruption,
- ensure clear instance termination semantics.

---

### 8.2 Classification of Failures

Failures are classified into three categories:

- **Structural Failures**  
  Loss of viability due to internal dynamics (e.g., collapse of contractivity).

- **Runtime Failures**  
  Violations of execution correctness (e.g., memory corruption, numerical instability).

- **Infrastructure Failures**  
  External termination caused by the hosting environment (e.g., container kill, node shutdown).

All categories result in instance termination.

---

### 8.3 Structural Failure Detection

Structural failure is detected exclusively by the Genesis Core.

Indicators may include:

- contractivity reaching a non-viable threshold,
- unbounded structural energy growth,
- irreversible loss of coherence.

Structural failure detection must not rely on telemetry or external observation.

---

### 8.4 Runtime Failure Detection

Runtime failures include, but are not limited to:

- detected undefined behavior,
- violated invariants,
- arithmetic exceptions,
- internal consistency checks failing.

Upon detection of a runtime failure, the process must terminate immediately.

No attempt is made to continue execution in a degraded mode.

---

### 8.5 Infrastructure Failure Semantics

Infrastructure failures are external to the system.

Examples include:

- forced process termination,
- container eviction,
- hardware or virtual machine failure.

Such failures are treated as **external death events** and do not imply internal structural collapse.

---

### 8.6 Death Recording

Upon termination, the system must record a **death record**.

A death record includes:

- termination timestamp,
- termination classification (structural, runtime, infrastructure),
- last successfully emitted telemetry step,
- build and instance identifiers.

Death recording must not modify internal structural state.

---

### 8.7 Order of Operations at Death

The order of operations upon death is:

1. detect failure condition,
2. emit final death record,
3. terminate the process.

No additional cleanup or finalization logic is permitted beyond telemetry integrity.

---

### 8.8 Absence of Post-Death Continuation

After death:

- no state continuation exists,
- no restart is permitted,
- no recovery path is defined.

Any subsequent execution constitutes a **new instance**, not a continuation.

---

### 8.9 Auditability of Failure Handling

Failure handling behavior must be auditable through:

- inspection of termination paths,
- analysis of emitted death records,
- verification of immediate process exit.

Any hidden recovery or retry logic invalidates the design.

---

## 9. Build & Toolchain Design (C++20)

### 9.1 Build Objectives

The build system must ensure:

- strict reproducibility,
- deterministic binaries,
- full auditability of compilation options,
- alignment with experimental assumptions.

Build convenience or developer ergonomics must not override correctness or transparency.

---

### 9.2 Language Standard

The RAIG Testbed is built using **C++20**.

Later language standards (e.g., C++23) are not permitted unless explicitly approved through a design revision and compliance update.

All source code must compile cleanly under the selected standard without extensions.

---

### 9.3 Compiler Selection

Supported compilers include:

- Clang (preferred),
- GCC (acceptable alternative).

A specific compiler version must be pinned for each experiment and recorded in telemetry metadata.

Mixed compiler usage within a single build is not permitted.

---

### 9.4 Compilation Flags Policy

Compilation flags must prioritize correctness and determinism.

Required properties include:

- strict IEEE-754 floating-point behavior,
- disabled fast-math optimizations,
- disabled undefined-behavior assumptions,
- explicit warning levels treated as errors.

Optimization levels may be enabled only if they do not alter numerical semantics.

---

### 9.5 Deterministic Build Configuration

Builds must be deterministic given identical:

- source code,
- compiler version,
- compilation flags,
- dependency versions.

Build artifacts must not embed timestamps, hostnames, or nondeterministic identifiers.

---

### 9.6 Dependency Management

External dependencies must be:

- minimal,
- version-pinned,
- explicitly documented.

Header-only or vendored dependencies are preferred.

Dynamic linking to uncontrolled system libraries is discouraged.

---

### 9.7 Build System

The build system must:

- define explicit targets,
- separate build configuration from source code,
- support reproducible builds in containerized environments.

CMake or an equivalent declarative build system is recommended.

---

### 9.8 Sanitizers and Debug Builds

Separate build configurations must exist for:

- production execution,
- static analysis,
- sanitizer-enabled verification.

Sanitizer builds must not be used for long-lived experimental runs unless explicitly required.

---

### 9.9 Build Metadata Emission

Each build must produce metadata including:

- compiler name and version,
- language standard,
- compilation flags,
- dependency versions,
- source revision identifier.

This metadata must be included in telemetry headers and death records.

---

### 9.10 Prohibited Build Features

The following are prohibited:

- runtime code generation,
- just-in-time compilation,
- dynamic loading of code modules,
- self-modifying binaries.

The executable must remain static in behavior for the entire lifetime of an instance.

---

## 10. Security & Isolation

### 10.1 Security Objective

Security in the RAIG Testbed is defined as **preservation of experimental integrity**, not protection against external attackers.

Isolation mechanisms exist to ensure that:

- no external entity can influence internal evolution,
- no internal state can leak beyond admissible telemetry,
- no hidden control or feedback channels exist.

---

### 10.2 Process Isolation

The RAIG Testbed runs as a single isolated process.

Process isolation requirements:

- no shared memory with other processes,
- no IPC mechanisms beyond telemetry output,
- no dynamically attached debuggers or profilers during runtime.

Any attachment that alters execution timing or memory layout invalidates the experiment.

---

### 10.3 Network Isolation

The system must not accept inbound network connections.

Network constraints:

- no listening sockets,
- no RPC interfaces,
- no command channels.

Outbound network usage is permitted only for telemetry export and must be strictly one-way.

---

### 10.4 File System Isolation

File system access is restricted to:

- executable code,
- configuration files read at startup,
- append-only telemetry logs.

The system must not:

- read historical telemetry during runtime,
- modify executable files,
- access unrelated file system locations.

File system isolation ensures irreversibility of execution.

---

### 10.5 Configuration Immutability

All configuration parameters are immutable after process start.

The system must not support:

- live configuration reload,
- environment variable re-reading,
- dynamic parameter injection.

Any attempt to alter configuration during runtime invalidates the experiment.

---

### 10.6 Privilege Model

The RAIG Testbed must run with minimal privileges.

Recommendations include:

- non-root execution,
- restricted file system permissions,
- limited network capabilities.

The system must not require elevated privileges to function.

---

### 10.7 Dependency Isolation

All dependencies must be explicitly declared and version-pinned.

The runtime environment must not:

- auto-update libraries,
- dynamically fetch dependencies,
- alter dependency versions during execution.

Dependency isolation ensures repeatability and auditability.

---

### 10.8 External Observation Constraints

External observation is limited to telemetry consumption.

Observers must not:

- attach debuggers,
- inspect process memory,
- modify execution parameters.

Any form of intrusive observation invalidates the instance.

---

### 10.9 Auditability of Isolation

Isolation guarantees must be verifiable through:

- container configuration inspection,
- process capability analysis,
- network policy inspection,
- file system access auditing.

Hidden or implicit communication channels invalidate the design.

---

### 10.10 Security as Experimental Integrity

Security is treated as an extension of experimental methodology.

Any breach of isolation is not a “security incident” but an **experimental violation**, rendering results non-interpretable.