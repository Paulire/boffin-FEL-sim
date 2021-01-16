#!/bin/bash

gcc -g -fPIC -Wall -Werror -Wextra -pedantic *.c -shared -o ~/.local/lib/boffin/file_handle.so
