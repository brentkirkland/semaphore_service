# semaphore_service

### Brent Kirkland and Greg Gunterson

```
____  ___ _ __ ___   __ _ _ __ | |__   ___  _ __ ___
/ __|/ _ \ '_ ` _ \ / _` | '_ \| '_ \ / _ \| '__/ _ \
\__ \  __/ | | | | | (_| | |_) | | | | (_) | | |  __/
|___/\___|_| |_| |_|\__,_| .__/|_| |_|\___/|_|  \___|
                         |_|                         
```                       

This minix 3.2.1 project creates a minix _service_ for semaphores. This is not service that starts directly with boot.

Semaphores allow for synchronization across multiple processes. Within this project are 4 functions that help solve synchronization issues.

* `int sem_init(int semaphore_value)` which creates a semaphore within a semaphore list.
* `int sem_up(int semaphore_number)` which increments a specific semaphore up by one or removes it off a waiting queue.
* `int sem_down(int semaphore_number)` which decrements a specific semaphore up by one or adds it to a waiting queue.
* `int sem_release(int semaphore_number)` which releases the semaphore from the semaphore list.

### TO COMPILE AFTER RUNNING PATCH:
```
1. cd /usr/src/
2. make build MKUPDATE=yes
3. make include etcforce
4. reboot
```

_NOTE:_ `make include etcforce` can cause trouble with pkgin. Use it with caution or include:

```
service sema {
  system ALL:
  ipc ALL;
  uid 0;
};
```
in /etc/system.conf


### TESTING
Compile pizza.c with cc. There is only one single file for for testing, because it instead uses fork and wait to spawn separate processes for undergrads and grads.

_NOTE:_ pizza.txt explains what is happening is pizza.c