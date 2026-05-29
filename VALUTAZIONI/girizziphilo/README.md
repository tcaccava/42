*This project has been created as part of the 42 curriculum by girizzi.*

# The dining philosophers problem

### The Challenge
Five philosophers are sitting together around a table. In front of each of them is a bowl of spaghetti and there is a fork between each pair of adjacent plates. Each philosopher can only alternately **eat**, **sleep**, or **think**. But there's a catch: to eat this kind of spaghetti, a philosopher needs **two forks**. 

If they don't eat within a certain specified time, they **die** of starvation. The problem is how to design a concurrent algorithm such that any philosopher will not starve to death.

Balancing their needs assuming that no philosopher can know when others may want to eat or think while avoiding deadlocks and memory leaks is the core of this assignment.

### Statement
- One or more philosophers sit at a round table.
- There is a large bowl of spaghetti in front of each philosopher.
- The philosophers alternatively **eat**, **think**, or **sleep**.
- There are as many forks as philosophers.
- A philosopher must need both their right and left forks to eat.
- When they finish eating, they put the forks back and start sleeping.
- The simulation stops when a philosopher dies of starvation.
- **Goal**: No philosopher should die!

## Technical Implementation

A deep dive into the world of concurrent programming, race conditions, and synchronization.

### Mandatory Part: Threads and Mutexes
In the `philo/` directory, each philosopher is a **thread**. To manage the shared forks, I used **mutexes** (Mutual Exclusion). Every fork is a mutex that can only be "locked" by one philosopher at a time.

- I assigned the forks based on the philosopher's ID to prevent circular wait (a classic deadlock condition).
- To avoid CPU over-consumption, I implemented a monitoring loop that checks the health of all philosophers at high frequency without stalling the simulation.

**Precise Timing (`ft_usleep`):**
Standard `usleep` is notoriously imprecise. In a simulation where milliseconds mean the difference between life and death, I implemented a custom `ft_usleep`. It checks the time in small chunks, ensuring that a philosopher never sleeps longer than they should, keeping the simulation perfectly aligned with the subject's strict requirements.

**Thread-Safe Logging:**
Printing to the console is a shared resource. I implemented a protected `print_status` function that uses a specific lock to ensure that logs never overlap, and more importantly, that no other thread prints anything after a philosopher has died.

---

## Instructions

### Compilation

```bash
cd philo && make && ./philo 5 800 200 200
```

### Execution
Run the program with the following arguments:
```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

### Arguments:
1. `number_of_philosophers`: The number of philosophers and also the number of forks.
2. `time_to_die` (ms): If a philosopher didn’t start eating `time_to_die` milliseconds since the beginning of their last meal or the beginning of the simulation, they die.
3. `time_to_eat` (ms): The time it takes for a philosopher to eat. During that time, they will need to hold two forks.
4. `time_to_sleep` (ms): The time a philosopher will spend sleeping.
5. `number_of_times_each_philosopher_must_eat` (optional): If all philosophers have eaten at least this many times, the simulation stops. If not specified, the simulation stops when a philosopher dies.

### Examples
- **Philosopher dies**: `./philo 1 800 200 200`
- **Philosophers survive**: `./philo 5 800 200 200`
- **Simulation with meal limit**: `./philo 5 800 200 200 7`

---

### Resources

- [The Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Philosophers 42 Guide](https://medium.com/@ruinadd/philosophers-42-guide-the-dining-philosophers-problem-893a24bc0fe2)
- [The Deadlock problem](https://webdocs.cs.ualberta.ca/~tony/OldPapers/dead.pdf)
- [The little book of semaphores](https://greenteapress.com/wp/semaphores/)
- [Philosophers Tutorial by Oceano](https://www.youtube.com/watch?v=zOpzGHwJ3MU)

This project adheres to the **42 Norm**.
