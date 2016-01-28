/*
 * cozir_ah_1.h
 *
 * Created: 21/01/2016 20:29:15
 *  Author: GroundLayer
 */ 


#ifndef COZIR_AH_1_H_
#define COZIR_AH_1_H_

#include "asf.h"

bool COZIR_init(void);
bool COZIR_read(float* CO2 , float* temp , float* humi);

#endif /* COZIR_AH_1_H_ */