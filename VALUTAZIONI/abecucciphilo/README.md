*This project has been created as part of the 42 curriculum by abecucci.*

# Philosophers

## Description
**philosophers** is a concurrency project written in C, based on the classic *Dining Philosophers* problem.  
The program simulates multiple philosophers sitting around a table, alternating between **thinking**, **eating**, and **sleeping**, while competing for shared resources (**forks**) protected by mutexes.

The goal of this project is to gain a solid understanding of:
- **threads** with `pthread_create()` / `pthread_join()`,
- **mutexes** for mutual exclusion and safe shared-state access,
- **timing** and accurate event scheduling (`time_to_die`, `time_to_eat`, `time_to_sleep`),
- **race conditions**, deadlocks, and how to avoid them,
- clean shutdown of a multithreaded simulation.

### Mandatory features (subject-driven)
- Create one thread per philosopher.
- Each philosopher repeatedly:
  - takes forks,
  - eats,
  - sleeps,
  - thinks.
- Forks are shared resources protected by mutexes.
- A philosopher **dies** if they don’t eat within `time_to_die`.
- Output must be synchronized to avoid interleaved prints.
- The program stops when:
  - a philosopher dies, or
  - (if provided) every philosopher has eaten at least `number_of_times_each_philosopher_must_eat`.

> Note: As with many timing-based concurrency projects, exact behavior depends on scheduling.
> The implementation should remain correct under typical Linux thread scheduling.

## Instructions

### Requirements
- Linux / Unix environment
- `make` installed
- a C compiler (e.g. `gcc`, `clang`)
- POSIX threads available (`pthread`)

### Build
From the project directory:

```
make
```

### Run

Program arguments:

```
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
``` 

Examples:

```
./philo 5 800 200 200
./philo 4 410 200 200 5
```

### Output format
Each event is printed as:

```
<timestamp_ms> <philo_id> <event>
``` 

Typical events:
- `has taken a fork`
- `is eating`
- `is sleeping`
- `is thinking`
- `died`

### Debug / data-race checks (optional)
You can use tools like Valgrind/Helgrind or ThreadSanitizer to help identify concurrency issues.

**Valgrind (basic leak check):**

```
valgrind --leak-check=full --show-leak-kinds=all ./philo 5 800 200 200
```

> Note: Timing-based projects can behave differently under Valgrind due to slowdown.
> If results look inconsistent, try with larger `time_to_die`.

### Clean
Remove object files:

```
make clean
``` 

Remove object files and binary:

```
make fclean
```

## Usage notes / implementation details
- Forks are modeled as an array of `pthread_mutex_t`, one per fork.
- Printing is guarded by a dedicated mutex to keep logs readable.
- Shared state (e.g., stop condition / last meal timestamps) must be protected to avoid data races.
- A start barrier mechanism can be used to start philosopher threads in a controlled way.

## Project structure
- `main.c` — argument parsing orchestration and execution flow
- `init_shared.c` — shared state initialization and parsing utilities
- `init_philos.c` — philosophers initialization (ids, fork indices, etc.)
- `routine.c` — philosopher routine (take forks / eat / sleep / think)
- `execute_eating.c` — thread creation, monitoring logic, joins
- `time.c` — time helpers (timestamps in ms)
- `cleanup.c` — resource cleanup (mutex destroy/free)

## Resources
Classic references:
- POSIX threads: `man pthread_create`, `man pthread_mutex_lock`, `man pthread_join`
- Timing: `man gettimeofday`, `man usleep`
- Dining Philosophers overview: https://en.wikipedia.org/wiki/Dining_philosophers_problem

### AI usage disclosure
AI tools were used **only as support** for:
- drafting and organizing this README
- code-completion assistance
- producing checklists based on the subject requirements
- reviewing documentation references
- reasoning about synchronization and edge cases
- testing the program under different conditions

All implementation choices and code behavior were validated by manual review and runtime testing.

No AI-generated code was copied blindly into the project.
