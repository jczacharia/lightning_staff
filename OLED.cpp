#include "OLED.h"
#include "ssdfont.h"
#include <Wire.h>

#if SSD1306_128_64 || SSD1306_128_32
const uint8_t initTable[] PROGMEM = {		// Initialization command sequence
	SSD1306_DISPLAYOFF,          	// 0xAE
	SSD1306_SETDISPLAYCLOCKDIV,  	// 0xD5
	0x80,                        	// the suggested ratio 0x80
	SSD1306_SETMULTIPLEX,        	// 0xA8
	#if SSD1306_128_64
	0x3F,
	#else
	0x1F,
	#endif
	SSD1306_SETDISPLAYOFFSET,    	// 0xD3
	0x0,                         	// no offset
	SSD1306_SETSTARTLINE | 0x0,  	// line #0
	SSD1306_CHARGEPUMP,          	// 0x8D
	0x14,
	SSD1306_MEMORYMODE,          	// 0x20
	0x00,			  	// was: 0x2 page mode
	SSD1306_SEGREMAP | 0x1,
	SSD1306_COMSCANDEC,
	SSD1306_SETCOMPINS,          	// 0xDA
	#if SSD1306_128_64
	0x12,
	#else
	0x02,
	#endif
	SSD1306_SETCONTRAST,         	// 0x81
	0xCF,
	SSD1306_SETPRECHARGE,         // 0xd9
	0xF1,
	SSD1306_SETVCOMDETECT,      	// 0xDB
	0x40,
	SSD1306_DISPLAYALLON_RESUME, 	// 0xA4
	SSD1306_NORMALDISPLAY,       	// 0xA6
	SSD1306_DISPLAYON		//--turn on oled panel
};

const uint8_t clearScreenTable[] PROGMEM = {	// Clear screen command sequence
	SSD1306_COLUMNADDR,
	0,				// Column start address (0 = reset)
	SSD1306_LCDWIDTH - 1,		// Column end address (127 = reset)
	SSD1306_PAGEADDR,
	0,				// Page start address (0 = reset)
	7				// Page end address
};
#endif

/***************************************/
/**************FUNCTIONS****************/
/***************************************/

void OLED::StartTransfer() {
	i2cCount = 0;
}

/***************************************/

inline void OLED::SelectCommand() { i2cControl = 0x00; }

/***************************************/

inline void OLED::SelectData() { i2cControl = 0x40; }

/***************************************/

inline void OLED::WriteData(uint8_t b) {
	if ((i2cCount == 0) || (i2cCount == (BUFFER_LENGTH - 1))) {
		i2cCount = 0;
		Wire.endTransmission();
		Wire.beginTransmission(SSD1306_I2C_ADDRESS);
		Wire.write(i2cControl);
	}
	Wire.write(b);
	i2cCount++;
}

/***************************************/

void OLED::EndTransfer() {
	if (!i2cIgnoreEnd)
	Wire.endTransmission();
}

/***************************************/

void OLED::I2CRestartTransfer() {
	Wire.endTransmission();
	i2cCount = 0;
}

void OLED::init() {
	col_ = 0;
	row_ = 0;
	textSize_ = 1;
	textSpacing_ = 1;
	//#if USE_2X_FONT || USE_3X_FONT || USE_8X_FONT || SCALED_FONTS
	//	transparentSpacing_ = true;
	//#endif

	Wire.begin();
	i2cIgnoreEnd = false;
	#ifdef TWBR_INIT_VALUE
	TWBR = TWBR_INIT_VALUE; // set I2C clock rate (assumed prescale)
	#endif

	// Send initialization command sequence
	StartTransfer();
	SelectCommand();
	for (uint8_t i = 0; i<sizeof(initTable); i++)
	WriteData(pgm_read_byte(initTable + i));
	EndTransfer();
}

void OLED::clear() {
	StartTransfer();
	SelectCommand();
	for (uint8_t i = 0; i<sizeof(clearScreenTable); i++)
	WriteData(pgm_read_byte(clearScreenTable + i));
	I2CRestartTransfer();
	SelectData();
	for (uint16_t i = 0; i<(SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT / 8); i++) {
		WriteData(0);
	}
	EndTransfer();
	col_ = 0;
	row_ = 0;
}

void OLED::setCursor(uint8_t row, uint8_t col) {
	row_ = row % (SSD1306_LCDHEIGHT / 8);
	col_ = col % SSD1306_LCDWIDTH;

	StartTransfer();
	SelectCommand();
	WriteData(SSD1306_SETLOWCOLUMN | (col_ & 0XF));
	WriteData(SSD1306_SETHIGHCOLUMN | (col_ >> 4));
	WriteData(SSD1306_SETSTARTPAGE | row_);
	EndTransfer();
}


//------------------------------------------------------------------------------
// write(c) - write a character at the current cursor location
//
// Standard font is 5x7.
//
// Support for larger characters is included.
// Scaling from 2-8X is one option.
// Custom fonts for the numeric characters (0-9) are optional for 2x, 3x, 8x.
//
// The cursor location is adjusted to where the next character would go,
// depending on the text size and text spacing
//------------------------------------------------------------------------------
size_t OLED::write(uint8_t c) {

	if (c == '\r') return 1;
	if (c == '\n') {
		setCursor(row_ + textSize_);
		return 1;
	}

	#if PROPORTIONAL_PUNCTUATION
	uint8_t slices = (((c == '.') || (c == ':')) ? 3 : 5);	// proportional spacing
	#else
	#define slices 5
	#endif

	// ----------------------------------------------------------------------------
	// Single height 5x7 font characters
	// ----------------------------------------------------------------------------
	if (textSize_ == 1) {		// dedicated code since it's faster than scaling
		
		
		col_ += 5;	// x5 font
		uint8_t *base = (uint8_t *)font + 5 * (c - 32);

		StartTransfer();
		SelectData();
		for (uint8_t i = 0; i < slices; i++) {
			uint8_t b = pgm_read_byte(base + i);
			WriteData(b);
		}
		for (uint8_t i = 0; i < textSpacing_; i++) {
			if (col_ >= SSD1306_LCDWIDTH) break;
			col_++;
			WriteData(0);	// textSpacing_ pixels of blank space between characters
		}
		EndTransfer();

		//col_ += 5;	// x5 font
		//uint8_t *base = (uint8_t *)font + 5 * (c - 32);

		//StartTransfer();
		//SelectData();
		//for (uint8_t i = 0; i < slices; i++) {
		//	uint8_t b = pgm_read_byte(base + i);
		//	WriteData(b);
		//}
		//for (uint8_t i = 0; i < textSpacing_; i++) {
		//	if (col_ >= SSD1306_LCDWIDTH) break;
		//	col_++;
		//	WriteData(0);	// textSpacing_ pixels of blank space between characters
		//}
		//EndTransfer();

		// ----------------------------------------------------------------------------
		// Larger fonts for numeric characters ('0' - '9')
		// ----------------------------------------------------------------------------
		#if USE_2X_FONT || USE_3X_FONT || USE_8X_FONT
	}
	else if (((c >= '0') && (c <= '9')) && (pgm_read_word(&fTbl[textSize_ - 2]))) {

		uint16_t ptr = pgm_read_word(pgm_read_word(&fTbl[textSize_ - 2]) + sizeof(uint16_t)*(c - '0'));
		uint8_t startRow = row_;
		uint8_t startCol = col_;
		uint8_t columns = textSize_ * 5;
		uint8_t rows = (textSize_ == 8 ? textSize_ - 1 : textSize_);
		uint8_t b, instruction, count, columnCnt, d;
		for (uint8_t irow = 0; irow < rows; irow++) {
			if (irow  > 0)
			setCursor(startRow + irow, startCol);
			StartTransfer();
			SelectData();
			uint8_t columnCnt = 0;
			while (columnCnt < columns) {
				b = pgm_read_byte(ptr++);
				instruction = b & INSTRUCTION_MASK;
				count = b & ~INSTRUCTION_MASK;
				if (instruction == _I) {
					for (uint8_t i = 0; i<count; i++)
					WriteData(pgm_read_byte(ptr++));
				}
				else {
					if (instruction == _0)
					d = 0;
					else if (instruction == _F)
					d = 0xFF;
					else // _S
					d = pgm_read_byte(ptr++);
					for (uint8_t i = 0; i<count; i++)
					WriteData(d);
				} //if else
				columnCnt += count;
			} // while columnCnt
			if (!transparentSpacing_)
			for (uint8_t i = 0; i<textSpacing_; i++)
			WriteData(0);
		} // for irow
		EndTransfer();
		setCursor(startRow, startCol + 5 * textSize_ + textSpacing_);
		#endif

		// ----------------------------------------------------------------------------
		// Scaled characters (up to 8X the original 5x7 font)
		// ----------------------------------------------------------------------------
	}
	#if SCALED_FONTS
	else {
		uint8_t sourceSlice, targetSlice, sourceBitMask, targetBitMask, extractedBit, targetBitCount;
		uint8_t startRow = row_;
		uint8_t startCol = col_;
		uint8_t irow8, sourceBitMaskInit, targetBitCountInit;

		for (uint8_t irow = 0; irow < textSize_; irow++) {
			irow8 = irow * 8;
			sourceBitMaskInit = 1 << (irow8 / textSize_);
			targetBitCountInit = (textSize_ * 7 - irow8) % textSize_;
			if (targetBitCountInit == 0) targetBitCountInit = textSize_;
			if (irow > 0)
			setCursor(startRow + irow, startCol);
			StartTransfer();
			SelectData();
			for (uint8_t iSlice = 0; iSlice<slices; iSlice++) {
				sourceSlice = pgm_read_byte(font + 5 * (c - 32) + iSlice);
				targetSlice = 0;
				targetBitMask = 0x01;
				sourceBitMask = sourceBitMaskInit;
				targetBitCount = targetBitCountInit;
				do {
					extractedBit = sourceSlice & sourceBitMask;
					for (uint8_t i = 0; i<textSize_; i++) {
						if (extractedBit != 0) targetSlice |= targetBitMask;
						targetBitMask <<= 1;
						targetBitCount--;
						if (targetBitCount == 0) {
							targetBitCount = textSize_;
							sourceBitMask <<= 1;
							break;
						}
						if (targetBitMask == 0) break;
					}
				} while (targetBitMask != 0);
				for (uint8_t i = 0; i<textSize_; i++) {
					WriteData(targetSlice);
				}
			}
			if (!transparentSpacing_)
			for (uint8_t i = 0; i<textSpacing_; i++)
			WriteData(0);
		}
		EndTransfer();
		setCursor(startRow, startCol + slices*textSize_ + textSpacing_);
	}
	#endif

	return 1;
}

//------------------------------------------------------------------------------
// write(*str) - write a string of characters
//------------------------------------------------------------------------------
int OLED::write(const char* str) {
	return write((const uint8_t *)str, strlen(str));
}

//------------------------------------------------------------------------------
// write(*buffer, size) - write multiple characters
//
// I2C is optimized in this routine to better utilize the Wire library buffer.
//------------------------------------------------------------------------------
size_t OLED::write(const uint8_t *buffer, size_t size) {
	#if I2C
	i2cIgnoreEnd = true;
	#endif
	for (size_t i = 0; i<size; i++) {
		char c = *buffer++;
		if (c == '\n') {
			#if I2C
			i2cIgnoreEnd = false;
			EndTransfer();
			#endif
			setCursor(row_ + textSize_);
			#if I2C
			i2cIgnoreEnd = true;
			#endif
		}
		else if (c != '\r')
		write((uint8_t)c);
	}
	#if I2C
	i2cIgnoreEnd = false;
	EndTransfer();
	#endif
	return size;
}

//------------------------------------------------------------------------------
// sendCommand(c) - send a single byte command
//------------------------------------------------------------------------------
void OLED::sendCommand(uint8_t c) {
	StartTransfer();
	SelectCommand();
	WriteData(c);
	EndTransfer();
}

//------------------------------------------------------------------------------
// sendData(c) - send a single byte of data; the cursor col is incremented
//
// This can be used to draw primitive pseudo-graphical elements on the screen
//------------------------------------------------------------------------------
void OLED::sendData(uint8_t c) {
	StartTransfer();
	SelectData();
	WriteData(c);
	EndTransfer();
	col_++;
}