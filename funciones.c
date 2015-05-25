/**
* @file		funciones.c
* @author	Manuel Caballero
* @date 	11/5/2015
* @brief 	Funciones del sistema.
* \copyright
* 	AqueronteBlog@gmail.com
*
* Este archivo es propiedad intelectual del blog Aqueronte,
* cuya direccion web, es la siguiente:
*
* 	http://unbarquero.blogspot.com/
*
* Se permite cualquier modificacion del archivo siempre y cuando
* se mantenga la autoria del autor.
*/
#include "funciones.h"


/**
 *  \brief     void conf_CLK (void)
 *  \details   Configura los relojes del MCU.
 *  				- Reloj Principal:  MCLK  = DCO    ~ 20 MHz.
 *  				- Reloj Secundario: SMCLK = DCO    ~ 20 MHz.
 *  				- Reloj Auxiliar: 	ACLK  = VLOCLK ~ 8.3 kHz ( datasheet msp430FR5739 5.14. Internal Very-Low-Power Low-Frequency Oscillator ).
 *  \author    Manuel Caballero
 *  \version   0.0
 *  \date      20/3/2015
 */
void conf_CLK (void)
{  
  CSCTL0_H	 =	CSKEY_H;				// Unlock register
  CSCTL1     =	DCORSEL + DCOFSEL_1;	// DC0 ~ 20 MHz
  CSCTL3	 =	DIVS_0 + DIVM_0;
  CSCTL2    |=	SELA_1;					// ACLK = VLOCLK
  CSCTL0_H 	 =	0x01;					// Lock register
}



/**
 *  \brief     void conf_WDT (void)
 *  \details   Desactiva el Watchdog del MCU.
 *  \author    Manuel Caballero
 *  \version   0.0
 *  \date      2/2/2015
 */
void conf_WDT (void)
{
  WDTCTL = WDTPW + WDTHOLD;     // Stop watchdog timer
}



/**
 *  \brief     void conf_IO (void)
 *  \details   Configura los pines del MCU a usar en este proyecto.
 *  				- Puerto 2: UART.
 *  					- P2.0: Salida  ( TX UART ).
 *  					- P2.1: Entrada ( RX UART ).
 *  				- Puerto 3: LEDs.
 *  					- P3.4: Salida ( LED5 ).
 *  					- P3.5: Salida ( LED6 ).
 *  					- P3.6: Salida ( LED7 ).
 *  					- P3.7: Salida ( LED8 ).
 *  				- Puerto J: LEDs.
 *  					- PJ.0: Salida ( LED1 ).
 *  					- PJ.1: Salida ( LED2 ).
 *  					- PJ.2: Salida ( LED3 ).
 *  					- PJ.3: Salida ( LED4 ).
 *  \author    Manuel Caballero
 *  \version   0.0
 *  \date      11/5/2015
 */
void conf_IO (void)
{
	P3OUT	&=	~( BIT6 + BIT7 + BIT5 + BIT4 );		// reset PINs
	P3DIR	|= 	 BIT6 + BIT7 + BIT5 + BIT4;

	PJOUT	&= 	~( BIT0 + BIT1 + BIT2 + BIT3 );		// reset PINs
	PJDIR	|=	 BIT0 + BIT1 + BIT2 + BIT3;


	  // Configure UART pins P2.0 ( Tx ) & P2.1 ( Rx )
	P2SEL1 	|= 	 BIT0 + BIT1;
	P2SEL0 	&= 	~( BIT0 + BIT1 );
}



/**
 *  \brief     void conf_UART (void)
 *  \details   Configura el módulo UART a 115200 Baud Rate.
 *
 * 		· Reloj UART: SMCLK ~ 20 MHz.
 * 		· Buad Rate ~ 115200:
 *
 *     		N = f_BRCLK/BaudRate = 20MHz/115200 ~ 173.61 = {Parte entera} = 173
 *
 *      	N >= 16 -->  Oversampling ON (UCOS16 = 1)
 *
 * 		Por lo tanto:
 *
 *     		UCBRx = INT(N/16) = INT(173/16) = 10
 *     		UCBRFx = ROUND[((N/16) - INT(N/16))·16] = ROUND[((20MHz/115200)/16 - INT((20MHz/115200)/16))·16] ~ 13.61 = 14
 *
 * 		· Activamos UART
 * 	\pre	   Para ampliar información: Documentos slau272c.pdf, apartado 18 Enhanced Universal Serial Communication
 *			   Interface ( eUSCI ), concretamente 18.3.10 Setting a Baud Rate, se pueden encontrar datos tabulados a distintas frecuencias.
 * 	\pre	   El reloj SMCLK debe estar a 20 MHz.
 *  \author    Manuel Caballero
 *  \version   0.0
 *  \date      2/2/2015
 */
void conf_UART (void)
{
	UCA0CTLW0	|=	 UCSWRST;
	UCA0CTLW0	|=	 UCSSEL_2;					// SMCLK ~ 20MHz

	UCA0BRW		 =	 10;						// 20MHz 115200 (Oversampling)
	UCA0MCTLW	 =	 0xADE1;					// UCBRS = 0xAD , UCBRF = 14 ( 0x0E ) and UCOS16 = 1

	UCA0CTLW0	&=	~UCSWRST;					// **Initialize USCI state machine**

	// UCA0IFG		&=	~( UCRXIFG );				// reset flag
	//UCA0IE		|=	 UCRXIE;					// Rx interrupt enabled
}



/**
 *  \brief     void conf_ADC10 (void)
 *  \details   Configura ADC10 para la lectura del sensor externo de temperatura LM35
 *  		   con repetición por el canal A0.
 *
 *  		   El ADC10 se configurará en 8-bit de resolución.
 *
 * 			   Según la hoja de características de esta familia
 *			   de microcontroladores, el documento: slau272c.pdf, la lectura binaria que
 *			   obtendremos en el registro ADC10MEM evndrá dada por la siguiente expresión:
 *
 *			     N_ADC = 255·(V_IN - V_R-)/(V_R+ - V_R-)
 *
 *			   Teniendo en cuenta nuestra configuración: V_R+ = V_AVCC ~ 3.6 V y V_R- = 0 V, tendremos
 *			   una lectura analógica tal y como se expresa a continuación:
 *
 *			     V_IN = (3.6/255)·N_ADC ~ N_ADC·0.014118
 *
 * 	\pre	   Voltajes de referencia:
 * 					· Vref+ = V_AVCC ~ 3.6 V
 * 					· Vref- = Vss.
 * 	\pre	   Hay que asegurarse que el tiempo de muestreo ( t_Sample ) sea mayor a 2us @3V:
 * 				    · Reloj interno: ADC10OSC ( MODOSC ): 4.5 MHz
 * 				    · Tiempo muestreo en 4CLK: 4/(~4.5/3 MHz) ~ 2.66us -> Cumple la especificación.
 *  \author    Manuel Caballero
 *  \version   0.0
 *  \date      11/5/2015
 */
void conf_ADC10 (void)
{
  uint16_t degC = 0;

  ADC10CTL0 	&=	~ADC10ENC;
  ADC10CTL1  	 =   ADC10DIV_3 | ADC10SHP | ADC10SSEL_0 | ADC10CONSEQ_2;		// f_ADC10 ~ 4.5/3 MHz, Repeat-single-channel
  ADC10CTL2		&=	~ADC10RES;													// ADC10 8-bit
  ADC10MCTL0 	 =	 ADC10SREF_0 | ADC10INCH_0;
  ADC10CTL0 	 =   ADC10SHT_0 | ADC10MSC | ADC10ON;							// 4/(4.5/3 MHz), ADC10 enabled

  for(degC = 30; degC > 0; degC-- );											// delay to allow reference to settle
}



/**
 *  \brief     void conf_TimerA (void)
 *  \details   Configura el timer TA0 como Up Mode.
 *
 * 			   El TA0 será encargado de despertar al MCU del estado
 * 			   bajo consumo en, aproximadamente 3 s (24900/(~8.3kHz) ~ 3s).
 *
 * 	\pre	   El reloj ACLK debe estar a 8.3 kHz.
 *  \author    Manuel Caballero
 *  \version   0.0
 *  \date      11/5/2015
 */
void conf_TA0 (void)
{
  TA0CCR0 = 24900;		                	// TAIFG on around ~ 3s
  TA0CTL  = TASSEL_1 + MC_1 + TACLR + TAIE;	// ACLK, upmode, TA0 interrupt ON
}


/**
 *  \brief     void conf_DMA (void)
 *  \details   Configura dos canales de DMA.
 *
 * 			   El canal 0, será para recoger los datos de conversión del ADC10.
 * 			   El canal 1, transmitirá dichos datos por la UART.
 *
 *
 * 	\pre	   En este caso, tanto la fuente como el destino es tamaño BYTE en
 * 			   ambos canales.
 *  \author    Manuel Caballero
 *  \version   0.0
 *  \date      12/5/2015
 */
void conf_DMA (void)
{
	// DMA Channel 0: ADC10_B
	DMACTL0	  =	 DMA0TSEL_26;								// ADC10IFG Tigger
	DMA0SA	  =	 ADC10MEM0_;								// Source address
	DMA0DA 	  =	 ( uint16_t ) &ADC10_Buffer[0];				// Destination address
	DMA0SZ 	  =	 3;
	DMA0CTL	  =	 DMADT_0 | DMADSTINCR_3 | DMAIE | DMAEN | DMALEVEL | DMADSTBYTE | DMASRCBYTE;

	// DMA Channel 1: UART
	DMACTL0	 |=	 DMA1TSEL_15;								// UCA0TXIFG Tigger
	DMA1SA	  =	 ( uint16_t ) &ADC10_Buffer[0];				// Source address
	DMA1DA 	  =	 UCA0TXBUF_;								// Destination address
	DMA1SZ 	  =	 3;
	DMA1CTL	  =	 DMADT_0 | DMASRCINCR_3 | DMAIE | DMALEVEL | DMADSTBYTE | DMASRCBYTE;
}
