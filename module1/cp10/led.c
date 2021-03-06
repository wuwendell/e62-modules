/*
 * Erich Woo & Wendell Wu
 * ENGS 62 Module 1
 * 9 Jan 2022
 */

#include "led.h"

static XGpio port;
static XGpioPs portPs;

/*
 * Initialize the led module
 */
void led_init(void) {
	// led0-3
	XGpio_Initialize(&port, XPAR_AXI_GPIO_0_DEVICE_ID);	/* initialize device AXI_GPIO_0 */
	XGpio_SetDataDirection(&port, CHANNEL1, OUTPUT);	    /* set tristate buffer to output */

	// led4
	XGpioPs_CfgInitialize(&portPs, XGpioPs_LookupConfig(XPAR_PS7_GPIO_0_DEVICE_ID), XPAR_PS7_GPIO_0_BASEADDR); // XPAR_PS7_GPIO_0_BASEADDR
	XGpioPs_SetDirectionPin(&portPs, MIO7, OUTPUT_PS);
	XGpioPs_SetOutputEnablePin(&portPs, MIO7, OP_EN);
}

/*
 * Set <led> to one of {LED_ON,LED_OFF,...}
 *
 * <led> is either ALL or a number >= 0
 * Does nothing if <led> is invalid
 */
void led_set(u32 led, bool tostate) {
	if (led == 4 || led == ALL) {
		XGpioPs_WritePin(&portPs, MIO7, tostate ? 0x1 : 0x0);
		if (led == 4) return;
	}

	u32 prev = XGpio_DiscreteRead(&port, CHANNEL1);
	u32 mask = (led == ALL) ? 0xF : (0x1 << led); // either all leds, bitshift 1 to the correct led

	if (tostate) mask |= prev; // OR with the previous bit states
	else mask = ~mask & prev;  // 0-out mask, then AND in order to 0 out the correct leds

	XGpio_DiscreteWrite(&port, CHANNEL1, mask);
}

/*
 * Get the status of <led>
 *
 * <led> is a number >= 0
 * returns {LED_ON,LED_OFF,...}; LED_OFF if <led> is invalid
 */
bool led_get(u32 led) {
	if (led == 4) return XGpioPs_ReadPin(&portPs, MIO7) == 0x1;

	u32 bits = XGpio_DiscreteRead(&port, CHANNEL1); // current state of port
	u32 mask = (0x1 << led); // used to read the specific bit
	return ((mask & bits) > 0) ? LED_ON : LED_OFF; // if was on, then value would be greater than 0
}

/*
 * Toggle <led>
 *
 * <led> is a value >= 0
 * Does nothing if <led> is invalid
 */
void led_toggle(u32 led) {
	if (led == 4) XGpioPs_WritePin(&portPs, MIO7,  0x1 ^ XGpioPs_ReadPin(&portPs, MIO7));
	else XGpio_DiscreteWrite(&port, CHANNEL1, (0x1 << led) ^ XGpio_DiscreteRead(&port, CHANNEL1));
}

