/*
 * Menus.cpp
 *
 *  Created on: Jul 8, 2017
 *      Author: Jeremy
 */

#include "Menus.h"

/*****************************************************/
/*****************************************************/
/*************************MENU************************/
/*****************************************************/
/*****************************************************/

void Menu::prepend(char* s, const char* t) {
	size_t len = strlen(t);
	size_t i;

	memmove(s + len, s, strlen(s) + 1);

	for (i = 0; i < len; ++i)
		s[i] = t[i];
}

/*****************************************************/
/*****************************************************/
/********************StartUpScreen********************/
/*****************************************************/
/*****************************************************/

StartUpScreen::StartUpScreen(uint16_t startUpDelay, char * version) {
	_oled.init();
	_oled.clear();
	_oled.setTextSize(2);
	_oled.write(" LIGHTNING");
	_oled.setCursor(2, 1);
	_oled.write("   STAFF");
	_oled.setTextSize(1);
	_oled.setCursor(5, 1);
	_oled.write(version);
	_oled.setCursor(6, 1);
	_oled.write("    Copyright (C)");
	_oled.setCursor(7, 1);
	_oled.write("   Jeremy Zacharia");
	delay(startUpDelay);
}

/*****************************************************/
/*****************************************************/
/*************************ROOT************************/
/*****************************************************/
/*****************************************************/


void RootMenu::ShowMenu() {
	_oled.clear();
	_oled.setCursor(0, 0);
	strcpy_P(tempBuffer, (char*) pgm_read_word(&(getCurrentMenu()[0])));
	_oled.write(tempBuffer);
	_oled.setCursor(1, 0);
	strcpy_P(tempBuffer, (char*)pgm_read_word(&(getCurrentMenu()[1])));
	_oled.write(tempBuffer);
	if (_selectedIndex - 1 > _firstVisible + 5) {
		_firstVisible = _selectedIndex - 1 - 5;
	} else if (_selectedIndex - 1 < _firstVisible) {
		_firstVisible = _selectedIndex - 1;
	}
	byte p = 5;
	if (p > (_itemCount - _firstVisible + 1)) {
		p = _itemCount - _firstVisible + 1;
	}
	for (byte i = 0; i <= p; i++) {
		_oled.setCursor(i + 2, 0);
		strcpy_P(tempBuffer,
			(char*)pgm_read_word(&(getCurrentMenu()[i + _firstVisible + 2])));
		if (i == _selectedIndex - 1 - _firstVisible) {
			prepend(tempBuffer, ">");
			_oled.write(tempBuffer);
		} else if (i < _itemCount) {
			prepend(tempBuffer, " ");
			_oled.write(tempBuffer);
		}
		if (i == _itemCount - 1) {
			break;
		}
	}
}

byte RootMenu::MenuAction(byte action) {
	if (action == ACTION_UP) {
		_selectedIndex++;
	}
	if (action == ACTION_DOWN) {
		_selectedIndex--;
	}
	if (_selectedIndex > _itemCount) {
		_selectedIndex = 1;
	}
	if (_selectedIndex < 1) {
		_selectedIndex = _itemCount;
	}
	if (action == ACTION_SELECT) {
		return _selectedIndex;
	}
	ShowMenu();
}

/*****************************************************/
/*****************************************************/
/**********************Animation**********************/
/*****************************************************/
/*****************************************************/


void AnimationMenu::ShowMenu() {
	_oled.clear();
	//_oled.setCursor(0, 0);
	//strcpy_P(tempBuffer, (char*)pgm_read_word(&(getCurrentMenu()[0])));
	//_oled.write(tempBuffer);
	
	_brightBuf = map(getBrightness(), 0, 255, 0, SCREEN_CHAR_MAX_LENGTH_SIZE_1);
	_oled.setCursor(0, 0);
	strcpy(tempBuffer, "");
	for (byte i = 0; i < _brightBuf; i++)
	{
		strcat(tempBuffer, "#");
	}
	_oled.write(tempBuffer);
	
	_oled.setCursor(1, 0);
	strcpy_P(tempBuffer, (char*)pgm_read_word(&(getCurrentMenu()[0])));
	_oled.write(tempBuffer);
	if (_selectedIndex - 1 > _firstVisible + 5) {
		_firstVisible = _selectedIndex - 1 - 5;
	}
	else if (_selectedIndex - 1 < _firstVisible) {
		_firstVisible = _selectedIndex - 1;
	}
	byte p = 5;
	if (p > (_itemCount - _firstVisible + 1)) {
		p = _itemCount - _firstVisible + 1;
	}
	for (byte i = 0; i <= p; i++) {
		_oled.setCursor(i + 2, 0);
		strcpy_P(tempBuffer,
			(char*)pgm_read_word(&(getCurrentMenu()[i + _firstVisible + 1])));
		if (getMenuWithAni() == getCurrentMenu()
			&& i == _currentAni - 1 - _firstVisible) {
			prepend(tempBuffer, "`");
			_oled.write(tempBuffer);
		}
		else if (i == _selectedIndex - 1 - _firstVisible) {
			prepend(tempBuffer, ">");
			_oled.write(tempBuffer);
		}
		else if (i < _itemCount) {
			prepend(tempBuffer, " ");
			_oled.write(tempBuffer);
		}
		if (i == _itemCount - 1) {
			break;
		}
	}
	/*Serial.print("selected index: ");
	Serial.println(_selectedIndex);
	Serial.print("first visible: ");
	Serial.println(_firstVisible);*/
}

byte AnimationMenu::MenuAction(byte action) {
	
	if(_isChangingAni){
		if (action == ACTION_UP) {
			_selectedIndex++;
		}
		if (action == ACTION_DOWN) {
			_selectedIndex--;
		}
		if (_selectedIndex > _itemCount) {
			_selectedIndex = 1;
		}
		if (_selectedIndex < 1) {
			_selectedIndex = _itemCount;
		}
		if (action == ACTION_SELECT) {
			return _selectedIndex;
		}
	}
	else
	{
		if (action == ACTION_DOWN) {
			if (getBrightness() <= 12) {
				setBrightness(0);
				} else {
				setBrightness(getBrightness() - 12);
			}
		}
		if (action == ACTION_UP) {
			if (getBrightness() > 243) {
				setBrightness(255);
				} else {
				setBrightness(getBrightness() + 12);
			}
		}
	}
	ShowMenu();

}

/*****************************************************/
/*****************************************************/
/*********************Brightness**********************/
/*****************************************************/
/*****************************************************/


void BrightnessMenu::ShowMenu()
{
	//_oled.clear();
	//_oled.setCursor(0, 0);
	//_oled.setTextSize(1);
	//strcpy_P(tempBuffer, (char*)pgm_read_word(&(getCurrentMenu()[0])));
	//_oled.write(tempBuffer);
	//_oled.setCursor(1, 0);
	//strcpy_P(tempBuffer, (char*)pgm_read_word(&(getCurrentMenu()[1])));
	//_oled.write(tempBuffer);
//
	//_brightBuf = map(getBrightness(), 0, 255, 0, SCREEN_CHAR_MAX_LENGTH_SIZE_1);
	//int brightBufPerCent = map(getBrightness(), 0, 255, 0, 100);
	//char buf[4];
	//sprintf(buf, "%03i", brightBufPerCent);
	//strcpy(tempBuffer, "%");
	//prepend(tempBuffer, buf);
	//if (brightBufPerCent < 100)
	//{
		//tempBuffer[0] = ' ';
	//}
	//_oled.setCursor(2, 0);
	//_oled.write(tempBuffer);
//
	//_oled.setCursor(4, 0);
	//strcpy(tempBuffer, "");
//
	//for (byte i = 0; i < _brightBuf; i++)
	//{
		//strcat(tempBuffer, "#");
	//}
	//_oled.write(tempBuffer);
	//_oled.setCursor(5, 0);
	//strcpy_P(tempBuffer, (char*)pgm_read_word(&(getCurrentMenu()[1])));
	//_oled.write(tempBuffer);
	//_oled.setCursor(7, 0);
	//strcpy_P(tempBuffer, (char*)pgm_read_word(&(getCurrentMenu()[2])));
	//prepend(tempBuffer, ">");
	//_oled.write(tempBuffer);
	////_oled.write(tempBuffer);
	///*Serial.print("selected index: ");
	//Serial.println(_selectedIndex);*/
	
	
	
	

	//_oled.write(tempBuffer);
	/*Serial.print("selected index: ");
	Serial.println(_selectedIndex);*/
}

byte BrightnessMenu::MenuAction(byte action) {
	if (action == ACTION_DOWN) {
		if (getBrightness() <= 12) {
			setBrightness(0);
		} else {
			setBrightness(getBrightness() - 12);
		}
	}
	if (action == ACTION_UP) {
		if (getBrightness() > 243) {
			setBrightness(255);
		} else {
			setBrightness(getBrightness() + 12);
		}
	}
	if (action == ACTION_SELECT) {
		return 1;
	}
	ShowMenu();

	return 0;
}

/*****************************************************/
/*****************************************************/
/**********************Current************************/
/*****************************************************/
/*****************************************************/
