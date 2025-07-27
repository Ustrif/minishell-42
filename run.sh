#!/bin/bash

make

clear

valgrind \
  --leak-check=full \
  --show-leak-kinds=all \
  --track-origins=yes \
  --suppressions="$(pwd)/val.supp" \
  --track-fds=yes \
  ./minishell
