/**
* @file		main.c
* @author	Manuel Caballero
* @date 	11/5/2015
* @brief 	Archivo principal.
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

#include <msp430.h>
#include <stdint.h>
#include "variables.h"
#include "funciones.h"
#include "interrupciones.h"



/**
 *  \brief     void main( void )
 *  \details   Vamos a trabajar con la placa de desarrollo MSP-EXP430FR5739.
 *
 *  		   Este programa consiste en realizar tres lecturas consecutivas de datos del sensor externo
 *  		   de temperatura LM35 cada, aproximadamente 3 segundos, para posteriormente, mandarlas
 *  		   por la UART.
 *
 *			   Pero en este caso, tanto la lectura de los tres valores del ADC10_B como la transferencia
 *			   de datos, se realizará por el módulo DMA.
 *
 *			   Se asignará dos canales DMA:
 *
 *			   		· Canal 0: Lectura del resultado del módulo ADC10_B.
 *			   		· Canal 1: Tranmitir dichos datos por la UART.
 *
 *
 *  		   Se envían al ordenador por medio de un par de dispositivos XBee usando la UART.
 *
 *  		   El resto del tiempo, el MCU permanecerá en estado bajo consumo LPM3.
 *
 *
 *  \author    Manuel Caballero
 *  \version   0.0
 *  \date      11/5/2015
 *  \pre	   Se utilizarán un par de módulos XBee XB24-ZB ( uno Coordinator y el otro End Device )
 *  		   a 115200 Baudios.
 *  \pre	   El software de ordenador utilizado es el llamado UART1_SW, proporcionado con todos los
 *  		   archivos necesarios del éste proyecto.
 *  \pre	   Code Composer Studio, Version: 6.0.1.00104.
 *  \pre	   C Compiler, MSP430 GCC GNU v4.9.1 ( Red Hat ).
 */
void main( void )
{
  conf_WDT      ();				// Configura WDT del sistema
  conf_CLK      ();				// Configura CLK del sistema
  conf_IO       ();         	// Configura Entradas/Salidas
  conf_ADC10	();				// Configura ADC10
  conf_UART     ();        		// Configura UART
  conf_TA0		();				// Configura Timer A0
  conf_DMA		();				// Configura DMA

  __enable_interrupt();			// Interrupciones ON.


  while(1)
  {
	LPM3;
    // __nop();					   // Solo para DEBUG
  }
}
