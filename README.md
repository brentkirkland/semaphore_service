# semaphore_service

### Brent Kirkland and Greg Gunterson

This minix 3.2.1 project creates a minix _service_ for semaphores. This is not service that starts directly with boot.

Semaphores allow for synchronization across multiple processes. Within this project are 4 functions that help solve synchronization issues.

* `int sem_int(int semaphore_value)` which creates a semaphore
* `int sem_up(int semaphore_number)` which increments a specific semaphore up by one. Or removes it off a waiting queue.
* `int sem_int(int semaphore_number)` which decrements a specific semaphore up by one. Or adds it to a waiting queue.
* `int sem_int(int semaphore_number)` which releases the semaphore from the semaphore list.

## TO COMPILE:
```
1. Run Patch
2. cd /usr/src/
3. make build MKUPDATE=yes
4. make include etcforce
5. reboot

```

_NOTE:_ `ake include etcforce` can cause trouble with pkgin. Use it with caution or include:

```
service sema {
  system ALL:
  ipc ALL;
  uid 0;
}
```

## Testing
Compile pizza.c with cc. There is only one single for for testing, because it instead uses fork and wait to spawn separate processes for undergrads and grads.

_NOTE:_ pizza.txt explain what is happening is pizza.c
