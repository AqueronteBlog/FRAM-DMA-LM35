/**
* @file		interrupciones.h
* @author	Manuel Caballero
* @date 	27/4/2015
* @brief 	Variables de las rutinas de interrupción.
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


// PROTOTIPO DE FUNCIONES
void TA0_ISR(void) 		__attribute__((interrupt(TIMER0_A1_VECTOR)));
void DMA_ISR(void) 		__attribute__((interrupt(DMA_VECTOR)));



// VARIABLES EXTERNAS
extern volatile uint8_t 	ADC10_Buffer [3];
