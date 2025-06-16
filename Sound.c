// Sound.c
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// your name
// your data 



//#include <cstdint>
#include <stdint.h> 
#include <ti/devices/msp/msp.h>  
#include "Sound.h" 
#include "sounds/sounds.h" 
#include "../inc/DAC5.h" 
#include "../inc/Timer.h" 
#include "../inc/Timer.h"
#include "SoundEffects/killed.txt"
#include "SoundEffects/fishSound.txt"
#include "SoundEffects/piranhaSound.txt"
#include "SoundEffects/gameOver.txt"

const uint8_t *wave;
uint32_t counter;
uint32_t idx = 0;

void SysTick_IntArm(uint32_t period, uint32_t priority){
  SysTick->CTRL = 0x00; // disable during initialization
  SysTick->LOAD = period - 1; // set reload register
  SCB->SHP[1]   = (SCB->SHP[1]&(~0xC0000000))|priority<<30; // set priority (bits 31,30)
  SysTick->VAL = 0; // clear count, cause reload
  SysTick->CTRL = 0x07; // Enable SysTick IRQ and SysTick Timer
}

// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5-bit DAC
void Sound_Init(void){
DAC5_Init();          // Port B is DAC
SysTick->CTRL = 0;         // disable SysTick during setup
SysTick->LOAD = 80000000/11000 - 1;  // reload value
SysTick->VAL = 0;          // any write to current clears it
SCB->SHP[1] = SCB->SHP[1]&(~0xC0000000)|0x40000000; // set priority = 1
SysTick->CTRL = 0x0007;    // enable SysTick with core clock and interrupts

}


void SysTick_Handler(){ // called at 11 kHz
  // output one value to DAC if a sound is active
  DAC5_Out(wave[idx]); // calls DAC5_Out() with the value in our sin wave
  idx++; // increments the global index and wraps back if needed
  if (idx >= counter)
  {
    SysTick->LOAD = 0;
  }

}

//******* Sound_Start ************
// This function does not output to the DAC. 
// Rather, it sets a pointer and counter, and then enables the SysTick interrupt.
// It starts the sound, and the SysTick ISR does the output
// feel free to change the parameters
// Sound should play once and stop
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement
void Sound_Start(const uint8_t *pt, uint32_t count)
{
  idx = 0;
  counter = count;
  SysTick->LOAD = 7256;
  SysTick->CTRL = 0x07;   
  wave = pt;

}

// sound when player is killed
void Sound_Killed(void)
{
  Sound_Start(killed, 11520);
}

// sound when the user catches a piranha
void Sound_Piranha(void)
{
  Sound_Start(piranhaSound, 3526);
}

// sound when the user catches a fish
void Sound_Fish(void)
{
  Sound_Start(fishSound, 6656);
}

// sound for when the game ends
void Sound_Game_Over(void)
{
  Sound_Start(gameOver, 12684);
}

