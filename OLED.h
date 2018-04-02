#ifndef _OLED_H_
#define _OLED_H_

#include "Arduino.h"
#include "Print.h"

// Define to set TWBR in init() (for I2C mode only)
// If commented out, TWBR is not set in init()
#define TWBR_INIT_VALUE 12	// I2C clock: 400 KHz

// Set to true to allow scaling of the 5x7 fonts up to 8X in size
#define SCALED_FONTS 1

// Set to 1 to use large fonts for numeric characters '0'-'9'
#define USE_2X_FONT 1
#define USE_3X_FONT 1
#define USE_8X_FONT 0

// Define to proportionally space "." and ":" characters
#define PROPORTIONAL_PUNCTUATION 1

#define SSD1306_128_64 1
#define SSD1306_128_32 0

//------------------------------------------------------------------------------

#if SSD1306_128_64
#define SSD1306_LCDWIDTH  128
#define SSD1306_LCDHEIGHT  64
#endif

#if SSD1306_128_32
#define SSD1306_LCDWIDTH  128
#define SSD1306_LCDHEIGHT  32
#endif

#define SSD1306_I2C_ADDRESS   0x3C	// 011110+SA0+RW - 0x3C or 0x3D

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22

#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_SETSTARTPAGE 0XB0
#define SSD1306_MEMORYMODE 0x20

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

class OLED : public Print
{
	public:
	OLED() {}
	~OLED() {}

	void init();
	void clear();
	void setCursor(uint8_t row, uint8_t col = 0);
	void setTextSize(uint8_t size) { setTextSize(size, size); }
	void setTextSize(uint8_t size, uint8_t spacing) { textSize_ = size; textSpacing_ = spacing; }
	size_t write(uint8_t c);
	int write(const char* str);
	size_t write(const uint8_t *buffer, size_t size);
	void sendCommand(uint8_t c);
	void sendData(uint8_t c);
	private:
	void StartTransfer();
	void SelectCommand();
	void SelectData();
	void WriteData(uint8_t b);
	void EndTransfer();
	void I2CRestartTransfer();
	int8_t col_, row_;		// cursor position
	uint8_t textSize_, textSpacing_;	// text size and horiz char spacing (pixels between)
	int8_t data_, clk_;		// OLED pins
	uint8_t i2cCount, i2cControl, i2cIgnoreEnd;
	#if USE_2X_FONT || USE_3X_FONT || USE_8X_FONT || SCALED_FONTS
	bool transparentSpacing_;	// do not clear pixels between large characters
	#endif
};

#endif