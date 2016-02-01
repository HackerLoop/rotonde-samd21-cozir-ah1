/*
 * cozir_ah_1.h
 *
 * Created: 28/01/2016 17:37:28
 *  Author: GroundLayer
 */ 


#ifndef COZIR_AH_1_H_
#define COZIR_AH_1_H_

#include "asf.h"

struct usart_module COZIR_init(const struct usart_config const usart_conf, Sercom *const sercom);
bool COZIR_read(struct usart_module COZIR_uart, float* CO2 , float* temp , float* humi);

#endif /* COZIR_AH_1_H_ */