cmake_minimum_required(VERSION 3.18) # The minimum CMake version used.

project(DRIVER_LINUX VERSION 1.0.0 LANGUAGES C) # The specification of this project.
set(CMAKE_C_STANDARD 11) # Set the C language standard. We are using C11.
set(CMAKE_C_STANDARD_REQUIRED ON) # Use the C standards.

set(TARGET_MODULE aaldering-driver)

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

# Instruct the compiler where to look for the given header files within this project.
include_directories(
        ./inc
)

# Include the kernel directories with 'CMake'.
include_directories(
        ${KERNELHEADERS_DIR}/include
)

set(DRIVER_FILE ${TARGET_MODULE}.ko) # Set the '*.ko' files.
set(KBUILD_CMD $(MAKE) -C ${KERNELHEADERS_DIR} M=${CMAKE_CURRENT_BINARY_DIR} modules) # Execute the build command of 'make'.

FILE(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/Kbuild "${TARGET_MODULE}-objs := main.o device_file.o obj-m := ${TARGET_MODULE}.o") # Let CMake generate the 'Kbuild' file.

# This is my custom command to make sure that all the dependencies are there.
add_custom_command(OUTPUT ${DRIVER_FILE}
        COMMAND ${KBUILD_CMD}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} # Current working directory.
        DEPENDS main.c ./src/device_file.c VERBATIM # Dependencies.
        )

add_custom_target(driver
        ALL DEPENDS ${DRIVER_FILE} # Add all the dependencies.
        )

add_custom_target(load
        COMMAND insmod ./${TARGET_MODULE}.ko
        )

add_custom_target(unload
        COMMAND rmmod ./${TARGET_MODULE}.ko
        )
