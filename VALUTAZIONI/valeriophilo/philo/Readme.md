*This project has been created as part of the 42 curriculum by valucare.*

# Philosophers

## Description

**Philosophers** is a 42 School project that tackles the classic **Dining
Philosophers Problem**, a well-known illustration of the challenges of
concurrent programming (deadlocks, race conditions, resource starvation).

A number of philosophers sit at a round table with a fork between each pair
of them. Each philosopher repeatedly alternates between three states:
**eating**, **thinking**, and **sleeping**. To eat, a philosopher must pick
up both the fork on their left and the fork on their right. If a philosopher
does not manage to start eating before `time_to_die` milliseconds have
passed since their last meal (or since the start of the simulation), they
die of starvation.

The goal of the project is to simulate this scenario correctly:
- No philosopher should die unless it is truly unavoidable given the
  parameters.
- No data races or deadlocks should occur.
- State-change events must be logged with precise timestamps.

The **mandatory part** (`philo`) implements the simulation using **threads**
and **mutexes**. The **bonus part** (`philo_bonus`) implements it using
**processes** and **semaphores**.

## Instructions

### Compilation

```bash
make          # builds the mandatory "philo" program
make clean    # removes object files
make fclean   # removes object files and binaries
make re       # rebuilds everything from scratch
```

### Usage

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

- `number_of_philosophers`: number of philosophers (and forks) at the table.
- `time_to_die` (ms): time after which a philosopher dies if they haven't
  started eating.
- `time_to_eat` (ms): time it takes for a philosopher to eat.
- `time_to_sleep` (ms): time a philosopher spends sleeping.
- `number_of_times_each_philosopher_must_eat` (optional): if provided, the
  simulation stops once every philosopher has eaten this many times.

**Example:**

```bash
./philo 5 800 200 200
```
## Resources

- [The Dining Philosophers Problem – Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- POSIX Threads Programming (`man pthread_create`, `man pthread_mutex_init`)
- POSIX Semaphores (`man sem_open`, `man sem_wait`)
- `man gettimeofday`, `man usleep`

### AI usage

AI assistance was used to help structure this README according to the
project's requirements. No AI-generated code was copy-pasted directly into
the source files; all logic was written and understood independently, in
line with the project's AI usage guidelines.
