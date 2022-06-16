#!/bin/sh

DIR="build" # Build directory.

# Check if the folder already exists.
if [ -d "$DIR" ]; then
	cd DIR || exit
# Otherwise, make the
else
	mkdir DIR
  cd DIR || exit
fi

make -C /usr/src/linux-headers-"$(uname -r)"/ M="$(pwd)"
make load
