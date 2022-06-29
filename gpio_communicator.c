#include "gpio_communicator.h"

#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/gpio.h>

void initialize_gpio_shift_register(void) {
    if (gpio_is_valid(SHIFT_REG_DATA_PIN) == false || gpio_is_valid(SHIFT_REG_CLOCK_PIN) == false || gpio_is_valid(SHIFT_REG_LATCH_PIN) == false) {
        printk(KERN_NOTICE "[AALDERING DRIVER - MESSAGE] - GPIO for the shift register is not valid!\n");
        return;
    }

    if (gpio_request(SHIFT_REG_DATA_PIN, "SHIFT_REG_DATA_PIN") < 0 || gpio_request(SHIFT_REG_CLOCK_PIN, "SHIFT_REG_CLOCK_PIN") < 0 || gpio_request(SHIFT_REG_LATCH_PIN, "SHIFT_REG_DATA_PIN") < 0) {
        printk(KERN_NOTICE "[AALDERING DRIVER - MESSAGE] - GPIO for the shift register cannot be requested!\n");
        return;
    }

    gpio_direction_output(SHIFT_REG_DATA_PIN, 0);
    gpio_direction_output(SHIFT_REG_CLOCK_PIN, 0);
    gpio_direction_output(SHIFT_REG_LATCH_PIN, 0);
}

void de_initialize_gpio_shift_register(void) {
    gpio_free(SHIFT_REG_DATA_PIN);
    gpio_free(SHIFT_REG_CLOCK_PIN);
    gpio_free(SHIFT_REG_LATCH_PIN);
}

void update_output_shift_register(void) {
    ndelay(100);
    gpio_set_value(SHIFT_REG_LATCH_PIN, HIGH_OUTPUT);

    ndelay(100);
    gpio_set_value(SHIFT_REG_LATCH_PIN, LOW_OUTPUT);
}

void write_bit_shift_register(bool bit_value) {
    gpio_set_value(SHIFT_REG_DATA_PIN, bit_value ? HIGH_OUTPUT : LOW_OUTPUT);
    ndelay(100);

    gpio_set_value(SHIFT_REG_CLOCK_PIN, HIGH_OUTPUT);
    ndelay(100);
    gpio_set_value(SHIFT_REG_CLOCK_PIN, LOW_OUTPUT);
}

void write_byte_shift_register(int specific_digit) {
    int bit_number = 0;

    for (; bit_number < BIT_SIZE; bit_number++) {
        write_bit_shift_register(digit_representation_seven_segment[specific_digit] & (1 << bit_number));
        ndelay(100);
    }

    update_output_shift_register();
}
