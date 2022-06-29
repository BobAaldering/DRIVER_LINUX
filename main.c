#include <linux/init.h> // Include the 'linux/init' header. It contains for instance module loading and unloading functionalities.
#include <linux/module.h> // Include the 'linux/module' header. It contains for instance general module information.

#include "device_file.h" // Include the 'device_file' header.
#include "gpio_communicator.h"

/* ~~~ MODULE INFORMATION ~~~ */

MODULE_DESCRIPTION("AALDERING DRIVER"); // The description of the module.
MODULE_LICENSE("GPL"); // The license of the module.
MODULE_AUTHOR("Aaldering, Bob"); // The author of the module.

/* ~~~ MODULE INITIALIZE FUNCTION ~~~ */

// This function initializes the 'aaldering_driver'.
static int aaldering_driver_init(void) {
    printk(KERN_NOTICE "[AALDERING DRIVER - MESSAGE] - Initialization started...\n"); // Print a kernel message, it will show up with the 'dmesg' command.

    initialize_gpio_shift_register();

    return register_device(); // Return the device number.
}

/* ~~~ MODULE DE-INITIALIZE FUNCTION ~~~ */

// This function de-initializes the 'aaldering_driver'.
static void aaldering_driver_exit(void) {
    printk(KERN_NOTICE "[AALDERING DRIVER - MESSAGE] - Exiting...\n"); // Print a kernel message, it will show up with the 'dmesg' command.

    de_initialize_gpio_shift_register();

    unregister_device(); // Unregister the device.
}

/* ~~~ LINUX INITIALIZING/EXITING ~~~ */

module_init(aaldering_driver_init); // Initialize the module.
module_exit(aaldering_driver_exit); // Exit the module.
