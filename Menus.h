/*
 * Menus.h
 *
 *  Created on: Jul 8, 2017
 *      Author: Jeremy
 */

#ifndef MENUS_H_
#define MENUS_H_

#include "Arduino.h"
#include "OLED.h"
#include "MenuOutline.h"
#include <Wire.h>
//#include "OneButton.h"
#include "Rotary.h"
#include "FastLED.h"



//Rotary Actions
const byte ACTION_UP = 1;
const byte ACTION_DOWN = 2;
const byte ACTION_SELECT = 3;
const byte ACTION_BACK = 4;

//Screen Size
const byte SCREEN_CHAR_MAX_LENGTH_SIZE_1 = 21;
const byte SCREEN_CHAR_MAX_LENGTH_SIZE_2 = 10;

/*****************************************************/
/*****************************************************/
/*************************MENU************************/
/*****************************************************/
/*****************************************************/

class Menu {

public:

	Menu(const char * const * page, byte itemCount, byte selectedIndex)
	{
		_oled.init();
		_selectedIndex = selectedIndex;
		setCurrentMenu(page);
		_itemCount = itemCount;
	}

	virtual void ShowMenu() = 0;
	virtual byte MenuAction(byte action) = 0;

	byte _interr;
	byte _selectedIndex;

	void clearScreen() {
		_oled.clear();
	}

	virtual ~Menu() {
	}

	static byte getBrightness() { return BRIGHTNESS; }
	static void setBrightness(byte b) { BRIGHTNESS = b; }

	static const char * const * getCurrentMenu() { return CurrentMenu; }
	static void setCurrentMenu(const char * const * cm) { CurrentMenu = cm; }

	static const char * const * getMenuWithAni() { return MenuWithAni; }
	static void setMenuWithAni(const char * const * mwa) { MenuWithAni = mwa; }

	void rotateDown() {
		_oled.sendCommand(0xA1);
		_oled.sendCommand(0xB0);
		_oled.sendCommand(0xC8);
		ShowMenu();
	}

	void rotateUp() {
		_oled.sendCommand(0xA0);
		_oled.sendCommand(0xC0);
		ShowMenu();
	}


	//static const void SetBrightness(byte brightness) {
	//	_brightness = brightness;
	//}

private:
	static byte BRIGHTNESS;
	static const char * const * CurrentMenu;
	static const char * const * MenuWithAni;

protected:
	OLED _oled;
	byte _firstVisible;
	byte _itemCount;

	char tempBuffer[32];


	void prepend(char* s, const char* t);
};

/*****************************************************/
/*****************************************************/
/********************StartUpScreen********************/
/*****************************************************/
/*****************************************************/

//NOT INHERITED FROM MENU

class StartUpScreen {
public:
	StartUpScreen(uint16_t startUpDelay, char * version);
private:
	OLED _oled;
};

/*****************************************************/
/*****************************************************/
/*************************ROOT************************/
/*****************************************************/
/*****************************************************/

class RootMenu: public Menu {
public:
	
	RootMenu(const char * const * page, byte itemCount, byte selectedIndex) : Menu(page, itemCount, selectedIndex) {}

	void ShowMenu();

	byte MenuAction(byte action);

	~RootMenu() {}

};

/*****************************************************/
/*****************************************************/
/**********************Animation**********************/
/*****************************************************/
/*****************************************************/

class AnimationMenu : public Menu {
public:

	AnimationMenu(const char * const * page, byte itemCount, byte selectedIndex) : Menu(page, itemCount, selectedIndex) {}

	void ShowMenu();

	byte MenuAction(byte action);

	~AnimationMenu() {}

	const void setCurrentAnimation(byte selectedIndex) {
		_currentAni = selectedIndex;
	}
	boolean _isChangingAni;
	byte _currentAni;
	
private:
	byte _brightBuf;
	
};

/*****************************************************/
/*****************************************************/
/*********************Brightness**********************/
/*****************************************************/
/*****************************************************/

class BrightnessMenu : public Menu
{
public:

	BrightnessMenu(const char * const * page, byte itemCount, byte selectedIndex) : Menu(page, itemCount, selectedIndex) {}

	void ShowMenu();

	byte MenuAction(byte action);

private:
	byte _brightBuf;

};


/*****************************************************/
/*****************************************************/
/**********************Current************************/
/*****************************************************/
/*****************************************************/



#endif /* MENUS_H_ */
