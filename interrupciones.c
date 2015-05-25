/**
* @file		interrupciones.c
* @author	Manuel Caballero
* @date 	27/4/2015
* @brief 	Interrupciones del sistema.
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
#include "interrupciones.h"



/**
 *  \brief     TA0_ISR(void) TimerA0 (TA0) interrupt service routine
 *  \details   Cada, aproximadamente 3 segundos, se inicializa una petición de lectura
 *  		   del ADC10.
 *
 *			   El servicio de interrupción del TA0 estará deshabilitado hasta
 *			   que se realice la orden prevista y se envíen los datos a través de la UART
 *			   mediante el DMA ( canal 1 ).
 *
 *			   Una vez completada la orden, se volverá a habilitar desde el servicio de
 *			   interrupción del DMA.
 *
 *  \author    Manuel Caballero
 *  \version   0.0
 *  \date      11/5/2015
 */
void TA0_ISR(void)
{
  switch ( __even_in_range ( TA0IV, PC ) ){
	case	0x00:
	// Vector 0:	No interrupts
		break;

	case	0x02:
	// Vector 2:	Capture/Compare 1 ( TAxCCR1 )
		break;

	case	0x04:
	// Vector 4:	Capture/Compare 2 ( TAxCCR2 )
		break;

	case	0x06:
	// Vector 6:	Capture/Compare 3 ( TAxCCR3 )
		break;

	case	0x08:
	// Vector 8:	Capture/Compare 4 ( TAxCCR4 )
		break;

	case	0x0A:
	// Vector A:	Capture/Compare 5 ( TAxCCR5 )
		break;

	case	0x0C:
	// Vector C:	Capture/Compare 6 ( TAxCCR6 )
		break;

	case	0x0E:
	// Vector E:	Timer Overflow ( TAxCTL )
		TA0CTL  	&=	~( TAIFG | TAIE );   			// Reset flag y Servicio de interrupción OFF
		DMA0CTL	 	|=	 DMAEN;							// DMA Channel 0 enabled
		ADC10CTL0 	|=	 ADC10ON;						// ADC10 enabled
		ADC10CTL0 	|=	 ADC10ENC | ADC10SC ;			// Start conversion
		break;

	default:
		break;
	}
}



/**
 *  \brief     void DMA_ISR(void) DMA interrupt service routine
 *  \details   Esta subrutina de interrupción se encarga de guardar los datos leídos
 * 			   por el módulo ADC10 ( canal 0 ) y posteriormente transmitirlos por
 * 			   la UART ( canal 1 ).
 *
 *  \author    Manuel Caballero
 *  \version   0.0
 *  \date      12/5/2015
 */
void DMA_ISR(void)
{
	switch( __even_in_range( DMAIV, 16 ) )
	  {
	    case 0:
	    // No interrupt
	    	break;

	    case 0x02:
	    // Channel 0: DMA0IFG
	    	ADC10CTL0 	&=	~( ADC10ENC | ADC10ON );		// ADC10 disabled
	    	// __nop();					   					// Solo para DEBUG

	    	DMA0CTL	 	&=	 ~DMAEN;						// DMA Channel 0 disabled
	    	DMA1CTL	 	|=	  DMAEN;						// DMA Channel 1 enabled: Tranmite datos por la UART
	    	break;

	    case 0x04:
	    // Channel 1: DMA1IFG
	    	// __nop();										// Solo para DEBUG
	    	DMA1CTL	 	&=	 ~DMAEN;						// DMA Channel 1 disabled

	    // Todos los datos se han transmitido por la UART, empezamos de nuevo!
	    	TA0CTL		|= 	  TAIE;							// TA0 interrupt enabled
	    	break;

	    case 0x06:
	    // Channel 2: DMA2IFG
	    	break;

	    case 0x08:
	    // Channel 3: DMA3IFG
	    	break;

	    /*	MSP430FR5739 only has 3 DMA Channels
	    case 0x0A:
	    // Channel 4: DMA4IFG
	    	break;

	    case 0x0C:
	    // Channel 5: DMA5IFG
	    	break;

	    case 0x0E:
	    // Channel 6: DMA6IFG
	    	break;

	    case 0x10:
	    // Channel 7: DMA7IFG
	    	break;
	    */

	    default:
	    	break;
	}
}
