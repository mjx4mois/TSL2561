/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: LIGHT Sensor
     File Name		: SENSOR_TSL2561.h
     Function		: SENSOR_TSL2561
     Create Date	: 2017/07/18
---------------------------------------------------------------------- */
#ifndef __TSL2561_HEADER__
#define __TSL2561_HEADER__  

/*--------------------------------- Define SLAVE ADDRESS -------------------------------------*/
#define TSL2561_GND   		(0)	
#define TSL2561_VCC		(1)
#define TSL2561_FOLAT		(2)

/*	
	Set "0" -> ADDR_SEL GND ; 
	Set "1" -> ADDR_SEL VCC ; 
	Set "2" -> ADDR_SEL FLOAT
*/
#define TSL2561_ADDR_SEL		(TSL2561_GND)	
/* TSL2561 SLAVE ADDRESS */
#if (TSL2561_ADDR_SEL == 1)
#define TSL2561_SLAVE_ADDRESS 		(0x92)				
#elif (TSL2561_ADDR_SEL == 0)
#define TSL2561_SLAVE_ADDRESS 		(0x52)
#elif (TSL2561_ADDR_SEL == 2)
#define TSL2561_SLAVE_ADDRESS 		(0x72)
#endif
/*--------------------------------- Define SLAVE ADDRESS -------------------------------------*/


/*------------- DEFINE TSL2561 REGISTER  ------------*/
/*-------------------------------------------------*/
/* 
	*********** TSL2561 use " COMMAND REGISTER " to control else Register! *********** 
	 	detail see spec.
    	*********** COMMAND REGISTER  ***********  
	BIT 7 [CMD]		Select command register. Must write as " 1 " .
	-----------------
	BIT 6 [CLEAR]	Interrupt clear. Clears any pending interrupt. This bit is a write-one-to-clear bit. It is self clearing.
	-----------------
	BIT 5 [WORD]	SMB Write/Read Word Protocol. 
					Set "1" that this SMB transaction is using either the SMB Write Word or Read Word protocol.
	-----------------
	BIT 4 [BLOCK]	Block Write/Read Protocol.
			 		1 indicates that this transaction is using either the Block Write or the Block Read protocol. 
	-----------------
	BIT 3~0  Register Address . see below
*/
/*----------------------------------------------------------------------*/
// **************************** VERY IMPORTANT ***************************
// package :   bit7		bit6			bit5			bit4			bit3 		-		bit0
//			  [CMD]	   [CLEAR]	  [WORD]	  [BLOCK]		[REGISTER ADDRESS]
// **********************************************************************


/*---------------------------------------------------------------------- */
/****** 以下都要用  package 來做*/
/****** I2C 限制 -> READ/WRITE MAX -> 2 BYTE  ; use  TSL2561_CMD_BTI5 */
/*-----------------*/
#define TSL2561_REG_CONTROL				(0x00)	/* Control of basic functions */
/*
	bit 7 ~ 2 : Reserved . Write was "0".
	bit 1 ~ 0 [POWER]	Power up/power down. wirte 0x03 -> powered up . wirte 0x00 -> powered down .
	This feature can be used to verify that the device is communicating properly.
*/
/*-----------------*/
#define TSL2561_REG_TIMING					(0x01)	/* Integration time/gain control */
/*
	The TIMING register controls both the integration time and the gain of the ADC channels. A common set of
	control bits is provided that controls both ADC channels. The TIMING register defaults to 02h at power on.
	
	bit 7 ~ 5 , 2 : Reserved . Write was "0".
	
	bit 4 [GAIN]:Switches gain between low gain and high gain modes . "1" -> high gain (16×)  ;  "0" -> low gain (1×)
	
	bit 3 [Manual]	Manual timing control.  set "1" begins an integration cycle. set "0" stops an integration cycle.
	NOTE: This field only has meaning when INTEG [bit 1 ~ 0] = 11. It is ignored at all other times.
	
	bit 1 ~ 0 [INTEG]	Integrate time. This field selects the integration time for each conversion.
	INTEG FIELD VALUE		 SCALE 		NOMINAL INTEGRATION TIME
		00 					0.034 					13.7 ms
		01 					0.252 					101 ms
		10					 1 						402 ms
		11 					 -- 						N/A
*/
/*-----------------*/
#define TSL2561_REG_THRESH_LOW_LOW		(0x02)	/*Low byte of low interrupt threshold */
/*ADC channel 0 LOWER byte of the LOW threshold*/
/*-----------------*/
#define TSL2561_REG_THRESH_LOW_HIGH	(0x03)	/* High byte of low interrupt threshold */
/*ADC channel 0 UPPER byte of the LOW threshold*/
/*-----------------*/
#define TSL2561_REG_THRESH_HIGH_LOW	(0x04)	/* Low byte of high interrupt threshold */
/*ADC channel 0 LOWER byte of the HIGH threshold*/
/*-----------------*/
#define TSL2561_REG_THRESH_HIGH_HIGH	(0x05)	/* High byte of high interrupt threshold */
/*0 ADC channel 0 UPPER byte of the HIGH threshold*/
/*-----------------*/
#define TSL2561_REG_INTERRUPT				(0x06)	/* Interrupt control */
/*
	bit 7 ~ 6 : Reserved . Write was "0".
	
	bit 5 ~ 4 [INTR]	INTR Control Select.
	INTR FIELD VALUE		READ VALUE
			00				Interrupt output disabled
			01 				Level Interrupt
			10 				SMBAlert compliant
			11 				Test Mode: Sets interrupt and functions as mode 10
			
	bit 3 ~ 0 [PERSIST]	Interrupt persistence.
	PERSIST FIELD VALUE		 	INTERRUPT PERSIST FUNCTION
			0000 				Every ADC cycle generates interrupt
			0001 				Any value outside of threshold range
			0010 				2 integration time periods out of range
			0011 				3 integration time periods out of range
			0100 				4 integration time periods out of range
			0101 				5 integration time periods out of range
			0110 				6 integration time periods out of range
			0111 				7 integration time periods out of range
			1000 				8 integration time periods out of range
			1001				9 integration time periods out of range
			1010 				10 integration time periods out of range
			1011 				11 integration time periods out of range
			1100 				12 integration time periods out of range
			1101 				13 integration time periods out of range
			1110 				14 integration time periods out of range
			1111 				15 integration time periods out of range
*/
/*-----------------*/
#define TSL2561_REG_CRC						(0x08)	/* Factory test —   *****  NOT A USER REGISTER ***** */
/*-----------------*/
#define TSL2561_REG_ID						(0x0A)	/* Part number/ Rev ID */
/*
	bit 7 ~ 4 [PARTNO]	Part Number Identification: field value 0000 = TSL2560, field value 0001 = TSL2561
	bit 3 ~ 0 [REVNO]	Revision number identification
*/
/*-----------------*/
#define TSL2561_REG_DATA0LOW				(0x0C)	/* Low byte of ADC channel 0 ,READ ONLY */
/*ADC channel 0 lower byte*/
/*-----------------*/
#define TSL2561_REG_DATA0HIGH				(0x0D)	/* High byte of ADC channel 0 ,READ ONLY */
/*ADC channel 0 upper byte*/
/*-----------------*/
#define TSL2561_REG_DATA1LOW				(0x0E)	/* Low byte of ADC channel 1 ,READ ONLY */
/*ADC channel 1 lower byte*/
/*-----------------*/
#define TSL2561_REG_DATA1HIGH				(0x0F)	/* High byte of ADC channel 1 ,READ ONLY */
/*ADC channel 1 upper byte*/
/*-----------------*/
/*------------------------------------------------------------------------------------*/
/* ------------- DEFINE TSL2561 REGISTER  ------------*/



/*------------- DEFINE TSL2561 PARAMETER ------------*/
/* 
	function parameter : 	TSL2561_I2C_READ_1BYTE use.
						TSL2561_I2C_WRITE_1BYTE use.
						TSL2561_I2C_READ_2BYTE use.
*/
#define TSL2561_CMD_BIT7						(0x80)	/*mask bit 7 must "1" , Select command register. */
#define TSL2561_CMD_BIT6						(0x40)	/*mask bit 6 set "1"  , Interrupt clear*/
#define TSL2561_CMD_BTI5						(0x20)	/*mask bit 5  Read / Write   1word [2byte]*/

/*
	function parameter :	TSL2561_SET_POWER_DOWN use.
					     	TSL2561_SER_POWER_ON use.
*/
#define TSL2561_POWER_ON					(0x03)	/*set TSL2561 power on*/
#define TSL2561_POWER_OFF					(0x00)	/*set TSL2561 power off*/

/*
	function parameter : TSL2561_SET_GAIN use.
*/
#define TSL2561_GAIN_16x 						(0x10)	/*set high gain x16  */
#define TSL2561_GAIN_1x						(0x00)	/*set low gain x1	*/

#define TSL2561_MANUAL						(0x08)	/*set manual mode ;is field only has meaning when INTEG = 11. It is ignored at all other times.*/
#define TSL2561_INTEG0						(0x00)	/*NOMINAL INTEGRATION TIME : 13.7mS*/
#define TSL2561_INTEG1						(0x01)	/*NOMINAL INTEGRATION TIME : 101mS*/
#define TSL2561_INTEG2						(0x02)	/*NOMINAL INTEGRATION TIME : 402mS*/
#define TSL2561_INTEG3						(0x03)	/*NOMINAL INTEGRATION TIME : NA*/


/* 
	function parameter : TSL2561_SET_INTERRPUT_MODE use.
*/
#define TSL2561_INTR0_DISABLE_INT			(0x00)		
#define TSL2561_INTR1_LEVEL_INT			(0x10)	/*use level interrput -> using TSL2561_INTERRPUT_MODE*/
#define TSL2561_INTR2_SMBUS_ALART		(0x20)
#define TSL2561_INTR3_TEST_MODE			(0x30)

/* TSL2561 interrput parameter */
typedef enum
{
	EVERY_ADC_CYCLE_GENERATES_INT=0,
	ANY_VALUE_OUTSIDE_OF_THD_RANGE,
	INTEGRATION_TIME_2_PERIOD_OUTOF_RANGE,
	INTEGRATION_TIME_3_PERIOD_OUTOF_RANGE,
	INTEGRATION_TIME_4_PERIOD_OUTOF_RANGE,
	INTEGRATION_TIME_5_PERIOD_OUTOF_RANGE,
	INTEGRATION_TIME_6_PERIOD_OUTOF_RANGE,
	INTEGRATION_TIME_7_PERIOD_OUTOF_RANGE,
	INTEGRATION_TIME_8_PERIOD_OUTOF_RANGE,
	INTEGRATION_TIME_9_PERIOD_OUTOF_RANGE,
	INTEGRATION_TIME_10_PERIOD_OUTOF_RANGE,
	INTEGRATION_TIME_11_PERIOD_OUTOF_RANGE,
	INTEGRATION_TIME_12_PERIOD_OUTOF_RANGE,
	INTEGRATION_TIME_13_PERIOD_OUTOF_RANGE,
	INTEGRATION_TIME_14_PERIOD_OUTOF_RANGE,
	INTEGRATION_TIME_15_PERIOD_OUTOF_RANGE,
} TSL2561_INTERRPUT_MODE;


//** function parameter : TSL2561_GET_CALCULATE_DATA use.*/
/*set timming register*/
#define TSL2561_PACKAGE_T		(0)
#define TSL2561_PACKAGE_CS		(1)
/*------------- DEFINE TSL2561 PARAMETER ------------*/



/**********************************************/
/*SET CONFIG Parameter*/
/*** function parameter : TSL2561_SET_INTERRPUT_MODE use.*/
/*set interrupt control*/
#define TSL2561_SET_INT_CTL								(TSL2561_INTR0_DISABLE_INT | EVERY_ADC_CYCLE_GENERATES_INT)	

/*** function parameter : TSL2561_SET_INTERRPUT_MODE use.*/
/*set timming register*/
#define TSL2561_SET_AUTO_INTEGRATION_TIME		(TSL2561_INTEG2)

/*** function parameter : TSL2561_GET_CALCULATE_DATA use.*/
/*set timming register*/
#define TSL2561_PACKAGE_TYPE							(TSL2561_PACKAGE_T)
/**********************************************/


/**************************** CALCULATE USE PARAMETER **************************************/
/*
	FOR API function : TSL2561_GET_CALCULATE_DATA
*/

#define LUX_SCALE 				(14)			/*	scale by 2^14				*/
#define RATIO_SCALE 			(9)			/*	scale ratio by 2^9			*/
#define CH_SCALE 				(10)			/*	scale channel values by 2^10*/
#define CHSCALE_TINT0 			(0x7517)	/*	322/11 * 2^CH_SCALE		*/
#define CHSCALE_TINT1 			(0x0fe7)	/*	322/81 * 2^CH_SCALE		*/
/*------------------------------------------------------*/
#define K1T	(0x0040)	/*	0.125 * 2^RATIO_SCALE	*/
#define B1T	(0x01f2)	/*	0.0304 * 2^LUX_SCALE		*/
#define M1T	(0x01be)	/*	0.0272 * 2^LUX_SCALE		*/
#define K2T	(0x0080)	/*	0.250 * 2^RATIO_SCA		*/
#define B2T	(0x0214)	/*	0.0325 * 2^LUX_SCALE		*/
#define M2T	(0x02d1)	/*	0.0440 * 2^LUX_SCALE		*/
#define K3T	(0x00c0)	/*	0.375 * 2^RATIO_SCALE	*/
#define B3T	(0x023f)	/*	0.0351 * 2^LUX_SCALE		*/
#define M3T	(0x037b)	/*	0.0544 * 2^LUX_SCALE		*/
#define K4T	(0x0100)	/*	0.50 * 2^RATIO_SCALE		*/
#define B4T	(0x0270)	/*	0.0381 * 2^LUX_SCALE		*/
#define M4T	(0x03fe)	/*	0.0624 * 2^LUX_SCALE		*/
#define K5T	(0x0138)	/*	0.61 * 2^RATIO_SCALE		*/
#define B5T	(0x016f)	/*	0.0224 * 2^LUX_SCALE		*/
#define M5T	(0x01fc) 	/*	0.0310 * 2^LUX_SCALE		*/
#define K6T	(0x019a)	/*	0.80 * 2^RATIO_SCALE		*/
#define B6T	(0x00d2)	/*	0.0128 * 2^LUX_SCALE		*/
#define M6T	(0x00fb)	/*	0.0153 * 2^LUX_SCALE		*/
#define K7T	(0x029a)	/*	1.3 * 2^RATIO_SCALE		*/
#define B7T	(0x0018)	/*	0.00146 * 2^LUX_SCALE	*/
#define M7T	(0x0012)	/*	0.00112 * 2^LUX_SCALE	*/
#define K8T	(0x029a)	/*	1.3 * 2^RATIO_SCALE		*/
#define B8T	(0x0000)	/*	0.000 * 2^LUX_SCALE		*/
#define M8T	(0x0000)	/*	0.000 * 2^LUX_SCALE		*/
/*------------------------------------------------------*/
#define K1C	(0x0043)	/*	0.130 * 2^RATIO_SCALE	*/
#define B1C	(0x0204)	/*	0.0315 * 2^LUX_SCALE		*/
#define M1C	(0x01ad)	/*	0.0262 * 2^LUX_SCALE		*/
#define K2C	(0x0085)	/*	0.260 * 2^RATIO_SCALE	*/
#define B2C	(0x0228)	/*	0.0337 * 2^LUX_SCALE		*/
#define M2C	(0x02c1)	/*	0.0430 * 2^LUX_SCALE		*/
#define K3C	(0x00c8)	/*	0.390 * 2^RATIO_SCALE	*/
#define B3C	(0x0253)	/*	0.0363 * 2^LUX_SCALE		*/
#define M3C	(0x0363)	/*	0.0529 * 2^LUX_SCALE		*/
#define K4C	(0x010a)	/*	0.520 * 2^RATIO_SCALE	*/
#define B4C	(0x0282)	/*	0.0392 * 2^LUX_SCALE		*/
#define M4C	(0x03df)	/*	0.0605 * 2^LUX_SCALE		*/
#define K5C	(0x014d)	/*	0.65 * 2^RATIO_SCALE		*/
#define B5C	(0x0177)	/*	0.0229 * 2^LUX_SCALE		*/
#define M5C	(0x01dd)	/*	0.0291 * 2^LUX_SCALE		*/
#define K6C	(0x019a)	/*	0.80 * 2^RATIO_SCALE		*/
#define B6C	(0x0101)	/*	0.0157 * 2^LUX_SCALE		*/
#define M6C	(0x0127)	/*	0.0180 * 2^LUX_SCALE		*/
#define K7C	(0x029a)	/*	1.3 * 2^RATIO_SCALE		*/
#define B7C	(0x0037)	/*	0.00338 * 2^LUX_SCALE	*/
#define M7C	(0x002b)	/*	0.00260 * 2^LUX_SCALE	*/
#define K8C	(0x029a)	/*	1.3 * 2^RATIO_SCALE		*/
#define B8C	(0x0000)	/*	0.000 * 2^LUX_SCALE		*/
#define M8C	(0x0000)   	/*	0.000 * 2^LUX_SCALE		*/
/*------------------------------------------------------*/
/****************************** CALCULATE USE PARAMETER ***************************************/



/********************************************** SYSTEM **************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*	INITIAL TSL2561	*/
CHAR8S TSL2561_SET_INITIAL(void);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*	get the TSL2561 id	*/
CHAR8S TSL2561_GET_ID(CHAR8U *id);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	get channel 0 data -> 0x0C~0x0D  & turn to 16-bit
	get channel 1 data -> 0x0E~0x0F  & turn to 16-bit
*/
CHAR8S TSL2561_GET_DATA(INT16U *data);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	set interrput 0x02~0x05 
	0x02~0x03 -> low interrupt threshold	[16bit]
	0x04~0x05 -> high interrupt threshold	[16bit]
*/
CHAR8S TSL2561_SET_INTERRUPT_THD(INT16U low_thd,INT16U high_thd);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*	set the TSL2561 integration time and the gain of the ADC channels	*/
CHAR8S TSL2561_SET_GAIN(CHAR8U gain);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	set the interrput mode
	NOTE: Interrupts are based on the value of Channel 0 only.
*/
CHAR8S TSL2561_SET_INTERRPUT_MODE(void);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/* 	to clear the interrput pin status -> release low level.	*/
CHAR8S TSL2561_SET_CLEAR_INTERRPUT(void);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*	
	start measurement begin integration time
	if use TSL2561_SET_AUTO_INTEGRATION function dont use this function
*/
CHAR8S TSL2561_SET_MANUAL_ENABLE(void);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*	
	sotp measurement integration time
	if use TSL2561_SET_AUTO_INTEGRATION function dont use this function
*/
CHAR8S TSL2561_SET_MANUAL_DISABLE(void);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*	
	SET integration time  ; AUTO measurement 
	if use TSL2561_SET_MANUAL_ENABLE  & TSL2561_SET_MANUAL_DISABLE function dont use this function
*/
CHAR8S TSL2561_SET_AUTO_INTEGRATION(void);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*	get the INTEGRATION mode	*/
CHAR8S TSL2561_GET_AUTO_INTEGRATION_MODE(CHAR8U *int_data);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*	get TSL2561 power on mode	*/
CHAR8S TSL2561_GET_POWER_STATUS(CHAR8U *power_status);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*	set TSL2561 power on mode	*/
CHAR8S TSL2561_SET_POWER_ON(void);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*	set TSL2561 power down mode	*/
CHAR8S TSL2561_SET_POWER_DOWN(void);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*	Calculate TS2561 data	*/
CHAR8S TSL2561_GET_CALCULATE_DATA(INT16U raw_data_channel0,INT16U raw_data_channel1,INT32U *return_lux_data);
/*--------------------------------------------------------------------------------------------------*/


/************************************************* I2C *************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*	let TSL2561 read 1 byte data	*/
CHAR8S TSL2561_I2C_READ_1BYTE(CHAR8U reg_address,CHAR8U *r_data);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*	let TSL2561 write 1 byte data */
CHAR8S TSL2561_I2C_WRITE_1BYTE(CHAR8U reg_address,CHAR8U w_data);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*	let TSL2561 read 2 byte data	*/
CHAR8S TSL2561_I2C_READ_2BYTE(CHAR8U reg_address,CHAR8U *r_data);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*	let TSL2561 write 2 byte data */
CHAR8S TSL2561_I2C_WRITE_2BYTE(CHAR8U reg_address,CHAR8U *w_data);
/*--------------------------------------------------------------------------------------------------*/
/************************************************* I2C *************************************************/
/********************************************** SYSTEM **************************************************/

#endif //#ifndef __TSL2561_HEADER__ 
