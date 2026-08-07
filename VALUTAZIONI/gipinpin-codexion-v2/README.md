*This project has been created as part of the 42 curriculum by gipimpin.*

## Description
Codexion is a multi-threaded simulation written in C that models a collaborative coding environment. In this scenario, multiple coders sit in a circular hub around a shared Quantum Compiler. To compile their code, a coder must acquire two USB dongles (one on their left, one on their right). Dongles are shared resources with a strict cooldown period between uses. The simulation challenges coders to alternate between compiling, debugging, and refactoring without ever starving for resources, which would lead to a "burnout".

The core challenge of this project is to manage concurrency, prevent deadlocks, and implement fair resource arbitration using strict scheduling algorithms (FIFO and Earliest Deadline First - EDF), all while ensuring precise burnout detection within a 10ms tolerance.

## Instructions
### Compilation
The project includes a `Makefile` that compiles the source files with the required flags (`-Wall -Wextra -Werror -pthread`).
To compile the project, run:
```bash
make
```
Other available rules:
- `make clean`: Removes object files.
- `make fclean`: Removes object files and the executable.
- `make re`: Recompiles the entire project cleanly.

### Execution
Run the executable with the following mandatory arguments:
```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```
- **number_of_coders**: Number of coders and dongles.
- **time_to_burnout**: Time (in ms) a coder has to start compiling before burning out.
- **time_to_compile**: Time (in ms) spent compiling (requires two dongles).
- **time_to_debug**: Time (in ms) spent debugging.
- **time_to_refactor**: Time (in ms) spent refactoring.
- **number_of_compiles_required**: Simulation stops if all coders reach this number of compiles.
- **dongle_cooldown**: Time (in ms) a dongle is unavailable after being released.
- **scheduler**: Arbitration policy for dongles: `fifo` or `edf`.

**Example:**
```bash
./codexion 5 800 200 200 200 7 10 fifo
```

## Thread Synchronization Mechanisms
This project relies heavily on POSIX threading primitives to ensure thread safety and coordinate access to shared resources:

- **Mutexes (`pthread_mutex_t`)**: 
  - Each dongle is protected by its own mutex to ensure only one coder can acquire it or wait for it at a time.
  - A dedicated `print_mutex` serializes standard output to ensure log messages are never interleaved.
  - A `monitor_mutex` protects global simulation states, like the `sim_stop` flag and the `fifo_counter`.
  - Each coder has a `coder_mutex` to protect their individual state (`last_compile_time`, `is_compiling`, `compiles_done`), preventing the monitor thread from reading inconsistent data while a coder updates it.

- **Condition Variables (`pthread_cond_t`)**:
  - Each dongle uses a condition variable to implement waiting queues efficiently. Instead of busy-waiting (which wastes CPU cycles), coders wait on the condition variable (`pthread_cond_wait`) if the dongle is currently in use, in cooldown, or if it's not their turn according to the priority queue.
  - When a dongle is released, `pthread_cond_broadcast` wakes up waiting threads to re-evaluate their condition.

Race conditions are systematically prevented by ensuring that any shared data (like timestamps or counters) is read or written exclusively while holding the corresponding mutex.

## Blocking Cases Handled
The simulation addresses several critical concurrency challenges:

- **Deadlock Prevention (Coffman's Conditions)**: If all coders were to grab their left dongle simultaneously, they would all wait indefinitely for the right dongle (a circular wait). This is prevented by having the last coder acquire their right dongle first, breaking the symmetry and preventing the deadlock.
- **Starvation Prevention**: The project implements a custom Min-Heap priority queue (`t_pq`) for each dongle. When multiple coders compete for the same dongle, access is granted strictly based on the chosen scheduling policy (FIFO based on arrival order, or EDF based on the closest burnout deadline). This guarantees liveness.
- **Cooldown Handling**: When a dongle is released, its `last_used_time` is recorded, and an `in_cooldown` flag is set. The monitor thread polls the dongles and broadcasts the condition variable once the cooldown period expires, allowing the next coder in the queue to acquire it.
- **Precise Burnout Detection**: A dedicated monitor thread constantly checks the state of all coders. To avoid false positives (where a coder takes a dongle but gets marked as burnt out before updating their timestamp), an `is_compiling` flag is set during the compilation phase, signaling the monitor to skip the burnout check for that coder.
- **Log Serialization**: All calls to `log_status` lock the `print_mutex` and immediately check if the simulation has stopped before printing. This ensures that no further actions are logged after a burnout has occurred, keeping the output clean and compliant.

## Resources
- [POSIX Threads Programming](https://computing.llnl.gov/tutorials/pthreads/)
- [Operating Systems: Three Easy Pieces - Concurrency](https://pages.cs.wisc.edu/~remzi/OSTEP/#concurrency)
- **AI Usage**: Artificial Intelligence was utilized during this project as a pair-programming assistant to:
  - Analyze code for potential race conditions and deadlocks.
  - Refine the custom Min-Heap implementation used for the EDF/FIFO scheduler.
  - Suggest cleaner approaches for precise burnout detection (replacing momentary flags with continuous compilation state tracking).
  - Draft and structure documentation to comply with project requirements.
