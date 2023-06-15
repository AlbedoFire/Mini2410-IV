#ifndef __LED16X16_H__
#define	__LED16X16_H__

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u32;
typedef signed int s32;


extern u8 l_display_array[2*16];
extern u8 assic_buffer[3*16];

void char_out(u8 font, u8 *str);
void led_init(void);
void adc_test();

#endif
