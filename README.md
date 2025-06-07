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


📡 UART Driver (USART2 TX Only)

Next, I moved on to building a basic UART transmitter using USART2.
🔍 Reference Manuals Used:

    UM1724 – STM32 Nucleo-64 User Manual

    RM0090 – STM32F4 Reference Manual

🧠 GPIOA Pin 2 (PA2) Setup:

To use PA2 for USART transmission:

    Set MODER bits to alternate function mode (MODER4 = 0, MODER5 = 1)

    Configure AFR[0] to select Alternate Function AF7 (USART2 TX/RX)

GPIOA->MODER |= (1U<<5);
GPIOA->MODER &=~(1U<<4);

GPIOA->AFR[0] &= ~(1U<<11); // Clear bit 11
GPIOA->AFR[0] |= (1U<<10) | (1U<<9) | (1U<<8); // Set bits 10:8 => AF7

🔧 USART2 Configuration

RCC->APB1ENR |= (1U << 17); // Enable clock for USART2

Then set the baud rate (e.g., 115200) by configuring the BRR register:

static uint32_t uart_config_baudrate(uint32_t pclk, uint32_t baud) {
    return ((pclk + (baud / 2U)) / baud);
}

    We wrap this into uart_set_baudrate() and call it inside uart_tx_init().

✅ Final USART2 TX Initialization

USART2->CR1 = (1U << 3); // Enable Transmitter
USART2->CR1 |= (1U << 13); // Enable USART

