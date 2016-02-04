/*
 * cozir_ah_1.c
 *
 * Created: 28/01/2016 15:27:13
 *  Author: GroundLayer
 */ 

#include "cozir_ah_1.h"
#include "string.h"

static 	uint8_t command[5] = "K 2\r\n";
static  uint8_t pool[3] = "Q\r\n";

struct usart_module COZIR_init(const struct usart_config const usart_conf, Sercom *const sercom){

	struct usart_module COZIR_uart;
	while (usart_init(&COZIR_uart, sercom, &usart_conf) != STATUS_OK ) {
		//add a timeout if blocked here
	}
	usart_enable(&COZIR_uart);

	usart_write_buffer_wait(&COZIR_uart , command , 5);
	return COZIR_uart;
}

bool COZIR_read(struct usart_module COZIR_uart, float* CO2 , float* temp , float* humi){
		uint16_t garbage;
		uint16_t counter = 0;
	while(usart_read_wait(&COZIR_uart , &garbage) == STATUS_OK && counter < 50){
		counter++;
	}	
	
	if(counter == 50){
		return false;
	}
	
	char data[34];
	usart_write_buffer_wait(&COZIR_uart , pool , 3);
	usart_read_buffer_wait(&COZIR_uart , (uint8_t*) data , 34);
	
	if(data[1] != 0x48 || data[9] != 0x54 || data[17] != 0x5A || data[25] != 0x7A || data[32] != 0x0D || data[33] != 0x0A ){
		return false;
	}
	
	char temporary[5];
	strncpy(temporary , data + 3 , 5);
	*humi = atof(temporary);
	*humi /= 10;
	strncpy(temporary , data + 11 , 5);
	*temp = atof(temporary);
	*temp -= 1000;
	*temp /= 10;
	strncpy(temporary , data + 19 , 5);
	*CO2 = atof(temporary);
	return true;
}