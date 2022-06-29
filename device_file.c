#include "device_file.h"

#include <linux/fs.h> // Include the 'linux/fs' header. It contains functionalities for file access.
#include <linux/kernel.h> // Include the 'linux/kernel' header. It contains for example an implementation of 'printk'.
#include <linux/errno.h> // Include the 'linux/errno' header. It contains various error codes.
#include <linux/module.h> // Include the 'linux/module' header. It contains for instance general module information.
#include <linux/cdev.h> // Include the 'linux/cdev' header. It contains character device functionalities.
#include <linux/uaccess.h> // Include the 'linux/uaccess' header. It contains 'copy to user' functionalities.

#define BUFFER_SIZE_READ_WRITE (50)

static char g_s_kernel_mode_buffer[BUFFER_SIZE_READ_WRITE] = ""; // String to show from kernel mode, pay attention to the null character.
static const ssize_t g_s_kernel_mode_size = sizeof(g_s_kernel_mode_buffer); // The size of the string, to show from kernel mode.

/* ~~~ READ/WRITE DEVICE FILE FUNCTION ~~~ */

// This function reads the file from user space.
static ssize_t device_file_read(struct file *file_ptr, char __user *user_buffer, size_t count, loff_t *position) {
    printk(KERN_NOTICE "[AALDERING DRIVER - MESSAGE] - Trying to read device file at offset = %i, read bytes count = %u.\n", (int) *position, (unsigned int) count); // Print a kernel message, it will show up with the 'dmesg' command.

    // Check the position from the buffer.
    if (*position >= g_s_kernel_mode_size)
        return 0; // No data is read from the buffer, return a value of zero. Linux will not try read the buffer again (see page 66 of the book 'LINUX DEVICE DRIVERS').

    // Check the current position.
    if (*position + count > g_s_kernel_mode_size)
        count = g_s_kernel_mode_size - *position; // The new value of 'count'.

    // Check if copying information from kernel space to user space succeeded.
    if (copy_to_user(user_buffer, g_s_kernel_mode_buffer + *position, count) != 0)
        return -EFAULT; // Error code for providing a bad address.

    *position += count; // New value of 'count'.

    return (ssize_t) count; // Return the value of 'count'.
}

static ssize_t device_file_write(struct file *file_ptr, const char __user *user_buffer, size_t count, loff_t *position) {
    printk(KERN_NOTICE "[AALDERING DRIVER - MESSAGE] - Trying to write device file at offset = %i, read bytes count = %u.\n", (int) *position, (unsigned int) count); // Print a kernel message, it will show up with the 'dmesg' command.

    // Check the position from the buffer.
    if (*position >= g_s_kernel_mode_size)
        return -ENOSPC; // When the user exceeded the buffer size, a value of zero can be returned. But Linux tries to write data into the buffer again. This results in an infinite operation, so return an error in this case (see page 68 of the book 'LINUX DEVICE DRIVERS').

    // Check the current position.
    if (*position + count > g_s_kernel_mode_size)
        count = g_s_kernel_mode_size - *position; // The new value of 'count'.

    // Check if copying information from user space to kernel space succeeded.
    if (copy_from_user(g_s_kernel_mode_buffer + *position, user_buffer, count) != 0)
        return -EFAULT; // Error code for providing a bad address.

    *position += count; // New value of 'count'.

    return (ssize_t) count; // Return the value of 'count'.
}

/* ~~~ FILE OPERATIONS STRUCTURE ~~~ */

// This is our 'file operations' structure.
static struct file_operations simple_driver_fops = {
    .owner = THIS_MODULE, // The owner of the module.
    .read = device_file_read, // The read operation for the driver.
    .write = device_file_write
};

/* ~~~ DEVICE MAJOR NUMBER AND DEVICE NAME ~~~ */

static int device_file_major_number = 0; // Specify the major device number, to specify devices in a group.
static const char device_name[] = "AALDERING-DRIVER"; // The name of the driver.

/* ~~~ FUNCTION TO REGISTER THE DEVICE ~~~ */

// This function registers a device.
int register_device(void) {
    int result = 0; // This will hold the current result.

    printk(KERN_NOTICE "[AALDERING DRIVER - MESSAGE] - The function 'register_device()' is called.\n"); // Print a kernel message, it will show up with the 'dmesg' command.

    result = register_chrdev(0, device_name, &simple_driver_fops); // Register the device. Let Linux do the registration.

    // Check if registering succeeded.
    if (result < 0) {
        printk(KERN_WARNING "[AALDERING DRIVER - MESSAGE] - Can not register character device with error code = %i.\n", result); // Print a kernel message, it will show up with the 'dmesg' command.
        return result; // Return the result.
    }

    device_file_major_number = result; // The value of the major device number.

    printk(KERN_NOTICE "[AALDERING DRIVER - MESSAGE] - Registered character device with major number = %i and minor numbers 0 till 255.\n", device_file_major_number); // Print a kernel message, it will show up with the 'dmesg' command.

    return 0; // Return from the function.
}

/* ~~~ FUNCTION TO UNREGISTER THE DEVICE ~~~ */

// This function de-initializes a device.
void unregister_device(void) {
    printk(KERN_NOTICE "[AALDERING DRIVER - MESSAGE] - The function 'unregister_device()' is called.\n"); // Print a kernel message, it will show up with the 'dmesg' command.

    // Check if you can unregister the device.
    if (device_file_major_number != 0)
        unregister_chrdev(device_file_major_number, device_name); // Unregister the device.
}
