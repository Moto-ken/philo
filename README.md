*This project has been created as part of the 42 curriculum by kemotoha.*

## Description

Here are the key things you need to know to succeed in this assignment:
• One or more philosophers sit at a round table.
There is a large bowl of spaghetti in the middle of the table.
• The philosophers take turns eating, thinking, and sleeping.
While they are eating, they are not thinking nor sleeping;
while thinking, they are not eating nor sleeping;
and, of course, while sleeping, they are not eating nor thinking.
• There are also forks on the table. There are as many forks as philosophers.
• Since eating spaghetti with just one fork is impractical, a philosopher must pick up
both the fork to their right and the fork to their left before eating.
• When a philosopher has finished eating, they put their forks back on the table and
start sleeping. Once awake, they start thinking again. The simulation stops when
a philosopher dies of starvation.
• Every philosopher needs to eat and should never starve.
• Philosophers do not communicate with each other.
• Philosophers do not know if another philosopher is about to die.
• Needless to say, philosophers should avoid dying!

## Instructions

To compile: make
Example of execution: ./philo 5 800 200 200 3

- `number_of_philosophers`: total number of philosophers (and forks)
- `time_to_die`: time in milliseconds before a philosopher dies without eating
- `time_to_eat`: time in milliseconds to eat
- `time_to_sleep`: time in milliseconds to sleep
- `[number_of_times_each_philosopher_must_eat]` (optional): stop simulation if all philosophers have eaten this many times

## Resources

- References:
  - https://qiita.com/42yliu/items/86d16cdbc584c250ca6e#fn-1
  - https://note.com/tedd_jp/n/n84089858b417

- AI Usage:
  I used AI assistance for:
    - Refactoring code to improve readability and Norm compliance.
    - Understanding how to properly implement mutexes and thread synchronization.
    - Guidance on writing the README.md sections (Description, Instructions, Resources).
