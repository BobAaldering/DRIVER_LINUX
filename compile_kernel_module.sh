#!/bin/sh

# This shell script is used to generate easily the device driver.

DIR="cmake-build" # Build directory.

# Check if the folder already exists.
if [ -d "$DIR" ]; then
	cd "${DIR}" || exit # Create the directory, otherwise exit.

	cmake .. # Run 'CMake' in this case.
# Otherwise, create the directory first.
else
	mkdir "${DIR}" # Create the build directory.
  cd "${DIR}" || exit # Step into the created directory.

  cmake .. # Run 'CMake' in this case.
fi

make driver # Make the target 'driver'.

value=$1 # Get the first argument passed to this script.

# Check if we want to load the module into the kernel.
if [ "${value}" = "load" ]; then
  make load # Make the target 'load'.
else
  make unload # Make the target 'unload'.
  rm /dev/AALDERING-DRIVER # We are unloading, remove also the driver (just a file).
fi

cat /proc/modules # Check if the module is just inserted.
cat /proc/devices # Check if the device driver is there.
