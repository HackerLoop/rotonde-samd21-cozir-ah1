/*
* cozir_ah_1.c
*
* Created: 21/01/2016 20:36:24
*  Author: GroundLayer
*/

#include "modules/drivers/cozir_ah_1.h"
#include "modules/modules/cozir_ah_1.h"

#include "registrar/registrar.h"
#include "tasks/modules.h"

#define MODULE_NAME "COZIR_AH_1_TEMP_HUMI_CO2"
#define MODULE_DATA_SIZE 3
#define MODULE_TEMP "temperature"
#define MODULE_HUMIDITY "humidity"
#define MODULE_CO2 "co2"

data_name COZIR_AH_1_names[MODULE_DATA_SIZE] = { MODULE_TEMP , MODULE_HUMIDITY , MODULE_CO2 };
data_type COZIR_AH_1_types[] = { FLOAT , FLOAT , FLOAT};

void COZIR_AH_1_tick(void);
definition *COZIR_AH_1_def;

void COZIR_AH_1_module_init(void){
	COZIR_init();
	COZIR_AH_1_def = register_definition(EVENT , MODULE_NAME , COZIR_AH_1_names ,COZIR_AH_1_types , MODULE_DATA_SIZE , NULL, COZIR_AH_1_tick , 1000);
}

void COZIR_AH_1_tick(void) {
	float CO2 , temp , humi;
	
	if(COZIR_read(&CO2 , &temp , &humi)) {
		instance inst = create_instance(MODULE_NAME);
		if(inst.def != NULL && inst.values != NULL){
			(*(float*) inst.values[0]) = temp;
			(*(float*) inst.values[1]) = humi;
			(*(float*) inst.values[2]) = CO2;
			portBASE_TYPE xStatus = xQueueSendToBack(module_out, &inst , 10 / portTICK_RATE_MS );
			if(xStatus != pdPASS){
				release_instance(inst);
			}
		}
	}
}