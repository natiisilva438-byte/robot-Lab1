#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
export PATH="/usr/games:$PATH"

# Install build-essential, cmake, fortune-mod, fortunes-min, and locales first.
printf 'Lines: '; wc -l < dante.txt
printf 'Words: '; wc -w < dante.txt
printf 'Nonblank: '; grep -c '[^[:space:]]' dante.txt

: > fortunes.txt
for _ in 1 2 3 4 5; do fortune >> fortunes.txt; done

cmake -S RandomVector -B RandomVector/build
cmake --build RandomVector/build
(cd RandomVector/build && ctest --output-on-failure)
RandomVector/build/random_vector
