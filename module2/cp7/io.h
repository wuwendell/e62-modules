/*
 * io.h -- switch and button module interface
 *
 */
#pragma once

#include <stdio.h>			/* printf for errors */
#include <stdbool.h>
#include <xgpio.h>		  	/* axi gpio */
#include "xparameters.h"  	/* constants used by the hardware */
#include "xil_types.h"		/* types used by xilinx */
#include "gic.h"			/* General Interrupt Controller module */

#define INPUT 1				/* Set direction of GPIO Port pins */
#define CHANNEL1 1			/* which channel of GPIO device */

/*
 * initialize the btns providing a callback
 */
void io_btn_init(void (*btn_callback)(void* btnAddress));

/*
 * close the btns
 */
void io_btn_close(void);


/*
 * initialize the switches providing a callback
 */
void io_sw_init(void (*sw_callback)(u32 switchAddress));

/*
 * close the switches
 */
void io_sw_close(void);

