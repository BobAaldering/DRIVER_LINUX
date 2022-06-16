cmake_minimum_required(VERSION 3.18) # The minimum CMake version used.

project(driver VERSION 1.0.0 LANGUAGES C) # The specification of this project.
set(CMAKE_C_STANDARD 11) # Set the C language standard. We are using C11.
set(CMAKE_C_STANDARD_REQUIRED ON) # Use the C standards.

set(MODULE_NAME aaldering-module) # The name of the module.

add_definitions(-D__KERNEL__ -DMODULE) # Add a definition for kernel modules.

# Execute this command, to find your current Linux kernel version.
execute_process(
        COMMAND uname -r # This finds your kernel version.
        OUTPUT_VARIABLE KERNEL_RELEASE # Output the kernel release.
        OUTPUT_STRIP_TRAILING_WHITESPACE # Add a whitespace.
        )

# Now, we want to find the path with all the kernel modules.
find_path(
        KERNELHEADERS_DIR # Kernel headers directory.
        include/linux/user.h # Include this header file.
        PATHS /usr/src/linux-headers-${KERNEL_RELEASE} # The path of the kernel modules.
        )

message(STATUS "Kernel release: ${KERNEL_RELEASE}") # Message to show the kernel release.
message(STATUS "Kernel headers: ${KERNELHEADERS_DIR}") # Message to show the directory with kernel headers/modules.

# Include the kernel directories with 'CMake'.
include_directories(
        ${KERNELHEADERS_DIR}/include
        )

set(DRIVER_FILE "${MODULE_NAME}.ko") # Set the name of the '*.ko' file.
set(KBUILD_CMD $(MAKE) -C ${KERNELHEADERS_DIR} modules M=${CMAKE_CURRENT_BINARY_DIR} src=${CMAKE_CURRENT_SOURCE_DIR}) # Execute the build command of 'make'.

FILE(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/Kbuild "${MODULE_NAME}-objs := main.o device_file.o\n obj-m := ${MODULE_NAME}.o") # Let CMake generate the 'Kbuild' file.

# This is my custom command to make sure that all the dependencies are there.
add_custom_command(OUTPUT ${DRIVER_FILE}
        COMMAND ${KBUILD_CMD}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} # Current working directory.
        DEPENDS main.c device_file.c VERBATIM # Dependencies.
        )

# This target is used to build the actual device driver (and the '*ko' file).
add_custom_target(driver
        ALL DEPENDS ${DRIVER_FILE}
        )

# This target is used to load the module into the kernel (use it with 'sudo' rights).
add_custom_target(load
        COMMAND insmod ./${MODULE_NAME}.ko
        )

# This target is used to unload the module from the kernel (use it with 'sudo' rights).
add_custom_target(unload
        COMMAND rmmod ./${MODULE_NAME}.ko
        )

# Target used for CLion, to have all the nice features of the IDE. Do not use this target for building, because it will not compile.
add_library(dummy_target
        main.c # The 'main' file of the project.
        device_file.c # External source file.
        )
