/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: LIGHT Sensor 
     File Name		: SENSOR_TSL2561.c
     Function		: SENSOR_TSL2561
     Create Date	: 2017/06/30
---------------------------------------------------------------------- */

#define TSL2561_DEBUG				(0)		/* set "1" to printf debug message */

#include <stdio.h>
#include <math.h>
#include <delay.h>
#include <datatype_Layer.h>
#include <swi2c_Layer.h>
#include <SENSOR_TSL2561.h>

/********************************************* SYSTEM **************************************************/
/*--------------------------------------------------------------------------------------------------*/
/* INITIAL TSL2561 */
CHAR8S TSL2561_SET_INITIAL(void)
{
	CHAR8U status = 0;
	CHAR8U power_status=0;

		/* clear interrupt pending status , use Command Register bit 6 set '1' to clear interrupt  */
		status = TSL2561_SET_CLEAR_INTERRPUT();
		if(status !=0) 
		{
			return -1 ;	/*set fail.*/	
		}
		
		/* set the gain value 1x */		
		status =TSL2561_SET_GAIN(TSL2561_GAIN_1x);/*SET 1x*/
		if(status !=0) 
		{
			return -1 ;	/*set fail.*/	
		}

		/* set the interrupt mode  */		
		status =TSL2561_SET_INTERRPUT_MODE();
		if(status !=0) 
		{
			return -1 ;	/*set fail.*/	
		}

		/* set the interrupt high threshold & low threshold value  */		
		status = TSL2561_SET_INTERRUPT_THD(0,0);	/* INITIAL LOW THD : 0 ,HIGH THE : 0   (16bit data) */
		if(status !=0) 
		{
			return -1 ;	/*set fail.*/	
		}

		/* set auto integration time  */		
		status = TSL2561_SET_AUTO_INTEGRATION();
		if(status !=0) 
		{
			return -1 ;	/*set fail.*/	
		}

		/* let TLS2561 power up  */		
		status = TSL2561_SET_POWER_ON(); /* ENABLE TSL2561 sensor*/
		if(status !=0) 
		{
			return -1 ;	/*set fail.*/	
		}
		
		/* get the power status from Control Register (0x00)  */
		TSL2561_GET_POWER_STATUS(&power_status);/*get sensor status*/
		if(status !=0) 
		{
			return -1 ;	/*set fail.*/	
		}

		/* if the TSL2561 is  power off mode , print the status */
		if(power_status==TSL2561_POWER_OFF)
		{
			printf("TSL2561 power off error!!,0x%x\r\n",power_status);
		}

		return 0;	/* initial success !*/
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* get the TSL2561 IC ID */
CHAR8S TSL2561_GET_ID(CHAR8U *id)
{
	CHAR8U status = 0;
	CHAR8U read_data = 0;

		/* get TSL2561 id */
		status = TSL2561_I2C_READ_1BYTE(TSL2561_REG_ID,&read_data);
		if(status !=0) 
		{
			return -1;	/*read fail.*/	
		}
		
		*id = read_data;
		return 0; 	/*read success*/
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*get channel 0 data -> 0x0C~0x0D  & turn to 16-bit*/
/*get channel 1 data -> 0x0E~0x0F  & turn to 16-bit*/
CHAR8S TSL2561_GET_DATA(INT16U *data)
{
	CHAR8U status = 0;
	CHAR8U read_data[2] = {0};
	INT16U channel0_data=0,channel1_data=0;

		/*first read channel 0 data*/
		status = TSL2561_I2C_READ_2BYTE(TSL2561_REG_DATA0LOW,&read_data[0]);
		if(status !=0)
		{
			return -1 ; /*read fail.*/
		}

		/* get the channel0 data */
		channel0_data = (read_data[1]<<8) | (read_data[0]);
		

		/* first read channel 1 data*/
		status = TSL2561_I2C_READ_2BYTE((TSL2561_REG_DATA1LOW |TSL2561_CMD_BIT6) ,&read_data[0]); /* clear INT , SET BIT 6*/
		if(status !=0) 
		{
			return -1; /*read fail.*/
		}
		
		/* get the channel1 data */
		channel1_data = (read_data[1]<<8) | (read_data[0]);

		data[0] = channel0_data;
		data[1] = channel1_data;

#if TSL2561_DEBUG		
		printf("[TSL2561_GET_DATA]channel0 data = %ld , channel1 data = %ld \r\n",data[0],data[1]);
#endif

		return 0; /* read success*/
		
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*set interrput 0x02~0x05 */
/*0x02~0x03 -> low interrupt threshold	[16bit]*/
/*0x04~0x05 -> high interrupt threshold	[16bit]*/
CHAR8S TSL2561_SET_INTERRUPT_THD(INT16U low_thd,INT16U high_thd)
{
	CHAR8U status = 0;
	CHAR8U read_data[2] = {0};
	INT16U channel0_data=0,channel1_data=0;
		
		/*first write  low interrupt threshold  [0x02~0x03]*/
		read_data[0] = (CHAR8U)(low_thd & 0xff);/*mask low byte*/
		read_data[1] = (CHAR8U)(low_thd>>8);/*cache high byte*/
		status = TSL2561_I2C_WRITE_2BYTE(TSL2561_REG_THRESH_LOW_LOW,&read_data[0]);
		if(status !=0)
		{
			return -1 ;/*wirte fail.*/	
		}
		
		/*secode write  high interrupt threshold  [0x04~0x05]*/
		read_data[0] = (CHAR8U)(high_thd & 0xff); /*mask low byte*/
		read_data[1] = (CHAR8U)(high_thd>>8);/*cache high byte*/
		status = TSL2561_I2C_WRITE_2BYTE(TSL2561_REG_THRESH_HIGH_LOW,&read_data[0]);
		if(status !=0) 
		{
			return -1;/*wirte fail.*/	
		}
		
		return 0; /*write success;*/

}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*set the TSL2561 integration time and the gain of the ADC channels*/
CHAR8S TSL2561_SET_GAIN(CHAR8U gain)
{
	CHAR8U status = 0;
	CHAR8S write_data = 0,read_data=0;
	
		if(gain == TSL2561_GAIN_16x)
		{
			status = TSL2561_I2C_READ_1BYTE(TSL2561_REG_TIMING,&read_data); 
			if(status !=0) 
			{
				return -1 ;/*read fail.*/	
			}
			
			/*set gain 16x*/ 
			read_data =(read_data |gain) ;
			status = TSL2561_I2C_WRITE_1BYTE(TSL2561_REG_TIMING,gain);
			if(status !=0)
			{
				return -1; /*write fail.*/	
			}
			return 0; /*write success*/
		}
		else if((gain == TSL2561_GAIN_1x))
		{
			status = TSL2561_I2C_READ_1BYTE(TSL2561_REG_TIMING,&read_data); 
			if(status !=0) 
			{
				return -1 ;/*read fail.*/	
			}
			read_data = read_data & (0xEF);/*set gain 1x*/ 
			status = TSL2561_I2C_WRITE_1BYTE(TSL2561_REG_TIMING,read_data);
			if(status !=0)
			{
				return -1 ; /*write fail.*/
			}
			return 0 ;/* write success*/

		}
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*set the interrput mode*/
/*NOTE: Interrupts are based on the value of Channel 0 only.*/
CHAR8S TSL2561_SET_INTERRPUT_MODE(void)
{
	CHAR8U status = 0;
	CHAR8S write_data = 0;
		
		status = TSL2561_I2C_WRITE_1BYTE(TSL2561_REG_INTERRUPT ,TSL2561_SET_INT_CTL);
		if(status !=0) 
		{
			return -1; 	/*write fail.*/	
		}
		return 0; /* write success.*/
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*to clear the interrput pin status -> release low level.*/
CHAR8S TSL2561_SET_CLEAR_INTERRPUT(void)
{
	CHAR8U status = 0;
	CHAR8U read_data = 0;
		
		/* clear INT , SET BIT 6*/
		status = TSL2561_I2C_READ_1BYTE((TSL2561_REG_CONTROL |TSL2561_CMD_BIT6) ,&read_data);
		if(status !=0) 
		{
			return -1;	/*read fail.*/
		}
		else
		{
			return 0;	/*read success.*/
		}
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* start measurement begin integration time*/
/* if use TSL2561_SET_AUTO_INTEGRATION function dont use this function*/
CHAR8S TSL2561_SET_MANUAL_ENABLE(void)
{
	CHAR8U status = 0;
	CHAR8S write_data = 0,read_data=0;

		status = TSL2561_I2C_READ_1BYTE(TSL2561_REG_TIMING,&read_data); 
		if(status !=0) 
		{
			return -1;	/*read fail. */
		}

		if(read_data & (TSL2561_MANUAL |TSL2561_INTEG3)) 
		{
			return -2;	/*already enable manual mode*/
		}
		
		/*write enable manual mode*/ /*if use manual INTEG[1:0]='11' */
		read_data = (read_data |TSL2561_MANUAL | TSL2561_INTEG3);
		status = TSL2561_I2C_WRITE_1BYTE(TSL2561_REG_TIMING,read_data);
		if(status !=0)
		{
			return -1 ;	/*write fail.*/	
		}
		
		return 0; /*write success*/
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* sotp measurement integration time*/
/* if use TSL2561_SET_AUTO_INTEGRATION function dont use this function*/
CHAR8S TSL2561_SET_MANUAL_DISABLE(void)
{
	CHAR8U status = 0;
	CHAR8S write_data = 0,read_data=0;

		status = TSL2561_I2C_READ_1BYTE(TSL2561_REG_TIMING,&read_data); 
		if(status !=0) 
		{
			return -1;	/*read fail.*/
		}
		
		read_data = read_data & (0xF7) | TSL2561_INTEG3;	
		status = TSL2561_I2C_WRITE_1BYTE(TSL2561_REG_TIMING,read_data);	/*write enable manual mode*/
		if(status !=0)
		{
			return -1;	/*write fail.*/
		}
		return 0;	/*write success*/
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* SET integration time  ; AUTO measurement */
/* if use TSL2561_SET_MANUAL_ENABLE  & TSL2561_SET_MANUAL_DISABLE function dont use this function*/
CHAR8S TSL2561_SET_AUTO_INTEGRATION(void)
{
	CHAR8U status = 0;
	CHAR8S write_data = 0,read_data=0;

		status = TSL2561_I2C_READ_1BYTE(TSL2561_REG_TIMING,&read_data); 
		if(status !=0) 
		{
			return -1;	/*read fail.*/
		}
		
		read_data = ((read_data & (0xF0)) |TSL2561_SET_AUTO_INTEGRATION_TIME);	
		status = TSL2561_I2C_WRITE_1BYTE(TSL2561_REG_TIMING,read_data);	/*write enable manual mode*/
		if(status !=0) 
		{
			return -1;	/*write fail.*/
		}
		return 0;	/*write success*/
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*get the INTEGRATION mode*/
CHAR8S TSL2561_GET_AUTO_INTEGRATION_MODE(CHAR8U *int_data)
{
	CHAR8U status = 0;
	CHAR8S read_data = 0;
		
		status = TSL2561_I2C_READ_1BYTE(TSL2561_REG_TIMING ,&read_data);
		if(status !=0)
		{
			return -1;	/*write fail.*/
		}

#if TSL2561_DEBUG		
		printf("[TSL2561_GET_AUTO_INTEGRATION_MODE]TSL2561_REG_TIMING = 0x%x\r\n",read_data);
#endif
		
		*int_data = read_data;
		return 0;	/*write success.*/
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*get TSL2561 power on mode*/
CHAR8S TSL2561_GET_POWER_STATUS(CHAR8U *power_status)
{
	CHAR8U status = 0;
	CHAR8U read_data = 0;
	
		status = TSL2561_I2C_READ_1BYTE(TSL2561_REG_CONTROL,&read_data);
		if(status !=0)
		{
			return -1; /*read fail.*/	
		}

#if TSL2561_DEBUG		
		printf("[TSL2561_GET_POWER_STATUS]TSL2561_REG_CONTROL = 0x%x\r\n",read_data);
#endif
		
		*power_status = read_data ;
		return 0;	/*read success.*/
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*set TSL2561 power on mode*/
CHAR8S TSL2561_SET_POWER_ON(void)
{
	CHAR8U status = 0;
	CHAR8S write_data = 0;
		
		status = TSL2561_I2C_WRITE_1BYTE(TSL2561_REG_CONTROL,TSL2561_POWER_ON);
		if(status !=0) 
		{
			return -1;	/*write fail.*/	
		}
		
		return 0;	/*write success.*/	
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* set TSL2561 power down mode */
CHAR8S TSL2561_SET_POWER_DOWN(void)
{
	CHAR8U status = 0;
	CHAR8S write_data = 0;
		
		status = TSL2561_I2C_WRITE_1BYTE(TSL2561_REG_CONTROL,TSL2561_POWER_OFF);
		if(status !=0) 
		{
			return -1;	/*write fail.*/
		}
		
		return 0;	/*write success.*/
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* Calculate TS2561 data */
CHAR8S TSL2561_GET_CALCULATE_DATA(INT16U raw_data_channel0,INT16U raw_data_channel1,INT32U *return_lux_data)
{
	CHAR8S status = 0;
	CHAR8U read_data = 0;
	CHAR8U read_gain = 0;
	INT32U chScale;
	INT32U ratio1;
	INT32U b;
	INT32U m;
	INT32U temp;
	INT32U lux;
	INT32U ratio;
	 
	INT32U channel0 = (INT32U)raw_data_channel0;
	INT32U channel1 = (INT32U)raw_data_channel1;


#if TSL2561_DEBUG		
		printf("[TSL2561_GET_CALCULATE_DATA]channel0 =%lx , channel1 =%lx \r\n",channel0,channel1);
#endif
		
		/* READ STATUS */
		status = TSL2561_GET_AUTO_INTEGRATION_MODE(&read_data);
		if(status !=0) 
		{
			return -1;	/*read fail*/
		}
		
		read_gain = read_data & 0x10;	/*mask*/ 	/*GAIN */
		read_data = read_data & 0x03;	/*mask*/	/*INTEGRATION MODE */

		/* check read_data */
    		switch (read_data)
    		{
		        case 0:	/*13.7 msec*/
					chScale = CHSCALE_TINT0;
					break;
				
		        case 1: 	/*101 msec*/
					chScale = CHSCALE_TINT1;			
					break;
				
		        default:	 /*assume no scaling*/
					chScale = (1 << CH_SCALE);			
					break;
	    	}
		
		if (!read_gain) /* if read_gain ==0 */
		{
			chScale = chScale << 4;	/*scale 1X to 16X*/
		}
		
	    	/* scale the channel values */
	    	channel0 = (channel0 * chScale) >> CH_SCALE;
	    	channel1 = (channel1 * chScale) >> CH_SCALE; 

		ratio1 = 0;
		if (channel0!= 0) 
		{
			ratio1 = (channel1 << (RATIO_SCALE+1))/channel0;
		}

		/* round the ratio value*/
    		ratio = (ratio1 + 1) >> 1;

#if TSL2561_DEBUG		
		printf("ratio =0x%lx \r\n",ratio);
#endif	

   	 	switch (TSL2561_PACKAGE_TYPE)/*CHECK T package or CS package type*/
    		{
    			/*0*/
			case TSL2561_PACKAGE_T:/*T package*/
				            if ((ratio >= 0) && (ratio <= K1T)){b=B1T; m=M1T;}
				            else if (ratio <= K2T){b=B2T; m=M2T;}
				            else if (ratio <= K3T){b=B3T; m=M3T;}
				            else if (ratio <= K4T){b=B4T; m=M4T;}
				            else if (ratio <= K5T){b=B5T; m=M5T;}
				            else if (ratio <= K6T){b=B6T; m=M6T;}
				            else if (ratio <= K7T){b=B7T; m=M7T;}
				            else if (ratio > K8T){b=B8T; m=M8T;}
				break;
				
			/*1*/					
			case TSL2561_PACKAGE_CS:/*CS package*/
				            if ((ratio >= 0) && (ratio <= K1C)){b=B1C; m=M1C;}
				            else if (ratio <= K2C){b=B2C; m=M2C;}
				            else if (ratio <= K3C){b=B3C; m=M3C;}
				            else if (ratio <= K4C){b=B4C; m=M4C;}
				            else if (ratio <= K5C){b=B5C; m=M5C;}
				            else if (ratio <= K6C){b=B6C; m=M6C;}
				            else if (ratio <= K7C){b=B7C; m=M7C;}
				break;
		} 


		temp=((channel0*b)-(channel1*m));
		
		if(temp<0) 
		{	
			temp=0;
		}
		
		temp+=(1<<(LUX_SCALE-1));
		
		/* strip off fractional portion */
		lux= (temp >> LUX_SCALE);
		*return_lux_data = lux;

#if TSL2561_DEBUG		
		printf("[TSL2561_GET_CALCULATE_DATA]final lux =%ld \r\n",lux);
#endif
	
			
		return 0;	/*calculate ok!!*/
 
}
/*--------------------------------------------------------------------------------------------------*/



/************************************************** I2C *************************************************/
/*--------------------------------------------------------------------------------------------------*/
/* let TSL2561 read 1 byte data*/
CHAR8S TSL2561_I2C_READ_1BYTE(CHAR8U reg_address,CHAR8U *r_data)
{
	CHAR8S status = 0;
	CHAR8U reg=0,read=0;
		
		reg = ((TSL2561_CMD_BIT7 | reg_address));	/*mask bit 7 */

		status = i2c_read_1_byte_data(TSL2561_SLAVE_ADDRESS,reg,&read);
		if(status !=1) 
		{
			return -1;	/*read fail*/
		}
		
		*r_data = read;
		return 0;	/*read success*/
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* let TSL2561 write 1 byte data*/
CHAR8S TSL2561_I2C_WRITE_1BYTE(CHAR8U reg_address,CHAR8U w_data)
{
	CHAR8S status = 0;
	CHAR8U reg=0,write=0;
		
		reg = (TSL2561_CMD_BIT7 | reg_address);	/* mask bit 7 */
		write = w_data;
		status = i2c_write_1_byte_data(TSL2561_SLAVE_ADDRESS,reg,write);
		if(status !=1) 
		{
			return -1;	/*write fail*/	
		}
		
		return 0;	/*write success*/
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* let TSL2561 read 2 byte data*/
CHAR8S TSL2561_I2C_READ_2BYTE(CHAR8U reg_address,CHAR8U *r_data)
{
	CHAR8S status = 0;
	CHAR8U reg=0,read[2]={0};
		
		reg = (TSL2561_CMD_BIT7  | TSL2561_CMD_BTI5  | reg_address) ; 	/*mask bit 7   & bit 5*/

		status = i2c_read_n_byte_data(TSL2561_SLAVE_ADDRESS,reg,2,&read[0]);
		if(status !=1) 
		{
			return -1;	 /*write fail	*/
		}

		r_data[0] = read[0];
		r_data[1] = read[1];
		
		return 0;	/* read success*/

}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* let TSL2561 write 2 byte data*/
CHAR8S TSL2561_I2C_WRITE_2BYTE(CHAR8U reg_address,CHAR8U *w_data)
{
	CHAR8S status = 0;
	CHAR8U reg=0,write[2]={0};
		
		reg = (TSL2561_CMD_BIT7  | TSL2561_CMD_BTI5   | reg_address);	/* mask bit 7   & bit 5*/
		write[0] = w_data[0];
		write[1] = w_data[1];
		
		status = i2c_write_n_byte_data(TSL2561_SLAVE_ADDRESS,reg,2,&write[0]);
		if(status !=1)
		{
			return -1;	/* write fail	*/	
		}
		return 0;	/* write success*/
}
/*--------------------------------------------------------------------------------------------------*/
/************************************************** I2C *************************************************/
/********************************************** SYSTEM **************************************************/

