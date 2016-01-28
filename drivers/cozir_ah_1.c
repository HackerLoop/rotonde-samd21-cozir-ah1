/*
 * cozir_ah_1.c
 *
 * Created: 21/01/2016 20:29:08
 *  Author: GroundLayer
 */ 

#include "modules/drivers/cozir_ah_1.h"
#include "string.h"

void COZIR_buffer_received(struct usart_module *const module);

static struct usart_module COZIR_uart;
static char buffer[50];
static uint16_t get_char_buffer;
static int index_data;

bool COZIR_init(void){
	struct usart_config usart_conf;
	
	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = USART_RX_3_TX_2_XCK_3;
	usart_conf.pinmux_pad0 = PINMUX_UNUSED;
	usart_conf.pinmux_pad1 = PINMUX_UNUSED;
	usart_conf.pinmux_pad2 = PINMUX_PA10C_SERCOM0_PAD2;
	usart_conf.pinmux_pad3 = PINMUX_PA11C_SERCOM0_PAD3;
	usart_conf.baudrate    = 9600;

	while (usart_init(&COZIR_uart, SERCOM0, &usart_conf) != STATUS_OK ) {
		//add a timeout if blocked here
	}
	usart_register_callback(&COZIR_uart, COZIR_buffer_received , USART_CALLBACK_BUFFER_RECEIVED);
	usart_enable_callback(&COZIR_uart, USART_CALLBACK_BUFFER_RECEIVED);

	usart_enable(&COZIR_uart);
	usart_read_job(&COZIR_uart , &get_char_buffer);
	return true;
}

bool COZIR_read(float* CO2 , float* temp , float* humi){
	
	if(index_data < 24){
		return false;
	}
	
	int i = index_data -1;
	while(buffer[i] != '\n' && buffer[i-1] != '\r' && i >= 23){
		i--;
	}
	
	if(buffer[i-24] != 'H' || buffer[i-16] != 'T' || buffer[i-8] != 'Z' || buffer[i-1] != '\r' || buffer[i] != '\n'){
		memset(buffer , 0 , 50);
		index_data = 0;
		usart_read_job(&COZIR_uart , &get_char_buffer);
		return false;
	}
	
	int zero = i -24;
	char temporary[5];
	strncpy(temporary , buffer + zero + 2 , 5);
	*humi = atof(temporary);
	*humi /= 10;
	strncpy(temporary , buffer + zero + 10 , 5);
	*temp = atof(temporary);
	*temp -= 1000;
	*temp /= 10;
	strncpy(temporary , buffer + zero + 18 , 5);
	*CO2 = atof(temporary);
	
	memset(buffer , 0 , 50);
	index_data = 0;
	usart_read_job(&COZIR_uart , &get_char_buffer);
	return true;
}

void COZIR_buffer_received(struct usart_module *const module) {
	buffer[index_data] = (char) get_char_buffer;
	if(index_data < 50){
		index_data++;
		usart_read_job(&COZIR_uart , &get_char_buffer);
	}
}
