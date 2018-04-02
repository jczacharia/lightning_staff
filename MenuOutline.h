#include <avr/pgmspace.h>


/******** PROGMEM ************/
//CONSTANTS



const char RootDash_PROG[] PROGMEM = "/";
const char Dashes_PROG[] PROGMEM = "---------------------";
const char BACK_PROG[] PROGMEM = "<- BACK";
const char RootMenu_Item1_PROG[] PROGMEM = "/Animations";
const char RootMenu_Item2_PROG[] PROGMEM = "/Brightness";
const char RootMenu_Item3_PROG[] PROGMEM = "/LED Power";
const char * const RootMenu_PROG[] PROGMEM = { RootDash_PROG, Dashes_PROG, RootMenu_Item1_PROG, RootMenu_Item2_PROG };
const byte cnt_RootMenu_PROG PROGMEM = 2;

//const char SubMenu_Animation1_PROG[] PROGMEM = "Darkness";
const char SubMenu_TORCH[] PROGMEM = "Torch";
const char SubMenu_LANTERN[] PROGMEM = "Lantern";
const char SubMenu_FADE[] PROGMEM = "Fade";
const char SubMenu_ACIDRAIN[] PROGMEM = "Acid Rain";
const char SubMenu_SKITTLES[] PROGMEM = "Skittles";
const char SubMenu_JOKER[] PROGMEM = "Joker";
const char SubMenu_SEIZURE[] PROGMEM = "Seizure";
const char SubMenu_WHITEWAVE[] PROGMEM = "White Wave";
const char SubMenu_P_1940S[] PROGMEM = "1940sPsychedelicRush";
const char SubMenu_WONKA[] PROGMEM = "Wonka";
const char SubMenu_SPECTRUM[] PROGMEM = "Spectrum";
const char SubMenu_RAINBOW[] PROGMEM = "Rainbow";
const char SubMenu_RAINBOWGLITTER[] PROGMEM = "Rainbow Glitter";
const char SubMenu_PONG[] PROGMEM = "Pong";
const char SubMenu_COMET[] PROGMEM = "Comet";
const char SubMenu_CLOUDS[] PROGMEM = "Clouds";
const char SubMenu_WAVERACER[] PROGMEM = "Wave Racer";
const char SubMenu_FLICKER[] PROGMEM = "Flicker";
const char SubMenu_STACK[] PROGMEM = "Stack";
const char * const AnimationsMenu_PROG[] PROGMEM =
{ Dashes_PROG, 
SubMenu_TORCH, 
SubMenu_LANTERN, 
SubMenu_FADE,
SubMenu_ACIDRAIN,
SubMenu_SKITTLES,
SubMenu_JOKER,
SubMenu_SEIZURE,
SubMenu_WHITEWAVE,
SubMenu_P_1940S,
SubMenu_WONKA,
SubMenu_SPECTRUM,
SubMenu_RAINBOW,
SubMenu_RAINBOWGLITTER,
SubMenu_PONG, 
SubMenu_COMET, 
SubMenu_CLOUDS,
SubMenu_WAVERACER,
SubMenu_FLICKER, 
SubMenu_STACK };
const byte cnt_AnimationsMenu_PROG PROGMEM = 19;

//IS ALSWAYS AMOUNT OF ANIS


const char * const BrightnessMenu_PROG[] PROGMEM =
{ RootMenu_Item2_PROG, Dashes_PROG, BACK_PROG };

const byte cnt_BrightnessMenu_PROG PROGMEM = 1;


const char REFRESH_PROG[] PROGMEM = "REFRESH";

//const char * const PowerMenu_PROG[] PROGMEM =
//{ RootMenu_Item3_PROG, Dashes_PROG, BACK_PROG };
//
//const byte cnt_PowerMenu_PROG PROGMEM = 1;
