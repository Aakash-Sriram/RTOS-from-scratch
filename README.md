# RTOS From Scratch

This repo is just a part of my RTOS journey — focused specifically on drivers (LED, UART, and Timebase).

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

🔨 Implementing uart_write()

To actually transmit characters, we implemented a basic UART transmit function:

static void uart_write(int ch) {
    while (!(USART2->SR & (1U << 7))); // Wait until TXE (Transmit Data Register Empty)
    USART2->DR = (ch & 0xFF);          // Write data to the data register
}

🧠 Explanation:

    USART2->SR & (1U << 7) checks the TXE flag — it tells us when the UART is ready to accept new data for transmission.

    The loop ensures we don’t overwrite previous data.

    The data is masked with 0xFF to ensure only 8 bits are written (one byte at a time).

📤 Redirecting printf() — __io_putchar()

To seamlessly use printf() for debugging:

int __io_putchar(int ch) {
    uart_write(ch);
    return ch;
}

This links printf() to our uart_write() under the hood. Most STM32 toolchains call __io_putchar() when printf() is used — this makes debugging much easier, especially when writing a custom RTOS.

🧪 Testing the TX Line

With everything set up, you can now do:

printf("Hello from RTOS!\r\n");

...and see the message on your serial terminal (like PuTTY or minicom), confirming UART TX is alive 


⏱️ Timebase (SysTick Timer Interrupts)

To bring timing capabilities to our RTOS, I added a timebase using the SysTick Timer, which generates 1-second periodic interrupts using the system clock.

📚 Reference Used:


🧠 Why Use SysTick?

SysTick is a built-in 24-bit down counter that's part of every Cortex-M core. It's ideal for periodic interrupts, like ticking the RTOS scheduler (think round-robin or time slicing).

🔍 Register Breakdown:

    LOAD: Reload value for 1s delay (16 million for 16 MHz)

    VAL: Clears the current counter to start fresh

    CTRL: Sets clock source, enables interrupt, and starts the timer

🧨 Key Flags:

    TICKINT: Enables the SysTick interrupt.

    ENABLE: Starts the counter.

    CLKSOURCE: Chooses processor clock (AHB) over external clock.

    COUNTFLAG: Can be polled to detect underflow (optional use).[It's automatically set to 1 by hardware whenever the timer counts down to 0]


Added a basic SysTick timebase to generate a tick interrupt every 1 second.



    The timer_variable increments every SysTick interrupt (configured for 1 second interval).

    timebase_stop() disables the SysTick timer by clearing the enable bit.

    This simple driver can be used as a basic timer tick for scheduling or other timing operations in the RTOS.






### 1-UART-driver
  Renode simulation for the driver has been simulated and verified 
  refer RENODE docs for Installation
  ## For setup in RENODE  
    mach create 
    machine LoadPlatformDescription @platforms/boards/stm32f4_discovery-kit.repl 
    (its a preconfigured stm32f4 discovery board available in renod)
    sysbus LoadELF @/path_to_the_elf_file/1-UART-driver.elf
    showAnalyzer sysbus.usart2
    start
![alt text](image.png)
