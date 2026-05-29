*This project has been created as part of the 42 curriculum by [Diego Montanari].*

## Description
This repository contains my implementations for 42 projects. For the Philosophers project, the goal is to solve the Dining Philosophers synchronization problem in C using threads and mutexes (mandatory), and processes/semaphores (bonus).

## Instructions
### Philosophers (mandatory)
- Directory: `philo/`
- Build: `make`
- Run: `./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]`
- Clean: `make clean` / `make fclean` / `make re`

Example:
`./philo 4 410 200 200 3`

## Resources
- POSIX Threads Programming
- `man pthread_create`, `man pthread_mutex_lock`, `man gettimeofday`, `man usleep`
- The Dining Philosophers Problem (Dijkstra)

### AI usage
AI was used as a coding assistant for:
- discussing synchronization strategies and tradeoffs,
- drafting initial code skeletons,
- reviewing edge cases (single philosopher, stop conditions, logging order),
- helping prepare checklist-style validation before peer evaluation.

All final code choices, debugging, and validation were reviewed and adjusted manually.
