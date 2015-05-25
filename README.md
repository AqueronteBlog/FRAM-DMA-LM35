# 8-FRAM-DMA-LM35

## DESCRIPCIÓN:

Este programa consiste en realizar tres lecturas consecutivas de datos del sensor externo
de temperatura LM35 cada, aproximadamente 3 segundos, para posteriormente, mandarlas
por la UART.

Pero en este caso, tanto la lectura de los tres valores del ADC10_B como la transferencia
de datos, se realizará por el módulo DMA.

Se asignará dos canales DMA:

* Canal 0: Lectura del resultado del módulo ADC10_B.
* Canal 1: Tranmitir dichos datos por la UART.


Se envían al ordenador por medio de un par de dispositivos XBee usando la UART.

El resto del tiempo, el MCU permanecerá en estado bajo consumo LPM3.

Este firmware está probado para el **MSP430FR5739**.


## Description:

This firmware shows how to get data from the LM35 device and transmit them using the UART. Everything is done using the DMA.


This firmware was used with **MSP430FR5739**.


## Datos Técnicos / Technical Data
* Code Composer Studio, Version: 6.0.1.00104.
* C Compiler, MSP430 GCC GNU v4.9.1.


El proyecto completo se encuentra en la siguiente dirección: [AqueronteBlog Project](http://unbarquero.blogspot.com.es/2015/05/msp430-dma-lm35-msp430fr5739.html)
