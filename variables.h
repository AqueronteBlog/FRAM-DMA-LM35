/**
* @file		variables.h
* @author	Manuel Caballero
* @date 	27/4/2015
* @brief 	Variables del sistema.
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


/**
 *  \brief     Variables Globales
 *  \details
 *  			- ADC10_Buffer [3]:	Dicha variable es encargada de almacenar los datos leídos del
 *  								módulo ADC10 para, posteriormente, ser transmitidos por la UART.
 */
volatile uint8_t ADC10_Buffer [3] = {0};
