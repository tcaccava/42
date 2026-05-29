*This project has been created as part of the 42 curriculum by ilnassi.*

# Philosophers

## Description

Philosophers is a concurrency project focused on understanding synchronization, threads, and resource management in a multithreaded environment.

The goal of the project is to simulate the classic Dining Philosophers Problem using POSIX threads and mutexes. Each philosopher is represented by a thread, and each fork is protected by a mutex to ensure mutual exclusion.

The challenge is to:
- Prevent data races.
- Avoid deadlocks.
- Ensure proper synchronization between threads.
- Detect philosopher death based on timing constraints.
- Optionally stop the simulation when each philosopher has eaten a specified number of times.

The simulation follows strict timing rules:
- `time_to_die`
- `time_to_eat`
- `time_to_sleep`

A dedicated monitor thread continuously checks for death conditions and stops the simulation when required.

---

## 📚 Learning Goals

- Understand **threads** and **mutexes**
- Practice **synchronization** and **parallel programming**
- Learn how to debug timing issues and race conditions
- Write clean, organized and safe C code

---

## How It Works

- Each philosopher is implemented as a separate thread.
- Forks are implemented as mutexes.
- Philosophers alternate between:
  - Thinking
  - Taking forks
  - Eating
  - Sleeping
- A monitor thread checks:
  - If a philosopher has exceeded `time_to_die`
  - If all philosophers have eaten `number_of_times_each_philosopher_must_eat` (if specified)

To prevent deadlock, fork acquisition order is asymmetric:
- Even philosophers take the right fork first.
- Odd philosophers take the left fork first.

Small delays (`usleep`) are introduced to reduce contention and improve scheduling stability.

---

## Usage 

./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]

1.  ./philo 1 800 200 200 (The philosopher should not be able to eat)
2.  ./philo 5 800 200 200 (No philosopher should die)
3.  ./philo 5 800 200 200 7 (No philosopher should die. The simulation will stop when each philosopher has eaten 7 times.)
4.  ./philo 4 410 200 200
5.  ./philo 4 310 200 100

---
## Resources

Dining Philosophers Problem – Edsger W. Dijkstra
https://www.youtube.com/watch?v=FYUi-u7UWgw
https://medium.com/@ruinadd/philosophers-42-guide-the-dining-philosophers-problem-893a24bc0fe2

POSIX Threads Documentation:
https://man7.org/linux/man-pages/man7/pthreads.7.html

Mutex documentation:
https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3.html

Wikipedia

42 Subject Documentation

---

## AI Usage

AI was used as a learning and explanatory tool to:

Clarify theoretical concepts (deadlock, starvation, scheduling, monitors).

Improve understanding of mutex behavior.

Help structure explanations for documentation purposes.

All implementation logic, debugging, and final design decisions were developed and validated independently.

---
