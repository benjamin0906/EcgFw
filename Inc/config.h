/*
 * config.h
 *
 *  Created on: Oct 18, 2021
 *      Author: Benjamin
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define ADAS_DRDY   PortA_3
#define ADAS_CS     PortA_4
#define ADAS_SCLK   PortA_5
#define ADAS_SDO    PortA_6
#define ADAS_SDI    PortA_7
#define ADAS_PD     PortC_4
#define ADAS_RESET  PortC_5
#define ADAS_CLKIO  PortB_1

#define SUPPLY_VOLTAGE      33
#define USB_RX_BUFFER_SIZE  127 //This needs to be less by 1 than a power of 2

#endif /* CONFIG_H_ */
