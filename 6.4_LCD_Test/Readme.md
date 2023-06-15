for(j=0;j<330;j++){
  while(!(rADCCON&0x8000));
  usConData=rADCDAT0&0x3FF;
  usEndData=usConData*3.3000/0x3FF;
  uart_printf("%0.3f",usEndData);
	y=450-usEndData*100;
	x+=2;
	if(j==0){
		x1=x;
		y1=y;
	}
	//»æÖÆÐÂÕÛÏßÍ¼
	Lcd_Draw_Line(x1,y1,x,y,DARKGRAY,2);
	
	sprintf(a,"%0.3f",usEndData);
	a[5]='v';
	Glib_FilledRectangle(150,70,300,100,WHITE);
	Lcd_DspAscII8x16(150,73,RED,b);
	char_out(0,(u8*)a);
	
	
	x1=x;
	y1=y;
	//delay(100);
 }
 uart_printf("End. \n");
 lcd_clr_rect(42,110,705,450,WHITE);
}	
