#ifndef DRIVER_GPIO_COMMUNICATOR_H
#define DRIVER_GPIO_COMMUNICATOR_H

#include <linux/types.h> // Include the 'linux/types' header. It contains various type for kernel functionalities.

#define SHIFT_REG_DATA_PIN (17) // Pin configuration for the data (RPi -> 74HC595).
#define SHIFT_REG_CLOCK_PIN (18) // Pin configuration for the data (RPi -> 74HC595).
#define SHIFT_REG_LATCH_PIN (27) // Pin configuration for the data (RPi -> 74HC595).

#define HIGH_OUTPUT (1) // High output on a pin.
#define LOW_OUTPUT (0) // Low output on a pin.

#define BIT_SIZE (8) // The size of one byte (in bit).

// Representation of all the values that can be showed on the seven segment display. The index for every item is the same as its value.
// It is possible to show hexadecimal numbers on the segment display.
// Configuration for a byte on the seven segment display 0b11111111 ->
//                                                         ABCDEFG-.
static const uint8_t digit_representation_seven_segment[] = {
        0b00000010, // The digit '0'.
        0b10011110, // The digit '1'.
        0b00100100, // The digit '2'.
        0b00001100, // The digit '3'.
        0b10011000, // The digit '4'.
        0b01001000, // The digit '5'.
        0b01000000, // The digit '6'.
        0b00011110, // The digit '7'.
        0b00000000, // The digit '8'.
        0b00001000, // The digit '9'.
        0b00010000, // The digit 'A'.
        0b11000000, // The digit 'B'.
        0b01100010, // The digit 'C'.
        0b10000100, // The digit 'D'.
        0b01100000, // The digit 'E'.
        0b01110000 // The digit 'F'.
};

// Structure that contains all the supported options, with an identification that they exist.
typedef struct provided_option_checker {
    bool is_display_option; // Indication for the '--display' option.
    bool is_countdown_option; // Indication for the '--countdown' option.
    bool is_countup_option; // Indication for the '--countup' option.
} provided_option_checker_t;

void initialize_gpio_shift_register(void); // Function to initialize the GPIO of the shift register.
void de_initialize_gpio_shift_register(void); // Function to release all the resources allocated for GPIO.

void update_output_shift_register(void); // This function updates the output of the shift register (latch).
void write_bit_shift_register(bool bit_value); // This function writes a bit to the shift register (over the data pin).
void write_byte_shift_register(int specific_digit); // This function writes a complete byte to the shift register and latches it.

#endif
