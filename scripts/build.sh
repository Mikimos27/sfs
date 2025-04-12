#!/bin/bash

gcc src/sfs-server.c src/connection.c -o server.elf -Wall -Werror
gcc src/sfs-client.c src/connection.c -o client.elf -Wall -Werror

