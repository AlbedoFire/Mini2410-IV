/*********************************************************************************************
* File：	lcdapp.c
* Author:	Embest	
* Desc：	LCD_Test
* History:	
*********************************************************************************************/

/*------------------------------------------------------------------------------------------*/
/*	 								include files						 				    */
/*------------------------------------------------------------------------------------------*/
#include <string.h>
#include "2410lib.h"
#include "lcdlib.h"
#include "glib.h"
#include "lcd.h"
#include <math.h>
#include "fonts.h"
#include "led16x16.h"

/*------------------------------------------------------------------------------------------*/
/*	 								constant define							 			    */
/*------------------------------------------------------------------------------------------*/
#define LCD_BUF_SIZE		(SCR_XSIZE_TFT_640480*SCR_YSIZE_TFT_640480/2)
#define LCD_ACTIVE_BUFFER	(0x33800000)
#define LCD_VIRTUAL_BUFFER  (0x33800000 + LCD_BUF_SIZE)
#define LCD_D_OFF 			rGPCDAT &= ~(1 << 4); 
#define LCD_D_ON 			rGPCDAT |= (1 << 4); 

/*------------------------------------------------------------------------------------------*/
/*	 								extern variables						 			    */
/*------------------------------------------------------------------------------------------*/
#ifdef BOARDTEST_EXH
#undef BOARDTEST
#endif	

#ifndef BOARDTEST	
//extern UINT8T g_ucBitmap[][76800];

extern UINT8T g_ucAscii8x16[];
extern UINT8T g_auc_Ascii6x8[];
extern UINT8T g_auc_hzk24[];
#endif
 
/*------------------------------------------------------------------------------------------*/
/*	 								function declare                                        */
/*------------------------------------------------------------------------------------------*/
void Lcd_DspAscII6x8(UINT16T x0, UINT16T y0, UINT16T ForeColor, UINT8T * s);
void Lcd_DspAscII8x16(UINT16T x0, UINT16T y0, UINT16T ForeColor, UINT8T * s);
//void Lcd_DspHz12(UINT16T x0, UINT16T y0, UINT16T ForeColor, UINT8T *s);
//void Lcd_DspHz16(UINT16T x0, UINT16T y0, UINT16T ForeColor, UINT8T *s);
void Lcd_DspHz24(UINT16T x0, UINT16T y0, UINT16T ForeColor, UINT8T *s);
//void Lcd_DspHz24s(UINT16T x0, UINT16T y0, UINT16T ForeColor, UINT8T *s);
void Lcd_Draw_HLine(INT16T usX0, INT16T usX1, INT16T usY0, UINT16T ucColor, UINT16T usWidth);
void Lcd_Draw_VLine (INT16T usY0, INT16T usY1, INT16T usX0, UINT16T ucColor, UINT16T usWidth);
void lcd_clr(void);
extern void Lcd_port_init(void);
extern void Lcd_Port_Return(void);

/*********************************************************************************************
* name:		lcd_init_app
* func:		lcd application initialization code 
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_init_app()
{
    Lcd_port_init();
    Lcd_Init(MODE_TFT_16BIT_800480);
    Glib_Init(MODE_TFT_16BIT_800480);
    Glib_ClearScr(0, MODE_TFT_16BIT_800480);
    Lcd_PowerEnable(0, 1);
    Lcd_EnvidOnOff(1);	  
    uart_printf(" [TFT 64K COLOR(16bit/1pixel) LCD TEST]\n");

}

/*********************************************************************************************
* name:		lcd_init_app_end
* func:		end lcd application initialization
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_init_app_end()
{
    Lcd_EnvidOnOff(0);
    Lcd_Port_Return();
}

#ifndef BOARDTEST	
/*********************************************************************************************
* name:		Lcd_DspHz24
* func:		display 24x24 Chinese Symbol on Lcd  
* para:		UINT16T x0          --  x coordinate of start point
*           UINT16T y0          --  y coordinate of start point
*           UINT16T ForeColor --  foreground color of lcd 
*           UINT8T *s        --  string to display 
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_DspHz24(UINT16T x0, UINT16T y0, UINT16T ForeColor, UINT8T *s)
{
	INT16T i,j,k,x,y,xx;
	UINT8T qm,wm;
	INT32T ulOffset;
	INT8T hzbuf[72];

	for(i = 0; i < strlen((const char*)s); i++)
	{
		if(((UINT8T)(*(s+i))) < 161)
		{
			break;
		}
		else
		{
			qm = *(s+i) - 176;                                  //161;
	    		wm = *(s+i + 1) - 161;
            		ulOffset = (INT32T)(qm * 94 + wm) * 72;
            		for (j = 0; j < 72; j ++)
            		{
            			hzbuf[j] = g_auc_hzk24[ulOffset + j];
             		}
            		for(y = 0; y < 24; y++)
            		{
	               	for(x = 0; x < 24; x++) 
	               	{
           				k = x % 8;
			       	if (hzbuf[y * 3 + x / 8]  & (0x80 >> k))
			       	{
			       		xx = x0 + x + i*12;
			       		PutPixel( xx, y + y0, (UINT16T)ForeColor);
			       	}
			      }
            		}
		    	i++;
         	}
     	}     	
}
#endif

/*********************************************************************************************
* name:		Lcd_DspAscII6x8()
* func:		display 6x8 ASCII character string 
* para:		usX0,usY0 -- ASCII character string's start point coordinate
*			ForeColor -- appointed color value
*			pucChar   -- ASCII character string
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
#define XWIDTH 			6
extern UINT8T g_ucAscii6x8[];
void Lcd_DspAscII6x8(UINT16T usX0, UINT16T usY0,UINT16T ForeColor, UINT8T* pucChar)
{
	UINT32T i,j;
	UINT8T  ucTemp;

	while( *pucChar != 0 )
	{
		for( i=0; i < 8; i++ )
		{
  			ucTemp = g_ucAscii6x8[(*pucChar) * 8 + i];
  			for( j = 0; j < 8; j++ )
  			{
  				if( (ucTemp & (0x80 >> j)) != 0 )
  				{
  					PutPixel(usX0 + i, usY0 + 8 - j, (UINT16T)ForeColor);
  				}  				
  			}
		}
		usX0 += XWIDTH;
		pucChar++;
	}
} 

#ifndef BOARDTEST	
/*********************************************************************************************
* name:		Lcd_DspAscII8X16
* func:		display 8x16 AscII Symbol on Lcd  
* para:		UINT16T x0          --  x coordinate of start point
*           UINT16T y0          --  y coordinate of start point
*           UINT16T ForeColor --  foreground color of lcd 
*           UINT8T *s        --  string to display 
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_DspAscII8x16(UINT16T x0, UINT16T y0, UINT16T ForeColor, UINT8T * s)
{
	INT16T i,j,k,x,y,xx;
	UINT8T qm;
	INT32T ulOffset;
	INT8T ywbuf[16];
    
	for(i = 0; i < strlen((const char*)s); i++)
	{
		if((UINT8T)*(s+i) >= 161)
		{
			return;
		}
		else
		{
			qm = *(s+i);
            		ulOffset = (INT32T)(qm) * 16;	
            		for (j = 0; j < 16; j ++)
            		{
            			ywbuf[j] = g_ucAscii8x16[ulOffset + j];
            		}

            		for(y = 0; y < 16; y++)
            		{
               		for(x = 0; x < 8; x++) 
               		{
                   			k = x % 8;
			       	if (ywbuf[y]  & (0x80 >> k))
			       	{
			       		xx = x0 + x + i*8;
			       		PutPixel( xx, y + y0, (UINT16T)ForeColor);
			       	}
			   	}
            		}
         	}
     	}
}
#endif	

/*********************************************************************************************
* name:		lcd_clr
* func:		clear LCD screen
* para:		none 
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void lcd_clr(void)
{
	UINT32T i;
	UINT32T *pDisp = (UINT32T*)LCD_ACTIVE_BUFFER;
	
	for (i = 0; i < (SCR_XSIZE_CSTN * SCR_YSIZE_CSTN /4); i++)
	{
		*pDisp = 0xffffffff;//ALLWHITE;
	}
}

/*********************************************************************************************
* name:		lcd_clr_rect
* func:		fill appointed area with appointed color
* para:		usLeft,usTop,usRight,usBottom -- area's rectangle acme coordinate
*			ucColor -- appointed color value
* ret:		none
* modify:
* comment:	also as clear screen function 
*********************************************************************************************/
void lcd_clr_rect(INT16T usLeft, INT16T usTop, INT16T usRight, INT16T usBottom, UINT16T ucColor)
{
	UINT32T i, j;
	UINT8T *pDisp = (UINT8T*)LCD_ACTIVE_BUFFER;
	
	for (i = usLeft; i < usRight; i++)
	for (j = usTop; j < usBottom; j++)
	{
		PutPixel(i,j,ucColor);
		//*(pDisp+i+j) = ucColor;
	}
}

/*********************************************************************************************
* name:		Lcd_Draw_Box()
* func:		Draw rectangle with appointed color
* para:		usLeft,usTop,usRight,usBottom -- rectangle's acme coordinate
*			ucColor -- appointed color value
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Draw_Box(INT16T usLeft, INT16T usTop, INT16T usRight, INT16T usBottom, UINT16T ucColor)
{
	Lcd_Draw_HLine(usLeft, usRight,  usTop,    ucColor, 1);
	Lcd_Draw_HLine(usLeft, usRight,  usBottom, ucColor, 1);
	Lcd_Draw_VLine(usTop,  usBottom, usLeft,   ucColor, 1);
	Lcd_Draw_VLine(usTop,  usBottom, usRight,  ucColor, 1);
}

/*********************************************************************************************
* name:		Lcd_Draw_Line()
* func:		Draw line with appointed color
* para:		usX0,usY0 -- line's start point coordinate
*			usX1,usY1 -- line's end point coordinate
*			ucColor -- appointed color value
*			usWidth -- line's width
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Draw_Line(INT16T usX0, INT16T usY0, INT16T usX1, INT16T usY1, UINT16T ucColor, UINT16T usWidth)
{
	INT16T usDx;
	INT16T usDy;
	INT16T y_sign;
	INT16T x_sign;
	INT16T decision;
	INT16T wCurx, wCury, wNextx, wNexty, wpy, wpx;

	if( usY0 == usY1 )
	{
		Lcd_Draw_HLine (usX0, usX1, usY0, ucColor, usWidth);
		return;
	}
	if( usX0 == usX1 )
	{
		Lcd_Draw_VLine (usY0, usY1, usX0, ucColor, usWidth);
		return;
	}
	usDx = abs(usX0 - usX1);
	usDy = abs(usY0 - usY1);
	if( ((usDx >= usDy && (usX0 > usX1)) ||
        ((usDy > usDx) && (usY0 > usY1))) )
    {
        GUISWAP(usX1, usX0);
        GUISWAP(usY1, usY0);
    }
    y_sign = (usY1 - usY0) / usDy;
    x_sign = (usX1 - usX0) / usDx;

    if( usDx >= usDy )
    {
        for( wCurx = usX0, wCury = usY0, wNextx = usX1,
             wNexty = usY1, decision = (usDx >> 1);
             wCurx <= wNextx; wCurx++, wNextx--, decision += usDy )
        {
            if( decision >= usDx )
            {
                decision -= usDx;
                wCury += y_sign;
                wNexty -= y_sign;
            }
            for( wpy = wCury - usWidth / 2;
                 wpy <= wCury + usWidth / 2; wpy++ )
            {
                PutPixel(wCurx, wpy, ucColor);
            }

            for( wpy = wNexty - usWidth / 2;
                 wpy <= wNexty + usWidth / 2; wpy++ )
            {
                PutPixel(wNextx, wpy, ucColor);
            }
        }
    }
    else
    {
        for( wCurx = usX0, wCury = usY0, wNextx = usX1,
             wNexty = usY1, decision = (usDy >> 1);
             wCury <= wNexty; wCury++, wNexty--, decision += usDx )
        {
            if( decision >= usDy )
            {
                decision -= usDy;
                wCurx += x_sign;
                wNextx -= x_sign;
            }
            for( wpx = wCurx - usWidth / 2;
                 wpx <= wCurx + usWidth / 2; wpx++ )
            {
                PutPixel(wpx, wCury, ucColor);
            }

            for( wpx = wNextx - usWidth / 2;
                 wpx <= wNextx + usWidth / 2; wpx++ )
            {
                PutPixel(wpx, wNexty, ucColor);
            }
        }
    }
}

/*********************************************************************************************
* name:		Lcd_Draw_HLine()
* func:		Draw horizontal line with appointed color
* para:		usX0,usY0 -- line's start point coordinate
*			usX1 -- line's end point X-coordinate
*			ucColor -- appointed color value
*			usWidth -- line's width
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Draw_HLine(INT16T usX0, INT16T usX1, INT16T usY0, UINT16T ucColor, UINT16T usWidth)
{
	INT16T usLen;

    if( usX1 < usX0 )
    {
        GUISWAP (usX1, usX0);
    }

    while( (usWidth--) > 0 )
    {
        usLen = usX1 - usX0 + 1;
        while( (usLen--) > 0 )
        {
        	PutPixel(usX0 + usLen, usY0, ucColor);
        }
        usY0++;
    }
}

/*********************************************************************************************
* name:		Lcd_Draw_VLine()
* func:		Draw vertical line with appointed color
* para:		usX0,usY0 -- line's start point coordinate
*			usY1 -- line's end point Y-coordinate
*			ucColor -- appointed color value
*			usWidth -- line's width
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void Lcd_Draw_VLine (INT16T usY0, INT16T usY1, INT16T usX0, UINT16T ucColor, UINT16T usWidth)
{
	INT16T usLen;

    if( usY1 < usY0 )
    {
        GUISWAP (usY1, usY0);
    }

    while( (usWidth--) > 0 )
    {
        usLen = usY1 - usY0 + 1;
        while( (usLen--) > 0 )
        {
        	PutPixel(usX0, usY0 + usLen, ucColor);
        }
        usX0++;
    }
}

/*********************************************************************************************
* name:		color_lcd_test()
* func:		LCD test function
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
#define ADC_FREQ 2500000
#define BUFFER_LENGTH 110


double vBuffer[BUFFER_LENGTH];//电压缓冲区
double drawBuffer[BUFFER_LENGTH];//绘制缓冲区

int bufferIndex = 0;//缓冲区索引

volatile UINT16T unPreScaler;

UINT32T _GetTft16Bit_800480(UINT32T x,UINT32T y) {
    if (x < SCR_XSIZE_TFT_800480 && y < SCR_YSIZE_TFT_800480)
        return (frameBuffer16BitTft800480[y][x/2] >> ((2-1-((x)%2))*16)) & 0x0000ffff;
    else
        return 0;
	}
#define SCROLL_SPEED 6
void scrollScreen() {
	int x,y;
    for (y = 110; y < 450; y++) {
        for (x =40; x < 700-SCROLL_SPEED; x++) {
            UINT32T color = _GetTft16Bit_800480(x + SCROLL_SPEED, y);
            _PutTft16Bit_800480(x, y, color);
        }
    }
    for (y = 110; y < 450; y++) {
			for(x = 700 - SCROLL_SPEED ;x < 702;x ++){
        _PutTft16Bit_800480(x, y, WHITE);
    }
	}
}

void show(void)
{
 int y=25;
 int i,j;
	//BitmapView((unsigned char*)g_ucBitmap);
 lcd_clr_rect(42,110,705,450,WHITE);
	
 Glib_FilledRectangle(0,0,800,700,WHITE);

 //显示学号姓名
	Lcd_DspAscII8x16(120,25,BLACK,"20101111");
 Lcd_DspHz24(190,25,BLACK,"张子霖");
	Lcd_DspAscII8x16(270,25,BLACK,"20101113");
Lcd_DspHz24(340,25,BLACK,"李昂");
	Lcd_DspAscII8x16(390,25,BLACK,"20101113");
	Lcd_DspHz24(450,25,BLACK,"何涛");

 //绘制坐标轴
 Lcd_Draw_Line(35,70,35,455,BLACK,2);
 Lcd_Draw_Line(35,455,700,455,BLACK,2);
 Lcd_DspHz24(60,70,BLACK,"电压"); 
 Lcd_DspAscII6x8(110,80,BLACK,"(V)");
 //y
 Lcd_Draw_Line(35,110,30,120,BLACK,2);
 Lcd_Draw_Line(35,110,35,120,BLACK,2);
 //x
 Lcd_Draw_Line(700,455,690,460,BLACK,2);
 Lcd_Draw_Line(700,455,690,460,BLACK,2);
 //刻度
 for(i=110;i<460;i+=20){
  Lcd_Draw_Line(35,i,40,i,BLACK,2);
 }
 /*
 for(i=110;i<450;i+=20){
  Lcd_Draw_Line(40,j,45,j,BLACK,2);
 }
*/
 //刻度值
 Lcd_DspAscII6x8(5,105,BLACK,"3.4");
 Lcd_DspAscII6x8(5,125,BLACK,"3.2");
 Lcd_DspAscII6x8(5,145,BLACK,"3.0");
 Lcd_DspAscII6x8(5,165,BLACK,"2.8");
 Lcd_DspAscII6x8(5,185,BLACK,"2.6");
 Lcd_DspAscII6x8(5,205,BLACK,"2.4");
 Lcd_DspAscII6x8(5,225,BLACK,"2.2");
 Lcd_DspAscII6x8(5,245,BLACK,"2.0");
 Lcd_DspAscII6x8(5,265,BLACK,"1.8");
 Lcd_DspAscII6x8(5,285,BLACK,"1.6");
 Lcd_DspAscII6x8(5,305,BLACK,"1.4");
 Lcd_DspAscII6x8(5,325,BLACK,"1.2");
 Lcd_DspAscII6x8(5,345,BLACK,"1.0");
 Lcd_DspAscII6x8(5,365,BLACK,"0.8");
 Lcd_DspAscII6x8(5,385,BLACK,"0.6");
 Lcd_DspAscII6x8(5,405,BLACK,"0.4");
 Lcd_DspAscII6x8(5,425,BLACK,"0.2");
 Lcd_DspAscII6x8(5,445,BLACK,"0");
}


double lastY =0;
double lastX = 40;

void color_lcd_test(void)
{
 int i,j;
 double x=40,y=0;
 double x1,y1;
 char a[6]={0};
 unsigned char*b=(unsigned char*)a;
 UINT16T usConData;
 float usEndData;

#ifndef BOARDTEST 
//Lcd_DspHz24(50,10,BLUE,"英蓓特三星实验平台");
#endif
 uart_printf("\n ADC_IN Test\n");
 uart_printf("ADC conv.freq.=%dHz\n",ADC_FREQ);
 unPreScaler = PCLK/ADC_FREQ - 1;
 rADCCON=(1<<14)|(unPreScaler<<6)|(0<<3)|(0<<2)|(1<<1);
 uart_printf("Please adjust AINO value!\n");
 uart_printf("The results of ADC are:\n");
 usConData=rADCDAT0&0x3FF;
 lcd_init_app();
 led_init();
 show();
 Glib_FilledRectangle(150,70,300,80,WHITE);


		x1=40,y1=450;

 //初始化绘制缓冲区
 for(i=0;i<BUFFER_LENGTH;i++){
	 while(!(rADCCON & 0x8000));
	 usConData = rADCDAT0 & 0x3FF;
	 vBuffer[i] = usConData * 3.3000/0x3FF;
	 drawBuffer[i] = vBuffer[i];
	 x = x+6;
	 y = 450 - drawBuffer[i]*100; 
	 //Lcd_Draw_Line(x1,y1,x,y,DARKGRAY,2);
	 y1=y;
	 x1 = x;
	 
	 delay(500);
	 
 }
  
  lastY = 450 - drawBuffer[0]*100;
	for(j = 0;j<100;j++){
		while(!(rADCCON & 0x8000));
		usConData = rADCDAT0 & 0x3FF;
		usEndData = usConData * 3.3000/0x3FF;
		//将电压值添加到最新数据的缓冲区
		vBuffer[bufferIndex] = usEndData;
		//移动缓冲区，循环使用
		bufferIndex = (bufferIndex + 1)%BUFFER_LENGTH;
		 
		 x = 700 - BUFFER_LENGTH*6;
		 for (i=0;i<BUFFER_LENGTH;i ++){
			 drawBuffer[i] = vBuffer[(bufferIndex + i )%BUFFER_LENGTH];
			 y = 450 - drawBuffer[i]*100;
			 x += 6;
			 if(i>0){
				 Lcd_Draw_Line(lastX,lastY,x,y,DARKGRAY,2);
		 }
			 lastX = x;
		 lastY = y;
	 }
		 
		//清空显示区域
		//lcd_clr_rect(40,110,115,45,WHITE);
		scrollScreen();
			 
			 
		//绘制缓冲区图线

		
		sprintf(a,"%0.3f",usEndData);
		a[5] = 'v';
		Glib_FilledRectangle(150,70,300,100,WHITE);
		Lcd_DspAscII8x16(150,73,RED,b);
		char_out(0,(u8*)a);
		
	
	}
	
	uart_printf("End.\n");
	lcd_clr_rect(42,110,705,450,WHITE);
}		
