#ifndef __FONTS_H__
#define __FONTS_H__

struct fonts_struct {
	unsigned char ascii_width;
	unsigned char ascii_height;
	unsigned char * ascii_code;	
	unsigned char ascii_beg;
	unsigned char ascii_end;
};

typedef enum
{
	FONT_FIVE_DOT,
   	FONT_COUNT=2
} FONT_BASE;

extern struct fonts_struct fonts[FONT_COUNT];
#endif
