#!/bin/bash

gcc -g -fPIC -Wall -Werror -Wextra -pedantic *.c -shared -o ~/.local/lib/boffin/fel_inter.so
