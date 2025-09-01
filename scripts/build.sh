#!/bin/bash

g++ src/server.cpp src/connection.cpp -o server.elf -Wall -Werror
g++ src/client.cpp src/connection.cpp -o client.elf -Wall -Werror

