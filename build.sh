#!/bin/bash

gcc sfs-server.c server.c -o server.elf
gcc sfs-client.c server.c -o client.elf
