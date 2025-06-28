/*
 * LED.c
 *
 *  Created on: April 14th, 2025
 *      Author: Siddharth Deshpande and Akshay Karthik
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table


// initialize your LEDs
void LED_Init(void){

IOMUX->SECCFG.PINCM[PA25INDEX] = (uint32_t) 0x081; // set up software for LED 1
IOMUX->SECCFG.PINCM[PA26INDEX] = (uint32_t) 0x081; // set up software for LED 2
IOMUX->SECCFG.PINCM[PA27INDEX] = (uint32_t) 0x081; // set up software for LED 3
IOMUX->SECCFG.PINCM[PA28INDEX] = (uint32_t) 0x081; // set up software for LED 4
GPIOA->DOE31_0 |= 0x01F000000;
    
}
// data specifies which LED to turn on
void LED_On(uint32_t data){
    // write this
    // use DOUTSET31_0 register so it does not interfere with other GPIO
GPIOA->DOUTSET31_0 = data;

}

// data specifies which LED to turn off
void LED_Off(uint32_t data){
    // write this
    // use DOUTCLR31_0 register so it does not interfere with other GPIO
GPIOA->DOUTCLR31_0 = data; // clears all bits that correspond to LED lights in a friendly manner
}

// data specifies which LED to toggle
void LED_Toggle(uint32_t data){
    // write this
    // use DOUTTGL31_0 register so it does not interfere with other GPIO
GPIOA->DOUTTGL31_0 = data;
 
}
