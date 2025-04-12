#!/bin/bash

gcc src/sfs-server.c src/connection.c -o server.elf
gcc src/sfs-client.c src/connection.c -o client.elf

