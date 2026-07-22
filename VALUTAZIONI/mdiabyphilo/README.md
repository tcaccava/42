*This project has been created as part of the 42 curriculum by mdiaby.*

# Dining Philosophers Problem

## Description

This project implements the classic Dining Philosophers problem in C, illustrating
resource sharing, deadlock avoidance and starvation prevention.

Philosophers sit around a circular table, alternating between eating, sleeping and
thinking. There is one fork between each pair of neighbours, and two forks are
needed to eat, so neighbours must compete for a shared resource. A philosopher who
does not eat within `time_to_die` milliseconds of their last meal dies and the
simulation stops.

### Features

* Argument parsing and validation
* One thread per philosopher
* Mutex-based fork management
* Deadlock avoidance through consistent lock ordering
* Starvation mitigation for odd numbers of philosophers
* Monitor thread watching philosopher states
* Accurate timing and clean cleanup of mutexes and memory

## Instructions

### Compilation

```
make
```

This produces the `philo` executable. Other rules: `clean`, `fclean`, `re`.

### Usage

```
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [meals_required]
```

All values are positive integers, times in milliseconds. The last argument is
optional: if given, the simulation ends once every philosopher has eaten that many
times.

```
./philo 5 800 200 200      # nobody should die
./philo 5 800 200 200 7    # ends after 7 meals each
./philo 1 800 200 200      # single philosopher, dies at 800 ms
```

## Resources

* `man pthread_create`, `pthread_mutex_init`, `pthread_mutex_lock`, `gettimeofday`

**Use of AI.** An AI assistant (Claude) was used to learn threads and mutexes
through guided questioning, to help diagnose a bug where satiated philosophers were
wrongly reported as dead, to run repeated executions measuring how often an
intermittent starvation bug occurred and compare two candidate fixes, to draw up
the list of edge cases to test, and to draft this README. The implementation was
written and understood by me.