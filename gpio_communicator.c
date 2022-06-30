#include "gpio_communicator.h"

#include <linux/kernel.h> // Include the 'linux/kernel' header. It contains for example an implementation of 'printk'.
#include <linux/delay.h> // Include the 'linux/delay' header. It contains various delay functions.
#include <linux/gpio.h> // Include the 'linux/gpio' header. It contains GPIO functionalities.

// Function to initialize the GPIO of the shift register.
void initialize_gpio_shift_register(void) {
    // Check if the specified GPIO is valid.
    if (gpio_is_valid(SHIFT_REG_DATA_PIN) == false || gpio_is_valid(SHIFT_REG_CLOCK_PIN) == false || gpio_is_valid(SHIFT_REG_LATCH_PIN) == false) {
        printk(KERN_NOTICE "[AALDERING DRIVER - MESSAGE] - GPIO for the shift register is not valid!\n"); // No valid GPIO, print a kernel message for this problem.
        return;
    }

    // Request the GPIO for the shift register.
    if (gpio_request(SHIFT_REG_DATA_PIN, "SHIFT_REG_DATA_PIN") < 0 || gpio_request(SHIFT_REG_CLOCK_PIN, "SHIFT_REG_CLOCK_PIN") < 0 || gpio_request(SHIFT_REG_LATCH_PIN, "SHIFT_REG_DATA_PIN") < 0) {
        printk(KERN_NOTICE "[AALDERING DRIVER - MESSAGE] - GPIO for the shift register cannot be requested!\n"); // GPIO cannot be requested, print a kernel message for this problem.
        return;
    }

    gpio_direction_output(SHIFT_REG_DATA_PIN, 0); // Set this pin as an output, for the data pin of the shift register.
    gpio_direction_output(SHIFT_REG_CLOCK_PIN, 0); // Set this pin as an output, for the clock pin of the shift register.
    gpio_direction_output(SHIFT_REG_LATCH_PIN, 0); // Set this pin as an output, for the latch pin of the shift register.
}

// Function to release all the resources allocated for GPIO.
void de_initialize_gpio_shift_register(void) {
    gpio_free(SHIFT_REG_DATA_PIN); // Release the GPIO for the data pin.
    gpio_free(SHIFT_REG_CLOCK_PIN); // Release the GPIO for the clock pin.
    gpio_free(SHIFT_REG_LATCH_PIN); // Release the GPIO for the latch pin.
}

// This function updates the output of the shift register (latch).
void update_output_shift_register(void) {
    ndelay(100); // Wait a short time (busy waiting).
    gpio_set_value(SHIFT_REG_LATCH_PIN, HIGH_OUTPUT); // Pull the latch pin high, to output the new result on the shift register.

    ndelay(100); // Wait a short time (busy waiting).
    gpio_set_value(SHIFT_REG_LATCH_PIN, LOW_OUTPUT); // Pull the latch pin low, a new output is visible on the seven segment display.
}

// This function writes a bit to the shift register (over the data pin).
void write_bit_shift_register(bool bit_value) {
    gpio_set_value(SHIFT_REG_DATA_PIN, bit_value ? HIGH_OUTPUT : LOW_OUTPUT); // Write the data.
    ndelay(100); // Wait a short time (busy waiting).

    gpio_set_value(SHIFT_REG_CLOCK_PIN, HIGH_OUTPUT); // Drive the pin for the clock high, the shift register will cap the value on its data pin.
    ndelay(100); // Wait a short time (busy waiting).
    gpio_set_value(SHIFT_REG_CLOCK_PIN, LOW_OUTPUT); // Drive the pin for the clock low again.
}

// This function writes a complete byte to the shift register and latches it.
void write_byte_shift_register(int specific_digit) {
    int bit_number = 0;

    // Go through all the bits that must be displayed on the seven segment display.
    for (; bit_number < BIT_SIZE; bit_number++) {
        write_bit_shift_register(digit_representation_seven_segment[specific_digit] & (1 << bit_number)); // Send a single bit to the shift register.
        ndelay(100); // Wait a short time (busy waiting).
    }

    update_output_shift_register(); // Latch the output, everything must be visible on the seven segment display.
}
