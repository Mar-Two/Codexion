*This project has been created as part of the 42 curriculum by mben-mer.*

# Codexion

## Description

Codexion simulates a group of coders sharing a limited number of USB dongles in a circular workspace. Each coder cycles through compiling, debugging and refactoring. Compiling requires two dongles simultaneously — the one on their left and the one on their right — each shared with a neighbour.

The simulation ends when every coder has compiled a required number of times, or when one of them burns out for not having compiled within the allowed delay.

This is a variant of the dining philosophers problem. It covers thread creation, mutual exclusion, condition variables, deadlock and starvation prevention, and two scheduling policies (FIFO and EDF) backed by a hand-written binary heap.

Multithreading allows several execution flows to run concurrently within a single process, sharing the same memory space. That shared memory is what makes synchronisation necessary: without it, concurrent access to the dongles would leave them in an inconsistent state.

## Instructions

```
make
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All times are in milliseconds. `scheduler` must be either `fifo` or `edf`.

Example:

```
./codexion 5 800 200 200 100 5 50 edf
```

The Makefile provides `all`, `clean`, `fclean` and `re`.

## Blocking cases handled

**Deadlock.** Both dongles are acquired atomically — either both or neither — which breaks Coffman's *hold and wait* condition. A coder never holds one dongle while waiting for the other, so a circular wait cannot form. A single mutex protects the whole allocation state, which also removes any risk of lock-ordering inversion.

**Starvation.** Each dongle owns a priority queue (binary heap). A coder registers in both queues and can only acquire when it is at the head of both. Under FIFO the key is the request timestamp, so waiting time is bounded: every service moves each waiting coder one rank forward. Under EDF the key is the burnout deadline; a coder that is not served sees its deadline approach, so its priority rises until it is served.

**Tie-breaking.** Equal keys are resolved by coder id, which makes both policies fully deterministic.

**Cooldown.** A released dongle carries a date before which it cannot be taken again. This date is part of the acquisition predicate. Since elapsed time is not signalled by any thread, a coder blocked only by a cooldown waits with `pthread_cond_timedwait` on that deadline.

**Burnout detection.** A dedicated monitor thread samples every millisecond, comparing each coder's `last_compile_start` against its deadline under that coder's mutex. Detection is well within the 10 ms tolerance.

**Log serialisation.** All output goes through a dedicated mutex, so two messages never interleave on a single line.

## Thread synchronization mechanisms

**Mutexes.** Three families of mutex are used, each protecting a named set of variables:

- `dongle_mutex` — the whole allocation state: `taken_by`, `available_at` and the waiting queue of every dongle, plus the stop flag.
- `coder_mutex` (one per coder) — `nb_compiles` and `last_compile_start`, which the coder writes and the monitor reads.
- `print_mutex` — standard output.

When a thread holds two of them, the order is always `dongle_mutex` before `print_mutex`, which rules out lock-ordering inversion.

**Condition variable.** A single condition variable, associated with `dongle_mutex`, puts waiting coders to sleep instead of busy-waiting. One is enough because all the data appearing in the acquisition predicate lives under that same mutex.

**Waiting.** A coder registers in both of its dongle queues, then loops: while its predicate is false and the simulation is running, it sleeps on the condition variable. The predicate requires being at the head of both queues, both dongles free, and both cooldowns elapsed. The re-check must happen in a `while` loop, since a wake-up guarantees nothing — POSIX allows spurious wake-ups, and a broadcast wakes every sleeper while only one can proceed.

When a coder releases its dongles, it updates the state and issues a `pthread_cond_broadcast`. Every sleeper wakes, re-evaluates its own predicate under the mutex, and either proceeds or goes back to sleep. A broadcast is used rather than a signal because the kernel picks an arbitrary sleeper, which would not respect the scheduling policy.

**Stopping.** The monitor sets a shared `stop` flag under `dongle_mutex` and broadcasts. The flag is part of every waiting predicate and is checked between each phase of the coder lifecycle, so no coder can start a new action or print a line once the simulation has ended.

### Race condition prevention — examples

**Check-then-act on dongle acquisition.** Testing whether both dongles are free and marking them as taken must be indivisible. If the test happened outside the critical section, two coders could both read `taken_by == -1` before either had written, and both would proceed. Registration, predicate evaluation, queue extraction and the `taken_by` update all happen under a single hold of `dongle_mutex`.

**Coder-to-monitor communication.** The monitor reads `nb_compiles` and `last_compile_start` while the owning coder may be writing them. On a 64-bit field the write is not guaranteed atomic, so an unprotected read could return a value that never existed — producing a phantom burnout, or missing a real one. Both threads therefore go through that coder's `coder_mutex`.

**Monitor-to-coder communication.** The `stop` flag is written by the monitor and read by every coder, always under `dongle_mutex`. Since the flag is part of the waiting predicate and the mutex is held across both the flag check and the print, no coder can emit a line after the burnout message.

These points were verified with Helgrind, which reports no data race and no lock-ordering violation.

## Technical choices

**A single mutex over the allocation state.** The subject asks that each dongle's state be protected by a mutex. A per-dongle mutex would make registration in both queues non-atomic: `pthread_cond_wait` releases only one mutex, so a coder falling asleep while holding two would block a neighbour indefinitely. Releasing one before sleeping reopens the window where a coder is registered in one queue but not the other — which allows a livelock where two coders are each at the head of one queue and neither can proceed. The single mutex guarantees that the coder with the smallest key is always at the head of both its queues. The trade-off is reduced concurrency during acquisition, which is acceptable since critical sections are short and all actual work happens outside them.

**FIFO keys in microseconds.** With millisecond resolution, all coders registering at start-up share the same key, so the tie-breaker becomes the only criterion and degenerates the policy into a strict id ordering. Microsecond keys make registrations distinguishable and restore concurrency between non-adjacent coders.

**Policy-agnostic heap.** The heap only compares keys. FIFO and EDF differ solely in how the key is computed at registration time, so a single implementation serves both.

**Fixed queue capacity.** At most two coders can ever wait on a given dongle, so each queue is allocated once with no resizing.

## Known limitation

With `number_of_compiles_required = 0` and all durations set to zero, coders complete several cycles before the monitor's first sampling tick, so a few lines are printed before the simulation stops. The stop condition is correct; only the sampling granularity makes it visible.

## Resources

- POSIX man pages: `pthread_create`, `pthread_mutex_lock`, `pthread_cond_wait`, `pthread_cond_timedwait`, `gettimeofday` — the only normative source on spurious wake-ups and the exact semantics of each primitive.
- Dijkstra's dining philosophers problem, of which this project is a variant.
- Coffman's four necessary conditions for deadlock — the formal name for the reasoning used in the *Blocking cases handled* section above.

## AI usage

Claude (Anthropic) was used in the following ways:

- **Understanding POSIX concurrency semantics**: what mutexes and condition variables actually guarantee, why the predicate must be re-checked in a `while` loop, and why elapsed time cannot be signalled (hence `pthread_cond_timedwait` for the cooldown).
- **Design validation**: the scheduler architecture was discussed before implementation — in particular the decision to use a single mutex over the whole allocation state rather than one per dongle, and to keep the heap policy-agnostic by encoding FIFO/EDF in the key.
- **Debugging**: interpreting Helgrind output, locating a lock-ordering deadlock, and diagnosing a mismatch between absolute and relative timestamps.
- **README writing**.

All code was written by me. Design decisions were mine; AI was used to validate reasoning and surface consequences.
