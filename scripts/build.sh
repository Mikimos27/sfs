#!/bin/bash

gcc src/server.c src/connection.c -o server.elf -Wall -Werror
gcc src/client.c src/connection.c -o client.elf -Wall -Werror

