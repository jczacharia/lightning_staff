//------------------------------------------------------------------------------
// File:          		Stotem.cpp
// Written By:   		Jeremy Zacharia
// Written For: 		Main .cpp file for Lightning Staff
//------------------------------------------------------------------------------

#include "ADXL345.h"
#include "Menus.h"


#include "AltSoftSerial.h"

AltSoftSerial altSerial;

#define BLUETOOTH
//#define DEV_MODE
#define SCREEN_SLEEP_AT_STARTUP

#ifdef SCREEN_SLEEP_AT_STARTUP
	#define INIT_SLEEPTIME 10000
#endif

void pciSetup(byte);
void selectedAction();
void Darkness();
void Lantern();
void Fire();
void RainbowGlitter();
void Pong();
void Clouds();
void WaveRacer();
void Flicker();
void Stack();
void FillLEDsFromPaletteColors(byte);
void addGlitter(fract8);
void rgORb();
void randomColors();
void Comet() ;
void Fade();
void Joker();
void p_1940s();
void Seizure();
void FunStuff(byte sel);
uint8_t Wheel(byte WheelPos);

#define ROT_DT	30
#define ROT_CK	29
#define ROT_SW	28
#define LS_SW	23
#define LS_DATA 28


ADXL345 adxl;

//Define Input Objects and Variables
Rotary r = Rotary(ROT_DT, ROT_CK);
//OneButton button(28, true);
byte interr = 0;


//
//Preprocessor constants
#define COLOR_ORDER GRB


//Pins

//Declare LED Objects
#define NUM_LEDS 55
CRGB leds[NUM_LEDS];
CRGBPalette16 gPal;
TBlendType currentBlending;

//Define LED Variables
byte animation = 1;
byte gHue = 0;
byte selectedItem = 0;

//Declare Menu Objects
RootMenu * rtmnu;
AnimationMenu * animnu;
BrightnessMenu * brtnsmnu;
//PowerMenu * powmnu;

//Define Static Menu Member Variables
byte Menu::BRIGHTNESS = 55;
const char * const * Menu::CurrentMenu = RootMenu_PROG;
const char * const * Menu::MenuWithAni = AnimationsMenu_PROG;

//Define User-Define Variable
byte SPARKING = 120;
byte COOLING = 80;
byte TOTAL_MENU_NUM = 3;
bool gReverseDirection = true;
byte FRAMES_PER_SECOND = 255;
byte cloudNumber = 3;
byte chanceOfGlitter = 80;
byte torchHue = 255;
bool cyclingTorchColor = false;
bool sleepScreen = false;
unsigned long sleepTime = 5000;
uint16_t sleepInt = 0;

//Stack Animation Variables
byte stack_colorIndex = 0;
byte stack_ch = NUM_LEDS;
byte stack_index = 0;

enum {FIRE = 1, LANTERN, FADE, ACIDRAIN, SKITTLES, JOKER, SEIZURE, WHITEWAVE, P_1940S, WONKA, SPECTRUM, RAINBOW, RAINBOWGLITTER, PONG, COMET, CLOUDS, WAVERACER, FLICKER, STACK, BLUETOOTH_RGB};
	
boolean exitloop = false;

void setup() {

	altSerial.begin(9600);
	altSerial.println("LIGHTING STAFF");

	pciSetup(ROT_DT);
	pciSetup(ROT_CK);
	pinMode(LS_SW, OUTPUT);
	pinMode(ROT_SW, INPUT_PULLUP);
	//Init LED Objects
	FastLED.addLeds<WS2811, LS_DATA, GRB>(leds, NUM_LEDS);
	gPal = HeatColors_p;
	LEDS.setBrightness(Menu::getBrightness());



	adxl.powerOn();
	

	//Bluetooth Settings
	//Serial.write("AT+NAMEStotem v1.3.1");
	//delay(500);
	/*Use this baudrate if using for Arduino Uno, Bluino and Mega2560*/
	//Serial.println("AT+UART=115200,0,0");
	/*Use this baudrate if using for Arduino Nano, Leonardo, Micro, Pro Mini 3V3/5V and Duemilanove */
	/*
	Serial.println("AT+UART=57600,0,0");
	delay(500);
	Serial.println("AT+POLAR=1,0");
	delay(500);
	*/



	//Init Root Menu
	animnu = new AnimationMenu(AnimationsMenu_PROG, cnt_AnimationsMenu_PROG, 1);
	animnu->setCurrentAnimation(animation);
	animnu->ShowMenu();


	#ifdef SCREEN_SLEEP_AT_STARTUP
		sleepScreen = true;
		sleepInt = INIT_SLEEPTIME;
		sleepTime = millis();
	#endif

	//Setup Input
	//button.attachClick(selectedAction);
	sei();

}

//Button Clicked
//void selectedAction() {
	//interr = ACTION_SELECT;
//}

void pciSetup(byte pin)
{
	*digitalPinToPCMSK(pin) |= bit(digitalPinToPCMSKbit(pin));  // enable pin
	PCIFR |= bit(digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
	PCICR |= bit(digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

//Interrupt for Rotary Encoder
ISR(PCINT0_vect) {
	sleepTime = millis();
	sleepScreen = true;
	//button.tick();
	unsigned char result = r.process();	
	
	animnu->_isChangingAni = !digitalRead(ROT_SW);
	
	exitloop = true;
	
	if (result == DIR_NONE)
	{
		// do nothing
	}
	else if (result == DIR_CCW)
	{
		interr = ACTION_UP;
	}
	else if (result == DIR_CW)
	{
		interr = ACTION_DOWN;
	}
}

String readString;
byte rotated = true;

#ifdef BLUETOOTH
enum { ANIMATION_PARAM = 1, BRIGHTNESS_PARAM, LED_DIRECTION_PARAM, TORCH_PARAM, CLOUD_NUM_PARAM, FPS_PARAM, RAINBOW_PARAM, SLEEP_SCREEN_PARAM, RGB_SET_PARAM };
#endif

byte red_color;
byte green_color;
byte blue_color;

void loop()
{
	
	int x_acel, y_acel, z_acel;
	adxl.readXYZ(&x_acel, &y_acel, &z_acel); //read the accelerometer values and store them in variables  x,y,z
											 // Output x,y,z values
	if (x_acel < -150 && !rotated) {
		animnu->rotateUp();
		gReverseDirection = false;
		rotated = true;
	}
	else if (x_acel > 150 && rotated) {
		animnu->rotateDown();
		gReverseDirection = true;
		rotated = false;
	}

	if (sleepScreen) {
		if (millis() - sleepTime > sleepInt) {
			animnu->clearScreen();
			sleepScreen = false;
		}
	}
	
	
	if(animnu->_selectedIndex != animnu->_currentAni){
		//if(digitalRead(ROT_SW)){
			interr = ACTION_SELECT;
			//animnu->_isChangingAni = false;
		//}
	}
	
	

	//button.tick();
	
	#ifdef BLUETOOTH
	
		byte state;
		byte aniIndex;
		byte value;
		byte last_color;

		if (altSerial.available())
		{
			state = altSerial.read();
			value = altSerial.read();
			aniIndex = altSerial.read();
			last_color = altSerial.read();

			//State is the type of action. e.g. Brightness, Animation
			/*byte state = ble.read();


			//Value is the state's value. e.g. brightness = 55, Animation = 3
			//So Value would be 55 or 3
			byte value = ble.read();

			byte aniIndex = ble.read();

			Serial.println();
			Serial.println(state);
			Serial.println(value);
			Serial.println(aniIndex);
			/*

			*MAX 256 states*
			State 1: Animation
			State 2: Brightness
			State 3: LED Direction
			State 4: Torch COOLING & Torch SPARKING
			State 5: Cloud Number
			State 6: FPS
			State 7: Rainbow Glitter Chance
			State 8: Sleep Screen
			*/


			/*Serial.println();
			Serial.println(state);
			Serial.println(value);
			Serial.println(aniIndex);
			Serial.println(last_color);*/

			switch (state)
			{
			default:
				state = 0;
				break;
			case ANIMATION_PARAM:
				animation = value;
				Darkness();
				//stack_colorIndex = 0;
				//stack_ch = NUM_LEDS;
				//stack_index = 0;
				if (Menu::getCurrentMenu() == AnimationsMenu_PROG)
				{
					animnu->setCurrentAnimation(animation);
					animnu->ShowMenu();
				}
				break;

			case BRIGHTNESS_PARAM:
				Menu::setBrightness(value);
				LEDS.setBrightness(value);
				if (Menu::getCurrentMenu() == BrightnessMenu_PROG)
				{
					brtnsmnu->ShowMenu();
				}
				break;

			case LED_DIRECTION_PARAM:
				gReverseDirection = (value == 1) ? true : false;
				break;

			case TORCH_PARAM:

				switch (value)
				{
				default:
				value = 0;
				break;
				case 1:
					COOLING = aniIndex;
					break;
				case 2:
					SPARKING = aniIndex;
					break;
				case 3:
					if (aniIndex == 0) {
						cyclingTorchColor = true;
					}
					else if (aniIndex == 255) {
						cyclingTorchColor = false;
						torchHue = 255;
					}
					else {
						torchHue = aniIndex;
						cyclingTorchColor = false;
					}
					break;
				}
				break;

			case CLOUD_NUM_PARAM:
				cloudNumber = value;
				break;

			case FPS_PARAM:
				FRAMES_PER_SECOND = value;
				break;

			case RAINBOW_PARAM:
				chanceOfGlitter = value;
				break;

			case SLEEP_SCREEN_PARAM:
				if (value == 0) {
					sleepScreen = false;
					interr = ACTION_DOWN;
				}
				else
				{
					sleepScreen = true;
					sleepTime = millis();
					sleepInt = value * 1000;
				}
				break;

			case RGB_SET_PARAM:
				animation = 10;
				red_color = value;
				green_color = aniIndex;
				blue_color = last_color;
				break;
			}
			readString = "";
			if(animation != BLUETOOTH_RGB)
			{
				int diff = animnu->_selectedIndex - animation;
				Serial.println(diff);
				animnu->_isChangingAni = true;
				if(diff < 0){
					for(int i = 0; i < abs(diff); i++)
					{
						animnu->MenuAction(ACTION_UP);
					}
				}
				else if (diff > 0)
				{
					for(int i = 0; i < diff; i++)
					{
						animnu->MenuAction(ACTION_DOWN);
					}
				}
				animnu->_isChangingAni = false;
			}
		}
		else if (interr > 0)
	#endif
	#ifndef BLUETOOTH
		if (interr > 0)
	#endif
	
	{

		/*****************************************************/
		/*****************************************************/
		/*************************ROOT************************/
		/*****************************************************/
		/*****************************************************/

		//if (Menu::getCurrentMenu() == RootMenu_PROG) {
			//switch (interr) {
			//case ACTION_DOWN:
				//rtmnu->MenuAction(ACTION_DOWN);
				//break;
			//case ACTION_UP:
				//rtmnu->MenuAction(ACTION_UP);
				//break;
			//case ACTION_SELECT:
				//selectedItem = rtmnu->MenuAction(ACTION_SELECT);
				//break;
			//}
//
			//switch (selectedItem)
			//{
			//case 1:
				//delete rtmnu;
				//animnu = new AnimationMenu(AnimationsMenu_PROG, cnt_AnimationsMenu_PROG, 2);
				//animnu->setCurrentAnimation(animation + 1);
				//animnu->ShowMenu();
				//break;
			//case 2:
				//delete rtmnu;
				//brtnsmnu = new BrightnessMenu(BrightnessMenu_PROG, cnt_BrightnessMenu_PROG, Menu::getBrightness());
				//brtnsmnu->ShowMenu();
				//break;
				///*case 3:
				//delete rtmnu;
				//powmnu = new PowerMenu(PowerMenu_PROG, cnt_PowerMenu_PROG, 1);
				//powmnu->beginINA();
				//powmnu->ShowMenu();
				//break;*/
			//}
		//}

		/*****************************************************/
		/*****************************************************/
		/**********************Animation**********************/
		/*****************************************************/
		/*****************************************************/

		if (Menu::getCurrentMenu() == AnimationsMenu_PROG) {
			switch (interr) {
			case ACTION_DOWN:
				animnu->MenuAction(ACTION_DOWN);
				break;
			case ACTION_UP:
				animnu->MenuAction(ACTION_UP);
				break;
			case ACTION_SELECT:
				selectedItem = animnu->MenuAction(ACTION_SELECT);
			break;
			}
			afterble:
			LEDS.setBrightness(Menu::getBrightness());
			for (byte i = 1; i <= cnt_AnimationsMenu_PROG; i++)
			{
				//button.tick();
				if (i==selectedItem)
				{
					//if (i == 1)
					//{
						//delete animnu;
						//rtmnu = new RootMenu(RootMenu_PROG, cnt_RootMenu_PROG, 1);
						//rtmnu->ShowMenu();
						//break;
					//}
					//show = 0;
					animation = i;
					animnu->setCurrentAnimation(animnu->_selectedIndex);
					animnu->setMenuWithAni(Menu::getCurrentMenu());
					animnu->ShowMenu();
					break;
					//Serial.println(animation);
				}
			}
		}
		/*****************************************************/
		/*****************************************************/
		/*********************Brightness**********************/
		/*****************************************************/
		/*****************************************************/

		//else if (Menu::getCurrentMenu() == BrightnessMenu_PROG)
		//{
			//switch (interr) {
				//case ACTION_DOWN:
				//brtnsmnu->MenuAction(ACTION_DOWN);
				//break;
				//case ACTION_UP:
				//brtnsmnu->MenuAction(ACTION_UP);
				//break;
				//case ACTION_SELECT:
				//selectedItem = brtnsmnu->MenuAction(ACTION_SELECT);
				//break;
			//}
			//LEDS.setBrightness(Menu::getBrightness());
			//switch (selectedItem)
			//{
				//case 1:
				//delete brtnsmnu;
				//rtmnu = new RootMenu(RootMenu_PROG, cnt_RootMenu_PROG, 1);
				//rtmnu->ShowMenu();
				//break;
			//}
//
		//}

		/*****************************************************/
		/*****************************************************/
		/**********************Current************************/
		/*****************************************************/
		/*****************************************************/

		/*else if (Menu::getCurrentMenu() == PowerMenu_PROG)
		{
		switch (interr) {
		case ACTION_DOWN:
		powmnu->MenuAction(ACTION_DOWN);
		break;
		case ACTION_UP:
		powmnu->MenuAction(ACTION_UP);
		break;
		case ACTION_SELECT:
		selectedItem = powmnu->MenuAction(ACTION_SELECT);
		break;
		}
		switch (selectedItem)
		{
		case 1:
		delete powmnu;
		rtmnu = new RootMenu(RootMenu_PROG, cnt_RootMenu_PROG, 1);
		rtmnu->ShowMenu();
		break;
		}

		}*/
		
		if (interr == ACTION_SELECT) {
			//adxl.readXYZ(&x_acel, &y_acel, &z_acel); //read the accelerometer values and store them in variables  x,y,z
													 //// Output x,y,z values
			//if (x_acel < -150) {
				//rtmnu->rotateDown();
				//gReverseDirection = true;
			//}
			//else if (x_acel > 150) {
				//rtmnu->rotateUp();
				//gReverseDirection = false;
			//}
			//else {
//
			//}
			stack_colorIndex = 0;
			stack_ch = NUM_LEDS;
			stack_index = 0;
			Darkness();
		}
		interr = 0;
		selectedItem = 0;
	}

	/*****************************************************/
	/*****************************************************/
	/******************LED Animations*********************/
	/*****************************************************/
	/*****************************************************/
	
	
	
	//button.tick();
	if (Menu::getBrightness() < 1)
	{
		digitalWrite(LS_SW, LOW);
	}
	else
	{
		digitalWrite(LS_SW, HIGH);
	}
	switch (animation)
	{
	default:
	Darkness();
	digitalWrite(LS_SW, LOW);
	break;
	case FIRE:
		Fire();
		break;
	case LANTERN:
		Lantern();
		break;
	case FADE:
		Fade();
		FastLED.delay(50);
		break;
	case COMET:
		Comet();
		break;
	case RAINBOWGLITTER:
		RainbowGlitter();
		break;
	case SPECTRUM:
		rgORb();
		break;
	case RAINBOW:
		FunStuff(0);
		break;
	case ACIDRAIN:
		FunStuff(1);
		break;
	case SKITTLES:
		FunStuff(2);
		break;
	case JOKER:
		FunStuff(3);
		break;
	case SEIZURE:
		FunStuff(4);
		break;
	case WHITEWAVE:
		FunStuff(5);
		break;
	case P_1940S:
		FunStuff(6);
		break;
	case WONKA:
		FunStuff(7);
		break;
	case PONG:
		Pong();
		break;
	case CLOUDS:
		Clouds();
		break;
	case WAVERACER:
		WaveRacer();
		break;
	case FLICKER:
		Flicker();
		break;
	case STACK:
		gPal = RainbowColors_p;
		currentBlending = LINEARBLEND;
		Stack();
		break;
	case BLUETOOTH_RGB:
		fill_solid(leds,NUM_LEDS, CRGB(red_color, green_color, blue_color));
		break;
	}
	EVERY_N_MILLISECONDS(20) { gHue++; }
	FastLED.show();
	FastLED.delay(1000 / FRAMES_PER_SECOND);
}

/*****************************************************/
/*****************************************************/
/******************LED Animations*********************/
/*****************************************************/
/*****************************************************/

void Darkness()
{
	for (int i = 0; i < NUM_LEDS; i++) {
		leds[i] = 0;
	}
}

void Fire() {
	// Add entropy to random number generator; we use a lot of it..


	// Fourth, the most sophisticated: this one sets up a new palette every
	// time through the loop, based on a hue that changes every time.
	// The palette is a gradient from black, to a dark color based on the hue,
	// to a light color based on the hue, to white.
	//

	if (cyclingTorchColor)
	{
		static byte hue = 0;
		hue++;
		CRGB darkcolor = CHSV(hue, 255, 192); // pure hue, three-quarters brightness
		CRGB lightcolor = CHSV(hue, 128, 255); // half 'whitened', full brightness
		gPal = CRGBPalette16(CRGB::Black, darkcolor, lightcolor, CRGB::White);
	}
	else if (torchHue == 255)
	{
		gPal = HeatColors_p;
		random16_add_entropy(random());
	}
	else
	{
		CRGB darkcolor = CHSV(torchHue, 255, 192); // pure hue, three-quarters brightness
		CRGB lightcolor = CHSV(torchHue, 128, 255); // half 'whitened', full brightness
		gPal = CRGBPalette16(CRGB::Black, darkcolor, lightcolor, CRGB::White);
	}



	// Array of temperature readings at each simulation cell
	static byte heat[NUM_LEDS];

	// Step 1.  Cool down every cell a little
	for (int i = 0; i < NUM_LEDS; i++) {
		heat[i] = qsub8(heat[i], random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
	}

	// Step 2.  Heat from each cell drifts 'up' and diffuses a little
	for (int k = NUM_LEDS - 1; k >= 2; k--) {
		heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
	}

	// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
	if (random8() < SPARKING) {
		int y = random8(7);
		heat[y] = qadd8(heat[y], random8(160, 255));
	}

	// Step 4.  Map from heat cells to LED colors
	for (int j = 0; j < NUM_LEDS; j++) {
		// Scale the heat value from 0-255 down to 0-240
		// for best results with color palettes.
		byte colorindex = scale8(heat[j], 240);
		CRGB color = ColorFromPalette(gPal, colorindex);
		int pixelnumber;
		if (gReverseDirection) {
			pixelnumber = (NUM_LEDS - 1) - j;
		}
		else {
			pixelnumber = j;
		}
		leds[pixelnumber] = color;
	}
}

void Lantern()
{
	fill_solid(leds, NUM_LEDS, CRGB::White);
}

void Pong()
{
	fadeToBlackBy(leds, NUM_LEDS, 20);
	int pos = beatsin16(13, 0, NUM_LEDS);
	leds[pos] += CHSV(gHue, 255, 192);
}

void Clouds()
{
	gPal = CloudColors_p;
	currentBlending = LINEARBLEND;
	static byte startIndex = 0;
	startIndex = startIndex + 1;
	FillLEDsFromPaletteColors(startIndex);
}

void WaveRacer()
{
	// eight colored dots, weaving in and out of sync with each other
	fadeToBlackBy(leds, NUM_LEDS, 20);
	byte dothue = 0;
	for (int i = 0; i < 8; i++) {
		leds[beatsin16(i + 7, 0, NUM_LEDS)] |= CHSV(dothue, 200, 255);
		dothue += 32;
	}
}

void Joker(){
		CRGB purple = CHSV( HUE_PURPLE, 255, 255);
		CRGB green  = CHSV( HUE_GREEN, 255, 255);
		CRGB black  = CRGB::Black;
		
		gPal = CRGBPalette16(
		green,  green,  black,  black,
		purple, purple, black,  black,
		green,  green,  black,  black,
		purple, purple, black,  black );
		
}

void p_1940s() {
	fill_solid( gPal, 16, CRGB::Black);
	// and set every fourth one to white.
	gPal[0] = CRGB::White;
	gPal[4] = CRGB::White;
	gPal[8] = CRGB::White;
	gPal[12] = CRGB::White;
	
}

void Seizure()
{
	for( int i = 0; i < 16; i++) {
		gPal[i] = CHSV( random8(), 255, random8());
	}
	FastLED.delay(50);
}


void FunStuff(byte sel){
	if( sel == 0)  { gPal = RainbowColors_p;         currentBlending = LINEARBLEND; }
	if( sel == 1)  { gPal = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
	if( sel == 2)  { gPal = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
	if( sel == 3)  { Joker();						 currentBlending = LINEARBLEND; }
	if( sel == 4)  { Seizure();					     currentBlending = LINEARBLEND; }
	if( sel == 5)  { p_1940s();						 currentBlending = NOBLEND; }
	if( sel == 6)  { p_1940s();				         currentBlending = LINEARBLEND; }
	if( sel == 7)  { gPal = PartyColors_p;           currentBlending = LINEARBLEND; }
	static byte startIndex = 0;
	startIndex = startIndex + 1;
	FillLEDsFromPaletteColors(startIndex);
	FastLED.delay(10);
}


void Flicker()
{
	// random colored speckles that blink in and fade smoothly
	fadeToBlackBy(leds, NUM_LEDS, 10);
	int pos = random16(NUM_LEDS);
	leds[pos] += CHSV(gHue + random8(64), 200, 255);
}

void FillLEDsFromPaletteColors(byte colorIndex)
{
	if (gReverseDirection)
	{
		for (byte i = 0; i < NUM_LEDS; i++) {
			leds[i] = ColorFromPalette(gPal, colorIndex, Menu::getBrightness(), currentBlending);
			colorIndex += cloudNumber;
		}
	}
	else
	{
		for (byte i = 0; i < NUM_LEDS; i++) {
			leds[(NUM_LEDS - 1) - i] = ColorFromPalette(gPal, colorIndex, Menu::getBrightness(), currentBlending);
			colorIndex += cloudNumber;
		}
	}
}

void fadeall() { for (int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

/*void Comet()
{
static byte hue = 0;
// First slide the led in one direction
for (int i = 0; i < NUM_LEDS; i++) {
// Set the i'th led to red
leds[i] = CHSV(hue++, 255, 255);
// Show the leds
FastLED.show();
// now that we've shown the leds, reset the i'th led to black
// leds[i] = CRGB::Black;
fadeall();
// Wait a little bit before we loop around and do it again
delay(10);
button.tick();

}


// Now go in the other direction.
for (int i = (NUM_LEDS)-1; i >= 0; i--) {
// Set the i'th led to red
leds[i] = CHSV(hue++, 255, 255);
// Show the leds
FastLED.show();
// now that we've shown the leds, reset the i'th led to black
// leds[i] = CRGB::Black;
fadeall();
// Wait a little bit before we loop around and do it again
delay(10);
button.tick();
}
}*/

void RainbowGlitter()
{
	fill_rainbow(leds, NUM_LEDS, gHue, 7);
	addGlitter(chanceOfGlitter);
}

void addGlitter(fract8 chanceOfGlitter)
{
	if (random8() < chanceOfGlitter) {
		leds[random16(NUM_LEDS)] += CRGB::White;
	}
}

void Stack()
{
	if (stack_index == stack_ch)
	{
		stack_ch--;
		stack_colorIndex += 4;
		stack_index = 0;
	}
	if (!gReverseDirection) {
		leds[(NUM_LEDS - 1) - stack_index - 1] = ColorFromPalette(gPal, stack_colorIndex, 100, currentBlending);
		leds[(NUM_LEDS - 1) - stack_index] = 0;
	}
	else {
		leds[stack_index] = ColorFromPalette(gPal, stack_colorIndex, 100, currentBlending);
		leds[stack_index - 1] = 0;
	}
	stack_index++;
	if (stack_ch == 0) {
		stack_colorIndex = 0;
		stack_ch = NUM_LEDS;
	}
}

void rgORb()
{
	for(uint16_t i = 0; i < NUM_LEDS; i++) 
	{
		uint8_t ran = random(0,3);
		if (ran == 1)
		{
			leds[i] = CRGB(255,0,0);
		}
		else if (ran == 2)
		{
			leds[i] = CRGB(0,255,0);
		}
		else
		{
			leds[i] = CRGB(0,0,255);
		}
		FastLED.show();
		//FastLED.delay(1000 / FRAMES_PER_SECOND);
	}
}

void randomColors()
{
	for(uint16_t i = 0; i < NUM_LEDS; i++)
	{
		leds[i] = CRGB(random(0,255), random(0,255), random(0,255));
		FastLED.show();
		//FastLED.delay(1000 / FRAMES_PER_SECOND);
	}
}

void Comet() 
{
	static uint8_t hue = 0;
	// First slide the led in one direction
	for(int i = 0; i < NUM_LEDS; i++) {
		// Set the i'th led to red
		leds[i] = CHSV(hue++, 255, 255);
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		// leds[i] = CRGB::Black;
		fadeall();
		// Wait a little bit before we loop around and do it again
		delay(10);
		if(exitloop){
			exitloop=false;
			goto bailout;
		}
	} 

	// Now go in the other direction.
	for(int i = (NUM_LEDS)-1; i >= 0; i--) {
		// Set the i'th led to red
		leds[i] = CHSV(hue++, 255, 255);
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		// leds[i] = CRGB::Black;
		fadeall();
		// Wait a little bit before we loop around and do it again
		delay(10);
		if(exitloop){
			exitloop=false;
			goto bailout;
		}
	}
	bailout:;
}

void Fade() {
	CHSV hsv;
	hsv.hue = gHue;
	hsv.val = 255;
	hsv.sat = 240;
	for( int i = 0; i < NUM_LEDS; i++) {
		leds[i] = hsv;
	}
}

uint8_t Wheel(byte WheelPos) {
	if(WheelPos < 85) {
		return WheelPos * 3;
	} else if(WheelPos < 170) {
		WheelPos -= 85;
		return 255 - WheelPos * 3;
	} else {
		WheelPos -= 170;
		return 0;
	}
}