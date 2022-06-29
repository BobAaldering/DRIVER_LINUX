#!/bin/sh

# This shell script is used to generate easily the device driver.

DIR="cmake-build" # Build directory.

# Check if the folder already exists.
if [ -d "$DIR" ]; then
	cd "${DIR}" || exit # Create the directory, otherwise exit.

  echo "[KERNEL MODULES] - Running 'CMake' for loading/unloading kernel modules..." # Message that we are now running 'CMake'.

	cmake .. # Run 'CMake' in this case.
# Otherwise, create the directory first.
else
	mkdir "${DIR}" # Create the build directory.
  cd "${DIR}" || exit # Step into the created directory.

  echo "[KERNEL MODULES] - Running 'CMake' for loading/unloading kernel modules..." # Message that we are now running 'CMake'.

  cmake .. # Run 'CMake' in this case.
fi

make driver # Make the target 'driver'.

value=$1 # Get the first argument passed to this script.

# Check if we want to load the module into the kernel.
if [ "${value}" = "load" ]; then
  sudo make load # Make the target 'load'.

  module_information=$(grep aaldering_module /proc/modules)
  device_number=$(grep AALDERING-DRIVER /proc/devices) # Check if the device driver is there.

  echo "[KERNEL MODULES] - Information of the specific module: $module_information"
  echo "[KERNEL MODULES] - Information of the specific device: $device_number"

  sudo mknod /dev/AALDERING-DRIVER c "${device_number%% *}" 0
  sudo chmod 777 /dev/AALDERING-DRIVER

  echo "[KERNEL MODULES] - Loaded the device driver!"
elif [ "${value}" = "unload" ]; then
  sudo rm /dev/AALDERING-DRIVER # We are unloading, remove also the driver (just a file).
  sudo make unload # Make the target 'unload'.

  echo "[KERNEL MODULES] - Unloaded the device driver!"
else
  echo "[KERNEL MODULES] - No target specified, use as argument 'load' or 'unload' for loading/unloading the driver!"
fi
