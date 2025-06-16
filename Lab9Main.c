// Lab9Main.c
// Runs on MSPM0G3507
// Lab 9 ECE319K
// Your name: Siddharth Deshpande and Akshay Karthik
// Last Modified: 04/22/2025

//#include <cstdio>
//#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/ADC1.h"
#include "../inc/DAC5.h"
#include "../inc/Arabic.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "images/images.h"

#include "Sprites/fishy.txt"
#include "Sprites/piranha.txt"
#include "Sprites/croc.txt"
#include "Sprites/player.txt"

#include "Screens/background.txt"
#include "Screens/endingScreen.txt"
#include "Screens/startingScreen.txt"

#include "Structs/spriteStructs.h"



// globals
uint32_t sample; // mailbox variable that collects ADCin
uint32_t buttonPressed; // mailbox variable that collects switch_in
uint32_t sema = 0; // semaphore
uint32_t range = 15; // range variable for collisions
uint32_t spriteOffset = 55; // second range variable
uint32_t playerOffset = 10; // third range variable
uint32_t lang; // defines the index which chooses the language


// starter text array
const char* startText[] = 
{
"Press Up to Start",
"Pressione Up\n   Para Iniciar"
};


// language option array
const char* languageOption[] = 
{
"English >",
"< Portugu\x88s"
};

// Game over! text array
const char* endingText[] = 
{
"Game Over!",
"O jogo acabou!"
};

// score display array
const char* scoreMessage[] = 
{
"Your score is:",
"Sua pontua\x87""ao \x82:"
};

// points display array 
const char* pointsDisplay[] = 
{
"Points: ",
"Pontos: "
};

// lives display array
const char* livesDisplay[] = 
{
"Lives: ",
"Vidas: "
};

// restart option array
const char* restart[] = 
{
"Press the Reset \nButton to \nstart over",
"Pressione o Botão \nReset para \nrecomen\x87""ar:"
};

// image pointer array
const unsigned short *imageArr[] = 
{
  fishyImage, crocodileImage, piranhaImage
};



// header files for RandomSprite generator functions
uint32_t RandomSprite(void);
uint32_t RandomHeight(void);


// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void){ // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}

Arabic_t ArabicAlphabet[]={
alif,ayh,baa,daad,daal,dhaa,dhaal,faa,ghayh,haa,ha,jeem,kaaf,khaa,laam,meem,noon,qaaf,raa,saad,seen,sheen,ta,thaa,twe,waaw,yaa,zaa,space,dot,null
};
Arabic_t Hello[]={alif,baa,ha,raa,meem,null}; // hello
Arabic_t WeAreHonoredByYourPresence[]={alif,noon,waaw,ta,faa,raa,sheen,null}; // we are honored by your presence
int main0(void){ // main 0, demonstrate Arabic output
  Clock_Init80MHz(0);
  LaunchPad_Init();
  ST7735_InitR(INITR_REDTAB);
  ST7735_FillScreen(ST7735_WHITE);
  Arabic_SetCursor(0,15);
  Arabic_OutString(Hello);
  Arabic_SetCursor(0,31);
  Arabic_OutString(WeAreHonoredByYourPresence);
  Arabic_SetCursor(0,63);
  Arabic_OutString(ArabicAlphabet);
  while(1){
  }
}
uint32_t M=1;
uint32_t Random32(void){
  M = 1664525*M+1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32()>>16)%n;
}


// games  engine runs at 30Hz
void TIMG12_IRQHandler(void){uint32_t pos,msg;
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
// game engine goes here
    // 1) sample slide pot
    sample = ADCin();
    // 2) read input switches
    buttonPressed = Switch_In();
    // set semaphore to 1
    sema = 1;
    // NO LCD OUTPUT IN INTERRUPT SERVICE ROUTINES
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
  }
}
uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}

typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Language_Portuguese[]="Portugu\x88s";
const char Language_French[]="Fran\x87" "ais";
const char *Phrases[3][4]={
  {Hello_English,Hello_Spanish,Hello_Portuguese,Hello_French},
  {Goodbye_English,Goodbye_Spanish,Goodbye_Portuguese,Goodbye_French},
  {Language_English,Language_Spanish,Language_Portuguese,Language_French}
};
// use main1 to observe special characters
int main1(void){ // main1
    char l;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(0x0000);            // set screen to black
  for(phrase_t myPhrase=HELLO; myPhrase<= GOODBYE; myPhrase++){
    for(Language_t myL=English; myL<= French; myL++){
         ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
      ST7735_OutChar(' ');
         ST7735_OutString((char *)Phrases[myPhrase][myL]);
      ST7735_OutChar(13);
    }
  }
  Clock_Delay1ms(3000);
  ST7735_FillScreen(0x0000);       // set screen to black
  l = 128;
  while(1){
    Clock_Delay1ms(2000);
    for(int j=0; j < 3; j++){
      for(int i=0;i<16;i++){
        ST7735_SetCursor(7*j+0,i);
        ST7735_OutUDec(l);
        ST7735_OutChar(' ');
        ST7735_OutChar(' ');
        ST7735_SetCursor(7*j+4,i);
        ST7735_OutChar(l);
        l++;
      }
    }
  }
}

// use main2 to observe graphics
int main2(void){ // main2
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  
  ST7735_DrawBitmap(22, 159, PlayerShip0, 18,8); // player ship bottom
  ST7735_DrawBitmap(53, 151, Bunker0, 18,5);
  ST7735_DrawBitmap(42, 159, PlayerShip1, 18,8); // player ship bottom
  ST7735_DrawBitmap(62, 159, PlayerShip2, 18,8); // player ship bottom
  ST7735_DrawBitmap(82, 159, PlayerShip3, 18,8); // player ship bottom
  ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
  ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
  ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
  ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16,10);
  ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);
  
  //ST7735_FillScreen(ST7735_BLACK);
  //ST7735_DrawBitmap(0, 159, background, 128, 160); // 
  //ST7735_DrawBitmap(50, 80, fishy, 30, 30); // fish


  for(uint32_t t=500;t>0;t=t-5){
    SmallFont_OutVertical(t,104,6); // top left
    Clock_Delay1ms(50);              // delay 50 msec
  }
  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Earthling!");
  ST7735_SetCursor(2, 4);
  ST7735_OutUDec(1234);
  while(1){
  }
  
}

// use main3 to test switches and LEDs
int main3(void){ // main3
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  while(1){
    // write code to test switches and LEDs
    
  }
}
// use main4 to test sound outputs
int main4(void){ uint32_t last=0,now;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(ADC0,6,0); // ADC1 channel 6 is PB20, TExaS scope
  __enable_irq();
  while(1){
    now = Switch_In(); // one of your buttons
    if((last == 0)&&(now == 1)){
      Sound_Piranha(); // call one of your sounds
    }
    if((last == 0)&&(now == 2)){
      Sound_Killed(); // call one of your sounds
    }
    if((last == 0)&&(now == 4)){
      Sound_Fish(); // call one of your sounds
    }
    if((last == 0)&&(now == 8)){
      Sound_Game_Over(); // call one of your sounds
    }
    // modify this to test all your sounds
  }
}

// ALL ST7735 OUTPUT MUST OCCUR IN MAIN
int main(void){ // final main
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  ADCinit();     //PB18 = ADC1 channel 5, slidepot
  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
    // initialize interrupts on TimerG12 at 30 Hz
  TimerG12_IntArm(80000000/30,2);
  // initialize all data structures
  __enable_irq();

  // startings the loading screen
  ST7735_DrawBitmap(0, 159, startingScreen, 128, 160);



int j; // delay logic for loop index
uint32_t spriteIndex; // index that chooses the sprite
uint32_t flag = 0; // flag for the language
uint32_t adcData; // variable to collect the reversed ADC data
uint32_t conversion = 26; // comversion factor for adc data to lcd values
uint32_t offset = 15; // offset for the player

sprite_t player;
player.image = playerImage;
player.width = 50; // defines the width of the player
player.height = 88; // defines the height of the player
player.lives = 2; // player has 2 lives to start
player.points = 0; // player begins with 0 points
player.x = 78; // player has a constant x position of 78

// array of sprite objects defines the three different sprites
sprite_t allSprites[3] = {
  {fishyImage, 30, 31, 0, 10}, // defines the image pointer, width, height, life value, and point value of a fish
  {crocodileImage, 46, 28, -2, 0}, // defines the image pointer, width, height, life value, and point value of a crocodile
  {piranhaImage, 37, 33, -1, 0} // defines the image pointer, width, height, life value, and point value of a piranha
};

// while loop which controls the starting screen

while (1)
{
  while (sema == 0)
  {} // does nothing until the semaphore is set
  sema = 0; // sets the semaphore back to 0

  // default starting screen
  if (buttonPressed == 0 && flag == 0)// checks flag to see if english is selected
  {
  lang = 0;
  ST7735_DrawBitmap(0, 159, startingScreen, 128, 160);
  ST7735_SetCursor(2, 10);
  ST7735_SetTextColor(ST7735_WHITE);
  printf("%s", startText[lang]);
  ST7735_SetCursor(5, 13);
  printf("%s", languageOption[lang]);
  }

  // screen to show Portuguese option
  else if (buttonPressed == 1)
  {
  lang = 1; // sets language to Portuguese
  flag = 1; // flag becomes 1 to indicate Portuguese
  ST7735_DrawBitmap(0, 159, startingScreen, 128, 160);
  ST7735_SetCursor(3, 10);
  ST7735_SetTextColor(ST7735_WHITE);
  printf("%s", startText[lang]);
  ST7735_SetCursor(4, 13);
  printf("%s", languageOption[lang]);
  
  }

  if (buttonPressed == 4) // tests if the option is switched back to English
  {
    flag = 0; // sets flag back to 0
    lang = 0;   //// CHANGE
  }

  else if (buttonPressed == 8) // checks if the user wants to star the game
    {
      break; // breaks the starter while loop and begins the game
    }
  }


// turns on specific LEDS
  LED_On(1<<28);
  LED_On(1<<27);
  LED_On(1<<26);

 // sets the screen to blue, which is the river
  ST7735_FillScreen(ST7735_BLUE);

  // starts the game
  while (1){
  while (sema == 0) 
   {}
  sema = 0;
  // generates random sprites and random positions of sprites
  spriteIndex = RandomSprite();
  allSprites[spriteIndex].y = RandomHeight();

  // checks for a crocodile
  if (spriteIndex == 1)
    {
      LED_On(1<<25);
      LED_On(1<<8);
    }
  else 
  {
    LED_Off(1<<25);
    LED_Off(1<<8);
  }

  // moves the sprite
  for (allSprites[spriteIndex].x = 0; allSprites[spriteIndex].x < 128; allSprites[spriteIndex].x+=2)
  {  
    ST7735_SetTextColor(ST7735_WHITE);
    ST7735_SetCursor(0,0);
    printf("%s%d",pointsDisplay[lang],player.points);
    ST7735_SetCursor(0,1);
    printf("%s%d",livesDisplay[lang],player.lives);
    adcData = 4096 - sample;
    player.y = (adcData / conversion) + 55;
    ST7735_DrawBitmap(player.x, player.y, player.image, player.width, player.height);
    for (j = 0; j < 100000; j++)
    {} // delay logic
    // draws the sprite
    ST7735_DrawBitmap(allSprites[spriteIndex].x, allSprites[spriteIndex].y, allSprites[spriteIndex].image, allSprites[spriteIndex].width, allSprites[spriteIndex].height);
    // checks for collisions
    if ((((player.x + offset) <= (allSprites[spriteIndex].x + allSprites[spriteIndex].width + playerOffset)) && 
    ((player.x + offset) >= (allSprites[spriteIndex].x + allSprites[spriteIndex].width - playerOffset)))    &&   
    (((allSprites[spriteIndex].y - range) >= (player.y - spriteOffset)) && ((allSprites[spriteIndex].y + range) <= player.y)))
    {
      if (spriteIndex == 0 || spriteIndex == 2) // fish or piranha
      {
        if (buttonPressed == 2) // is cathc button pressed?
        {
          while (buttonPressed == 2)
          {} // debouncing
          if (buttonPressed == 0)
          {
            if (spriteIndex == 0) // checks fort a fish
            {
              Sound_Fish();
              LED_Off(1<<28);
              for (j = 0; j < 1000000; j++)
              {}
              LED_On(1<<28);

            } 
            // transfers lives and points
            player.lives += allSprites[spriteIndex].lives;
            player.points += allSprites[spriteIndex].points;
            if (player.lives == 1)
            {
              LED_Off(1<<27); // turns off the rightmost red LED light to indicate that 1 life is gone
            }
          }
        }
      }
      else // if the sprite is a crocodile
      {
      // transfers lives and points
      player.lives += allSprites[spriteIndex].lives;
      player.points += allSprites[spriteIndex].points;
      }

      // checks if the player is dead
      if (player.lives <= 0)
      {
        LED_Off(1<<26); // turns off LED light to indicate life gone
        LED_Off(1<<27); // turns off LED light to indicate life gone
        Sound_Killed();
        ST7735_SetCursor(0,1);
        printf("%s 0",livesDisplay[lang]);
        break; // exits for loop
      }
    }
  }
  if (player.lives <= 0)      /////(((player.x + offset) == (i + allSprites[spriteIndex].width)) && (((spritePos - range) >= (player.y - spriteOffset)) && ((spritePos + range) <= player.y)))
  {
    break; // exits the playing screen  
  }
}


for (j = 0; j < 10000000; j++)
  {} // delay logic between death of the player and game over screen

// turns off LEDS
LED_Off(1<<25);
LED_Off(1<<28);
// draws the ending screen
ST7735_DrawBitmap(0, 159, endScreen, 128, 160);

// displays all ending messages
ST7735_SetCursor(0, 3);
ST7735_SetTextColor(ST7735_WHITE);
printf("%s", endingText[lang]);
ST7735_SetCursor(0, 5);
printf("%s %d",scoreMessage[lang], player.points);
ST7735_SetCursor(0, 8);
printf("%s",restart[lang]);
}


// function to get the random sprite index
uint32_t RandomSprite(void)
{
  return ((Random32()>>24)%3);  // returns 0, 1, 2
}

// function to get a random y positions for the sprite
uint32_t RandomHeight(void)
{ 
  return ((Random32()>>24)%155) + 10;  // returns 1, 2, 3, 4, or 5
}