# RTOS From Scratch

Learning to build my own RTOS.

## Drivers

### LED Driver
- Reading STM32 docs to understand GPIO setup.
- Identified port, bus (AHB1), and clock enable register.
- Refer: UM1724, RM0091 (STM32F4 Reference Manual)

🚦 LED Driver Implementation

After identifying the correct GPIO port and associated AHB1 bus register, I began implementing a basic LED driver.
🔧 led_init()

Initializes the LED by:

    Enabling the clock access to GPIO Port A via the RCC->AHB1ENR register.

    Configuring Pin 5 of Port A as an output pin using the MODER register.

Key insight:

    Instead of directly assigning to the register (which can accidentally disable other peripherals), I used the bitwise OR (|=) operator to ensure that only the relevant bit (bit 0 for GPIOA) is set without affecting others.

🧠 Why Set MODER Bits?

The MODER register defines the direction of GPIO pins:

    1 at bit 10, 0 at bit 11 configures pin 5 as general-purpose output mode.

💡 Code Snippet:

#define GPIOAEN (1U << 0)

void led_init(void) {
    RCC->AHB1ENR |= GPIOAEN;            // Enable clock to GPIOA
    GPIOA->MODER |= (1U << 10);         // Set MODER10 = 1
    GPIOA->MODER &= ~(1U << 11);        // Set MODER11 = 0 => Output mode
}

🔛 led_on() & 🔻 led_off()

These functions control the LED state using the ODR (Output Data Register):

    led_on() sets Pin 5 high

    led_off() sets Pin 5 low

void led_on(void) {
    GPIOA->ODR |= (1U << 5);   // Turn LED ON
}

void led_off(void) {
    GPIOA->ODR &= ~(1U << 5);  // Turn LED OFF
}