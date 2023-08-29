
#include "mytask.h"
#include "USART.h"

TaskHandle_t Task1Handle;
static void Task1Function(void *param){
	
	while(1){
		USART_SendData(USART1, 0x80);
	}
}

void Task_Create(void){
	xTaskCreate( Task1Function,Task1Name, 100,NULL,0,&Task1Handle);
	vTaskStartScheduler();
}
