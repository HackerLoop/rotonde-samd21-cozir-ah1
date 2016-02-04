/*
* cozir_ah_1.c
*
* Created: 28/01/2016 18:03:22
*  Author: GroundLayer
*/

#include "modules/modules/rotonde-samd21-cozir-ah1/drivers/cozir_ah_1.h"
#include "modules/modules/rotonde-samd21-cozir-ah1/modules/cozir_ah_1.h"

#include "registrar/registrar.h"
#include "tasks/modules.h"

#define MODULE_NAME "COZIR_AH_1_TEMP_HUMI_CO2"
#define MODULE_DATA_SIZE 4
#define MODULE_INDEX "index"
#define MODULE_TEMP "temperature"
#define MODULE_HUMIDITY "humidity"
#define MODULE_CO2 "co2"

#define N_COZIR 1

struct usart_module COZIR_uart[N_COZIR];

data_name COZIR_AH_1_names[MODULE_DATA_SIZE] = {MODULE_INDEX , MODULE_TEMP , MODULE_HUMIDITY , MODULE_CO2 };
data_type COZIR_AH_1_types[] = { INT , FLOAT , FLOAT , FLOAT};

void COZIR_AH_1_tick(void);
definition *COZIR_AH_1_def;

void COZIR_AH_1_module_init(void){
	struct usart_config usart_conf;
	
	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = USART_RX_3_TX_2_XCK_3;
	usart_conf.pinmux_pad0 = PINMUX_UNUSED;
	usart_conf.pinmux_pad1 = PINMUX_UNUSED;
	usart_conf.pinmux_pad2 = PINMUX_PA18C_SERCOM1_PAD2;
	usart_conf.pinmux_pad3 = PINMUX_PA19C_SERCOM1_PAD3;
	usart_conf.baudrate    = 9600;
	COZIR_uart[0] = COZIR_init(usart_conf, SERCOM1);
	
	COZIR_AH_1_def = register_definition(EVENT , MODULE_NAME , COZIR_AH_1_names ,COZIR_AH_1_types , MODULE_DATA_SIZE , NULL, COZIR_AH_1_tick , 5000);
}

void COZIR_AH_1_tick(void) {
	
	for(int i = 0 ; i < N_COZIR ; i++){
		float CO2 , temp , humi;
		
		if(COZIR_read(COZIR_uart[i], &CO2 , &temp , &humi)) {
			instance inst = create_instance(MODULE_NAME);
			if(inst.def != NULL && inst.values != NULL){
				(*(int*) inst.values[0]) = i;
				(*(float*) inst.values[1]) = temp;
				(*(float*) inst.values[2]) = humi;
				(*(float*) inst.values[3]) = CO2;
				portBASE_TYPE xStatus = xQueueSendToBack(module_out, &inst , 10 / portTICK_RATE_MS );
				if(xStatus != pdPASS){
					release_instance(inst);
				}
			}
		}
		vTaskDelay(100);
	}
}