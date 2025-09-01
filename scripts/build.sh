#!/bin/bash

gcc src/sfs-server.c src/server.c -o server.elf
gcc src/sfs-client.c src/server.c -o client.elf

