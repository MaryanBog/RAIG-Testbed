# RAIG Testbed v1 — Reproducible Structural Dynamics Experiment

This repository contains **RAIG Testbed v1** — a minimal, deterministic experimental system
designed to study **structural dynamics, memory accumulation, viability loss, and endogenous termination**.

> ⚠️ This is **not** an intelligence, not an agent, and not an optimizer.  
> It is a **controlled experimental testbed** for investigating necessary preconditions
> for the emergence of intelligence.

---

## 1. What This Project Is

RAIG Testbed implements a closed-loop structural process with:

- irreversible internal memory (`mmag`)
- bounded viability (`kappa ∈ [0,1]`)
- deterministic world stimulation
- explicit life/death semantics
- zero external control or optimization

The system:
- initializes
- evolves deterministically
- accumulates structure
- **dies endogenously** (viability collapse)
- records a complete telemetry trace

The **absence of intelligence** is an intentional and validated result.

---

## 2. Build Environment (Windows 11)

This project is built **from Git Bash** using **Ninja** and **MSYS2 MinGW toolchain**.

### Required tools
- Git Bash
- MSYS2 installed in `C:\msys64`
- Packages:
  - `mingw-w64-x86_64-gcc`
  - `mingw-w64-x86_64-cmake`
  - `mingw-w64-x86_64-ninja`

---

## 3. One-Time Setup (per Git Bash session)

```bash
export PATH="/c/msys64/mingw64/bin:/c/msys64/usr/bin:$PATH"
Verify:

bash
Копировать код
which g++
which cmake
which ninja
They must resolve to /c/msys64/mingw64/bin/....

4. Build (Ninja)
From project root:

bash
Копировать код
rm -rf build
cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=g++
cmake --build build
Artifacts:

build/raig_testbed.exe

build/raig_tests.exe

5. Run the Experiment
bash
Копировать код
./build/raig_testbed.exe
Output:

telemetry_long.jsonl (append-only JSONL telemetry)

Each line contains:

json
Копировать код
{
  "step": <uint64>,
  "kappa": <double>,
  "phi": <double>,
  "dmag": <double>,
  "mmag": <double>,
  "alive": <bool>,
  "death_reason": <enum>
}
Death reasons:

1 — NonFinite

2 — PhiOverflow

3 — KappaZero

6. Run Tests (Mandatory)
bash
Копировать код
ctest --test-dir build -V
Or directly:

bash
Копировать код
./build/raig_tests.exe
All tests must pass before any result is considered valid.

7. What Is Tested
The test suite validates:

Determinism
Byte-for-byte identical telemetry for identical seeds

Invariants
mmag is monotonic

all values are finite

kappa ∈ [0,1] until death

Time correctness
step increments exactly by 1

Death semantics
explicit death snapshot

sticky death (no evolution after death)

World replay
identical seed → identical stimulus stream

different seed → divergence

Runtime invariance
pace_ms does not affect physics or telemetry

8. Scientific Status of This Result
This project constitutes a completed experimental cycle:

a formal system was specified

invariants were enforced

long-horizon runs were executed

results were recorded

negative result (no intelligence emergence) was confirmed

full reproducibility was demonstrated

This is a valid and publishable negative result.

9. What This Project Is NOT
❌ not an AI system

❌ not an agent

❌ not a learning algorithm

❌ not a proof of intelligence emergence

It is a baseline experimental platform.

10. Project Status
RAIG Testbed v1 — CLOSED

Further progress requires:

extended parameter sweeps

higher computational budget

controlled asymmetry experiments

This concludes Stage 1.

11. License / Usage
Research use only.
No guarantees of fitness for any purpose.

“We verified that intelligence does not arise accidentally.
Now we know where to begin looking honestly.”

yaml
Копировать код
