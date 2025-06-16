/*
 * Switch.c
 *
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){
IOMUX->SECCFG.PINCM[PA15INDEX] = 0x00040081;
IOMUX->SECCFG.PINCM[PA16INDEX] = 0x00040081;
IOMUX->SECCFG.PINCM[PA17INDEX] = 0x00040081;
IOMUX->SECCFG.PINCM[PA18INDEX] = 0x00040081;
 
}
// return current state of switches
uint32_t Switch_In(void){
    // write this
uint32_t switchInput = (GPIOA->DIN31_0 & 0x078000)>>15;// reads the switches
return switchInput; // returns the state of the switch
}
