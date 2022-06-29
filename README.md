# Drivers

#### Study on operating systems, creating a device driver.

This project contains a device driver developed for Linux (more specifically a Raspberry Pi). This driver has been rewritten as `AALDERING-DRIVER`. Subsequently, a number of additional functionalities were added to make the driver more complex.

It is possible to build the modules using `CMake`. For this, this 'README' also provides a further explanation of how this is possible, and how these created modules can be added to the kernel.

## Build and execute

In order to be able to 'install' the driver on the Linux system I will clearly state this in two ways. In the first way, I just use `CMake`, along with a few other commands. In the second case I just use a shell script, which already runs `CMake` and `make`.

**Complete with `CMake`:**

1. Create a new build folder in the project. This folder is used for various configuration files, and ultimately the `*.ko` file. For example, create this folder like this and go into that specific folder (name doesn't matter):
    ```shell
    ../DRIVER_LINUX:~$ mkdir cmake-build
    ../DRIVER_LINUX:~$ cd cmake-build
    ```
2. Now all the different CMake files should be generated. This is necessary too eventually.
    ```shell
    ../DRIVER_LINUX/cmake-build:~$ cmake ..
    ```
3. Now it's time to actually build the module for the driver. This is done using `make` for the target `driver`.
    ```shell
    ../DRIVER_LINUX/cmake-build:~$ make driver
    ```
4. Now a `*ko` file should be available in the folder, for this project this will always be `aaldering-module.ko`.
    ```shell
    ../DRIVER_LINUX/cmake-build:~$ ls
    aaldering-module.ko   aaldering-module.mod.c   ...
    aaldering-module.mod  aaldeirng-module.mod.o   ...
    ```
5. Now this module needs to be added to the kernel. For this we use `make` again, with the target `load`.
    ```shell
    ../DRIVER_LINUX/cmake-build:~$ sudo make load
    ```
6. The module has been added to the kernel. Check this in `/proc/modules` and `proc/devices`. In the latter case, your major device number is also visible.
    ```shell
    ../DRIVER_LINUX/cmake-build:~$ cat /proc/modules
    aaldering_module 16384 0 - Live 0xbf4ec000 (O)
    joydev 20480 0 - Live 0xbf4ce000
    cmac 16384 3 - Live 0xbf502000
    ...
    ../DRIVER_LINUX/cmake-build:~$ cat /proc/devices
    ...
    254 gpiochip
    510 AALDERING-DRIVER
    511 cec
    ...
    ```
7. Above you can see the driver, which can be recognized by `510 AALDERING-DRIVER`. The number in front must be used to create a special character file associated with the major number `510`. You do this with the following command:
    ```shell
    ../DRIVER_LINUX/cmake-build:~$ sudo mknod /dev/AALDERING-DRIVER c 510 0
    ```
8. You have to change the access right of the driver with the following command:
   ```shell
   ../DRIVER_LINUX/cmake-build:~$ sudo chmod 777 /dev/AALDERING-DRIVER
   ```
9. Now a driver of its own has been added to your Linux operating system! Check this with:
    ```shell
    ../DRIVER_LINUX/cmake-build:~$ echo "TEST" > /dev/AALDERING-DRIVER
    ../DRIVER_LINUX/cmake-build:~$ cat /dev/AALDERING-DRIVER
    ```

Sometimes you also want to remove the module from the kernel, as well as the device driver. You do that in the following way:
1. Now `make` is used again, but then the target `unload`.
    ```shell
    ../DRIVER_LINUX/cmake-build:~$ make unload
    ```
2. Then there is one last step to remove the device driver. Know that this is just a file in Linux, so here's how you do it:
    ```shell
    ../DRIVER_LINUX/cmake-build:~$ sudo rm /dev/AALDERING-DRIVER
    ```

Now that the module has been removed from the kernel, as well as the device driver itself, it is possible to go through the above steps again.

**Complete with the `shell` script:**
1. The [shell](load_kernel_module.sh) script makes the above steps of building a device driver easier. One argument is very important to the script, namely `load`. This adds a module to the kernel. All other arguments ensure that it is removed if possible.
    ```shell
    ../DRIVER_LINUX/cmake-build:~$ sudo sh load_kernel_module.sh load
    ```
2. Now your driver is build and completely inserted into the kernel! How easy is that!
4. Now check if the driver is actually available, and if you see output.
    ```shell
    ../DRIVER_LINUX/cmake-build:~$ echo "TEST" > /dev/AALDERING-DRIVER
    ../DRIVER_LINUX/cmake-build:~$ cat /dev/AALDERING-DRIVER
    ```

Again it is possible to remove the device driver and the object for it from the kernel.

1. Any other argument can now be used for the shell script, except `load`. For clarity, use the `unload` argument. This ensures that the module is removed from the kernel, and you can add a new one in case of changes, for example.
    ```shell
    ../DRIVER_LINUX/cmake-build:~$ sudo sh load_kernel_module.sh unload
    ```
2. Now the device driver is completely unloaded from the kernel! Just as easy as loading the device driver!
   
## Correct functioning

Now a picture will be given of how the driver should function. Here you can see the kernel messages with the command `dmesg`. To see the output of the character device itself you can just run `cat /dev/AALDERIN-DRIVER`.

```shell
...
[ 3291.010622] [AALDERING DRIVER - MESSAGE] - Initialization started...
[ 3291.010633] [AALDERING DRIVER - MESSAGE] - The function 'register_device()' is called.
[ 3291.010642] [AALDERING DRIVER - MESSAGE] - Registered character device with major number = 510 and minor numbers 0 till 255.
[ 3307.847297] [AALDERING DRIVER - MESSAGE] - Trying to write device file at offset = 0, read bytes count = 5.
[ 3316.149036] [AALDERING DRIVER - MESSAGE] - Trying to read device file at offset = 0, read bytes count = 131072.
[ 3316.149125] [AALDERING DRIVER - MESSAGE] - Trying to read device file at offset = 50, read bytes count = 131072.
bobaa@armpi:~$../DRIVER_LINUX $ echo "TEST" > /dev/AALDERING-DRIVER
bobaa@armpi:~$../DRIVER_LINUX $ cat /dev/AALDERING-DRIVER
TEST
```
