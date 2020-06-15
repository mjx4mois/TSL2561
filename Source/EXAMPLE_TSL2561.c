/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: light sensor
     File Name		: EXAMPLE_TSL2561.c
     Function		: EXAMPLE_TSL2561
     Create Date	: 2017/11/01
---------------------------------------------------------------------- */

#ifndef __TSL2561_EXAMPLE__
#define __TSL2561_EXAMPLE__

#include <mega32a.h>
#include <stdio.h>
#include <delay.h>
#include <math.h>
#include <alcd.h>
#include <i2c.h>
#include "SENSOR_TSL2561.h"
#include "Porting_Layer.h"

void EXAMPLE_TSL2561(void);


void EXAMPLE_TSL2561(void)
{
	CHAR8S status = 0;
	CHAR8U id = 0;
	INT16U light_data[2]={0};
	INT32U get_lux_value=0;
	INT32U lcd_char_data[5]={0}; 
	
		i2c_stop_hang();

		/* get the TSL2561 id */
		status = TSL2561_GET_ID(&id);
		if(status !=0) 
		{
			printf("TSL2561 read ID fail\r\n");	
		}
		else
		{
			printf("TSL2561 ID =0x%x\r\n",id);	
		}

		/* initial TSL2561 */
		status = TSL2561_SET_INITIAL();
		if(status !=0)
		{
			printf("TSL2561 initial fail\r\n");	
		}
		else
		{
			printf("TSL2561 initial success \r\n",id);	
		}
		
		while(1)
		{
			/* wait data ok */
			delay_ms(102); 	/*NOMINAL INTEGRATION TIME : 101mS  */
			status = TSL2561_GET_DATA(&light_data[0]);
			if(status !=0) 
			{
				printf("get TSL2561 data fail\r\n");
			}

			/* calculate the raw data to light data */
			/* THIS EXAMPLE not for integration time 402mS & gain is not 16x */
			TSL2561_GET_CALCULATE_DATA(light_data[0],light_data[1],&get_lux_value);
			
			printf("TSL2561 lux = %ld \r\n",get_lux_value);	



			 /* Display Lux */
			{
				/* Display Lux */
				lcd_char_data[0] = (INT32U)(get_lux_value/10000)%10;     
				lcd_char_data[1] = (INT32U)(get_lux_value/1000)%10;
				lcd_char_data[2] = (INT32U)(get_lux_value/100)%10;
				lcd_char_data[3] = (INT32U)(get_lux_value/10)%10;
				lcd_char_data[4] = (INT32U)(get_lux_value)%10;					
			}
				
			/* SHOW Lux */                    
	            	lcd_gotoxy(0,0);
	            	lcd_putsf("Lux:");     
	       

			/* show Temperautre data on LCD */
			lcd_putchar(48+lcd_char_data[0]);
			lcd_putchar(48+lcd_char_data[1]);
			lcd_putchar(48+lcd_char_data[2]);				
			lcd_putchar(48+lcd_char_data[3]);
			lcd_putchar(48+lcd_char_data[4]);
			lcd_putsf(" lx");
			
			/* --------- Temperature bolck --------- */


			/* --------- Show ID bolck --------- */
			/* SHOW ID */                    
	            	lcd_gotoxy(0,3);
	            	lcd_putsf("TSL2561");  
			/* --------- Show ID bolck --------- */
					
		}
		
}

#endif		//#ifndef __TSL2561_EXAMPLE__
