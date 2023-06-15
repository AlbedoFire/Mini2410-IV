/*********************************************************************************************
* File:		Dotled.c
* Author:	embest
* Desc:		DotLed_Test 
* History:	
*********************************************************************************************/

/*------------------------------------------------------------------------------------------*/
/*	 								include files						 				    */
/*------------------------------------------------------------------------------------------*/
#include "2410lib.h"
#include "ziku.h"
#include "fonts.h"
#include "led16x16.h"

/*------------------------------------------------------------------------------------------*/
/*	 								constant define						 				    */
/*------------------------------------------------------------------------------------------*/
#define En_Dataout rGPBDAT |= 0x20
#define DisEn_Dataout rGPBDAT &= 0xFFDF

#define CLK_H rGPDDAT |= 0x400                            
#define CLK_L rGPDDAT &= 0xFBFF                            

#define Rdata_H rGPCDAT |= 0x1                      
#define Rdata_L rGPCDAT &= 0xFFFE                      

#define Ldata_H rGPCDAT |= 0x1
#define Ldata_L rGPCDAT &= 0xFFFE

#define Lstr_H rGPBDAT |= 0x10
#define Lstr_L rGPBDAT &= 0xFFEF

#define Rstr_H rGPGDAT |= 0x100
#define Rstr_L rGPGDAT &= 0xFEFF

/*------------------------------------------------------------------------------------------*/
/*	 								global variables					 				    */
/*------------------------------------------------------------------------------------------*/
INT8T cTemp;
INT8T cEnChange0,cEnChange1;
UINT8T dispram[32];

/*********************************************************************************************
* name:		write_L4094
* func:		write list data
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void write_L4094(UINT16T data)
{
	UINT8T i;     
	Lstr_H;  
	for(i=0;i<16;i++)
	{
		CLK_L;
        if(data & 0x01==1)
			Ldata_H;
        else
         	Ldata_L;
        data = data >> 1;      
        CLK_H;
	}
	Lstr_L;
}

/*********************************************************************************************
* name:		write_R4094
* func:		write row data
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void write_R4094(UINT8T data)
{
   UINT8T i;       
   Rstr_H;
   for(i = 0; i < 8; i++)
   {
		CLK_L;
        if(data & 0x01==1)
        	Rdata_H;
        else
         	Rdata_L;
        data = data >> 1;      
        CLK_H;
	}
	Rstr_L;
}

/*********************************************************************************************
* name:		led_char_disp
* func:		led char display
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void led_char_disp(void)
{
	UINT8T i=0;
	UINT8T j=2,k=0;
	UINT16T x;

	for(i = 0; i < 32; i++)
		dispram[i] = ~l_display_array[i];
	i=0;
	while(i < 16)
	{
		Rstr_L;										// Enable RSTR
		Lstr_L;										// Enable LSTR
		DisEn_Dataout;								// lock the data

		do {   										// Write the row data
			j--;
			write_R4094(dispram[i * 2 + j]);
		}while(j);

		x=~(0x8000 >> i);	   						// Write one list data
		write_L4094(x);

		En_Dataout;									// Output the data

		for(k = 0;k < 250; k++);

		i++;
		j = 2;

	}
}
