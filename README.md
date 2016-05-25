# semaphore_service

### Brent Kirkland and Greg Gunterson

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
