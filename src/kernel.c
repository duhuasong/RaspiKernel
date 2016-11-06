#include <stdint.h>
#include "kernel.h"

static inline void set32(volatile uintptr_t address, volatile uint32_t value) {
	*((uintptr_t*) address) = value;
}

static inline uint32_t get32(volatile uintptr_t address) {
	return *((uintptr_t*) address);
}

static inline void set64(volatile uintptr_t address, volatile uint64_t value) {
	*((uintptr_t*) address) = value;
}

static inline uint64_t get64(volatile uintptr_t address) {
	return *((uintptr_t*) address);
}

static inline void sleep_us(volatile uint64_t time) {
	volatile uint64_t goal = get64(TIMER_COUNTER) + time;
	while (get64(TIMER_COUNTER) < goal);

	return;
}

static inline void sleep_s(float time) {
	sleep_us(time * 1E6);
}

static void initGPIO(int gpio) {
	uintptr_t address = GPIO_ADDR;
	address += gpio / 10 * 4;

	set32(address, 1 << ((gpio % 10) * 3));
}

static void setGPIO(int gpio, int value) {
	int job;
	if (value) job = GPIO_SET1;
	else job = GPIO_CLR1;

	set32(GPIO_ADDR + job, 1 << gpio);
}

void kernel(uint32_t r0, uint32_t r1, uint32_t atags) {
	// Init GPIO pin
	// set32(GPIO_ADDR + GPIO_ACT_BYTE_SET, 
	// 	1 << (GPIO_ACT_PIN_OFFS * GPIO_BITS_PER_PIN));
	// initGPIO(2);
	set32(GPIO_ADDR + 0x4, 1 << (3 * 6));
	set32(GPIO_ADDR + GPIO_CLR0, 1 << 16);

	// Blink ACT light
	while (1) {
		// setGPIO(2, 1);
		set32(GPIO_ADDR + GPIO_CLR1, 1 << 16);
		sleep_s(0.1);

		// setGPIO(2, 0);
		set32(GPIO_ADDR + GPIO_SET1, 1 << 16);
		sleep_s(0.9);
	}

	return;
}

