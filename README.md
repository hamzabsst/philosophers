# Philosophers

A C simulation of the Dining Philosophers problem, exploring synchronization, shared resources, timing, and concurrent execution as part of the 42 curriculum.

## Implementations

| Directory | Approach |
| --- | --- |
| `philo/` | POSIX threads and mutexes |
| `philo_bonus/` | Separate processes with semaphore-based synchronization |

Both versions include argument parsing, philosopher routines, monitoring, and cleanup code.

## Build and run

Requirements: Make, a C compiler, and a POSIX system supporting threads and named semaphores.

```sh
make -C philo
./philo/philo 5 800 200 200 7

make -C philo_bonus
./philo_bonus/philo_bonus 5 800 200 200 7
```

Arguments:

```text
number_of_philosophers time_to_die time_to_eat time_to_sleep [meals_per_philosopher]
```

Times are in milliseconds. The optional final argument sets a meal target. The simulation reports philosopher actions and monitors stopping conditions.

The committed Makefiles do not explicitly link with `-pthread`. If your toolchain requires it, build with:

```sh
make -C philo CFLAGS="-Wall -Wextra -Werror -pthread"
make -C philo_bonus CFLAGS="-Wall -Wextra -Werror -pthread"
```

Scheduling and timing depend on the host system; the example is an invocation, not a guarantee of a particular simulation outcome.

## Cleanup

```sh
make -C philo clean
make -C philo fclean
make -C philo_bonus fclean
```
