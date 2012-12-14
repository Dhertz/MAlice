#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

// ARM-specific memory sizes
#define PAGE_SIZE 4096
#define BLOCK_SIZE 4096

// GPIO controller physical address
#define GPIO_CONTROLLER 0x20200000

// GPIO controller virtual address
volatile unsigned *gpio;

void init_io() {
    int mem_fd;
    char *gpio_mem, *gpio_map;

    if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC) ) < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }
 
    if ((gpio_mem = malloc(BLOCK_SIZE + (PAGE_SIZE - 1))) == NULL) {
        printf("allocation error \n");
        perror("malloc");
        exit(EXIT_FAILURE);
    }
 
    // Align pointer to a 4K boundary
    if ((unsigned long) gpio_mem % PAGE_SIZE)
        gpio_mem += PAGE_SIZE - ((unsigned long) gpio_mem % PAGE_SIZE);
 
    gpio_map = (unsigned char*) mmap(
        (caddr_t) gpio_mem,
        BLOCK_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_FIXED,
        mem_fd,
        GPIO_CONTROLLER
    );
 
    if ((long) gpio_map < 0) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
 
    gpio = (volatile unsigned*) gpio_map;
}

void make_input(int pin) {
    // 32-bit block containing pin
    volatile unsigned *block = gpio + (pin / 10);

    // Pin's location in block
    int loc = (pin % 10) * 3;

    // 0b000 is the 3-bit number to make pin an input 
    *block &= ~(~0b000 << loc);
}

void make_output(int pin) {
    // Reset 3-bit configuration bits to 0b000
    make_input(pin);

    // 32-bit block containing pin
    volatile unsigned *block = gpio + (pin / 10);

    // Pin's location in block
    int loc = (pin % 10) * 3;

    // 0b001 is the 3-bit number to make pin an output
   *block |=  (0b001 << loc);  
}

void set_high(int pin) {
    // Sets bits which are 1 and ignores bits which are 0
    *(gpio + 7) = 1 << pin;
}

void set_low(int pin) {
    // Clears bits which are 1 and ignores bits which are 0
    *(gpio + 10) = 1 << pin;
}

unsigned int read_in(int pin) {
    // Returns the value of pin (high or low)
    return (*(gpio + 13) >> pin) & 0b001;
}

void short_wait() {
    // wait 100 clocks cycles for fun
    int w = 0;
    int i;
    for (i = 0; i < 100; ++i) {
        w++;
    }
}

void pull_up() {
    // enable pull=up on GPIO24 & 25
    *(gpio + 37) = 0b010;

    short_wait();
    // clock on GPIO 24 & 25
    *(gpio + 38) = 0b011 << 24;

    short_wait();
    // set them back to 0
    *(gpio + 37) = 0b000;
    *(gpio + 38) = 0b000;
}
