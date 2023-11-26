#if 0
//ʵ��1�����þ�̬�ڴ洴�������� ��FreeRTOSConfig.h����Ҫ��configSUPPORT_STATIC_ALLOCATION��1 )
//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
//Ӳ��
#include "bsp_led.h"
#include "bsp_usart.h"

//ʹ�õ���ջ��TCB��������
static StackType_t AppTaskCreate_Stack[configMINIMAL_STACK_SIZE];
static StackType_t LED_Task_Stack[configMINIMAL_STACK_SIZE];
static StaticTask_t AppTaskCreate_TCB;
static StaticTask_t LED_Task_TCB;
static TaskHandle_t LED_Task_Handle;	
static TaskHandle_t AppTaskCreate_Handle;


/* �������������ջ */
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/* ��ʱ�������ջ */
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];
/* ����������ƿ� */
static StaticTask_t Idle_Task_TCB;	
/* ��ʱ��������ƿ� */
static StaticTask_t Timer_Task_TCB;

/**********  �������� **********/
static void Hardware_Init(void);
static void AppTaskCreate(void);
static void LED_Task(void* pvParameters);
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
																		StackType_t **ppxTimerTaskStackBuffer, 
																		uint32_t *pulTimerTaskStackSize);

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
																	 StackType_t **ppxIdleTaskStackBuffer, 
																	 uint32_t *pulIdleTaskStackSize);


int main(void)
{	
	//Ӳ����ʼ��
  Hardware_Init();
	
	printf("����һ��STM32_FreeRTOS_��̬�����������ʵ�飡\n");
	
	//��̬�ڴ��´�������
	AppTaskCreate_Handle =  xTaskCreateStatic(	(TaskFunction_t) AppTaskCreate,
																					(const char *)  "AppTaskCreate",
																					(uint32_t	) configMINIMAL_STACK_SIZE,
																					 NULL,
																					(UBaseType_t) 2,
																					(StackType_t*) AppTaskCreate_Stack,
																					(StaticTask_t *)&AppTaskCreate_TCB ) ;
	if(NULL != AppTaskCreate_Handle)
	//���õ�����
		vTaskStartScheduler();
	
	while(1)
	{
		printf("ERROR:������δ���óɹ���\n");
	};
}


static void Hardware_Init(void)
{
	/* ע�⣬��FreeRTOS�£������õ��ĸ����飬����һ�μ��� */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	LED_GPIO_Config();

	USART_Config();
}

static void AppTaskCreate(void)
{
  taskENTER_CRITICAL();           //�����ٽ�
	
	//�ڸ������´���LED����
	LED_Task_Handle = xTaskCreateStatic(	(TaskFunction_t) LED_Task,
																				(const char *)  "LED_Task",
																				(uint32_t	) configMINIMAL_STACK_SIZE,
																				 NULL,
																				(UBaseType_t) 3,
																				(StackType_t*) LED_Task_Stack,
																				(StaticTask_t *)&LED_Task_TCB ) ;
	if(NULL != LED_Task_Handle)
		printf("LED_Task�����ɹ��� \n");
	else
		printf("LED_Task����ʧ��...\n");
	vTaskDelete(AppTaskCreate_Handle);

  taskEXIT_CRITICAL();            //�˳��ٽ���
}

static void LED_Task(void* pvParameters)
{
	while(1)
	{
			LED1_ON;
			vTaskDelay(1000);   /* ��ʱ500��tick */
			printf("LED_Task Running,LED1_ON\r\n");
			
			LED1_OFF;     
			vTaskDelay(1000);   /* ��ʱ500��tick */		 		
			printf("LED_Task Running,LED1_OFF\r\n");
	}
}


//��ÿ���������ڴ������Ϣ
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
								   StackType_t **ppxIdleTaskStackBuffer, 
								   uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer=&Idle_Task_TCB;/* ������ƿ��ڴ� */
	*ppxIdleTaskStackBuffer=Idle_Task_Stack;/* �����ջ�ڴ� */
	*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;/* �����ջ��С */
}

//��ȡ��ʱ��������ڴ������Ϣ
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
									StackType_t **ppxTimerTaskStackBuffer, 
									uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer=&Timer_Task_TCB;/* ������ƿ��ڴ� */
	*ppxTimerTaskStackBuffer=Timer_Task_Stack;/* �����ջ�ڴ� */
	*pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;/* �����ջ��С */
}



#elif  0
//ʵ��2�����ö�̬�ڴ洴��������(��FreeRTOSConfig.h����Ҫ��configSUPPORT_STATIC_ALLOCATION��0)
//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
//Ӳ��
#include "bsp_led.h"
#include "bsp_usart.h"

//ʹ�õ���������
//�ڶ�̬�ڴ��£�ֻ��Ҫ������������������ɣ�ջ��tcb���贴��
static TaskHandle_t LED1_Task_Handle;	
static TaskHandle_t LED2_Task_Handle;	
static TaskHandle_t AppTaskCreate_Handle;


/**********  �������� **********/
static void Hardware_Init(void);
static void AppTaskCreate(void);
static void LED1_Task(void* pvParameters);
static void LED2_Task(void* pvParameters);


int main(void)
{	
	BaseType_t xReturn = pdPASS;
	//Ӳ����ʼ��
  Hardware_Init();
	
	//�ڴ�ĳ�ʼ��RTOS�Զ�������ʼ���������û��Լ����ã�������RTT liteos ��Ҫ�û�����
	
	printf("����һ��STM32_FreeRTOS_��̬�����������ʵ�飡\n");
	
	//��̬�ڴ��´�������
	xReturn =  xTaskCreate(	(TaskFunction_t) AppTaskCreate,
													(char *) "AppTaskCreate",
													(uint16_t) 512,
													NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&AppTaskCreate_Handle );
	if(pdPASS == xReturn)
	//���õ�����
		vTaskStartScheduler();
	
	while(1)
	{
		printf("ERROR:������δ���óɹ���\n");
	};
}


static void Hardware_Init(void)
{
	/* ע�⣬��FreeRTOS�£������õ��ĸ����飬����һ�μ��� */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	LED_GPIO_Config();

	USART_Config();
}

static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;
	
  taskENTER_CRITICAL();           //�����ٽ�
	
	//����1
	xReturn = xTaskCreate(	(TaskFunction_t) LED1_Task,
													(char *) "LED1_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 3,
													(TaskHandle_t *)&LED1_Task_Handle );
	if(pdPASS == xReturn)
		printf("LED1_Task�����ɹ��� \n");
	else
		printf("LED1_Task����ʧ��...\n");
	
	//����2
	xReturn = xTaskCreate(	(TaskFunction_t) LED2_Task,
													(char *) "LED2_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&LED2_Task_Handle );
	if(pdPASS == xReturn)
		printf("LED2_Task�����ɹ��� \n");
	else
		printf("LED2_Task����ʧ��...\n");
	

	vTaskDelete(AppTaskCreate_Handle);

  taskEXIT_CRITICAL();            //�˳��ٽ���
}

static void LED1_Task(void* pvParameters)
{
	while(1)
	{
			LED1_ON;
			vTaskDelay(500); 
			printf("LED_Task Running,LED1_ON\r\n");
			
			LED1_OFF;     
			vTaskDelay(500); 	 		
			printf("LED_Task Running,LED1_OFF\r\n");
	}
}

static void LED2_Task(void* pvParameters)
{
	while(1)
	{
			LED2_ON;
			vTaskDelay(1000);   
			printf("LED_Task Running,LED2_ON\r\n");
			
			LED2_OFF;     
			vTaskDelay(1000);  		 		
			printf("LED_Task Running,LED2_OFF\r\n");
	}
}

#elif 0
//ʵ��3�����þ�����ʱ���������� ( ��FreeRTOSConfig.h����Ҫ��configSUPPORT_STATIC_ALLOCATION��0 ) 

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
//Ӳ��
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"

//ʹ�õ���������
//�ڶ�̬�ڴ��£�ֻ��Ҫ������������������ɣ�ջ��tcb���贴��
static TaskHandle_t LED_Task_Handle;	
static TaskHandle_t KEY_Task_Handle;	
static TaskHandle_t AppTaskCreate_Handle;


/**********  �������� **********/
static void Hardware_Init(void);
static void AppTaskCreate(void);
static void LED_Task(void* pvParameters);
static void KEY_Task(void* pvParameters);


int main(void)
{	
	BaseType_t xReturn = pdPASS;
	//Ӳ����ʼ��
  Hardware_Init();
	
	//�ڴ�ĳ�ʼ��RTOS�Զ�������ʼ���������û��Լ����ã�������RTT liteos ��Ҫ�û�����
	
	printf("����һ��STM32_FreeRTOS_������ʱ+��������ͻָ������ʵ�飡\n");
	
	//��̬�ڴ��´�������
	xReturn =  xTaskCreate(	(TaskFunction_t) AppTaskCreate,
													(char *) "AppTaskCreate",
													(uint16_t) 512,
													NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&AppTaskCreate_Handle );
	if(pdPASS == xReturn)
	//���õ�����
		vTaskStartScheduler();
	
	while(1)
	{
		printf("ERROR:������δ���óɹ���\n");
	};
}


static void Hardware_Init(void)
{
	/* ע�⣬��FreeRTOS�£������õ��ĸ����飬����һ�μ��� */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	LED_GPIO_Config();

	USART_Config();
	
	Key_GPIO_Config();
}

static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;
	
  taskENTER_CRITICAL();           //�����ٽ�
	
	//����1
	xReturn = xTaskCreate(	(TaskFunction_t) LED_Task,
													(char *) "LED_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&LED_Task_Handle );
	if(pdPASS == xReturn)
		printf("LED_Task�����ɹ��� \n");
	else
		printf("LED_Task����ʧ��...\n");
	
	//����2
	xReturn = xTaskCreate(	(TaskFunction_t) KEY_Task,
													(char *) "KEY_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&KEY_Task_Handle );

	vTaskDelete(AppTaskCreate_Handle);

  taskEXIT_CRITICAL();            //�˳��ٽ���
}

static void LED_Task(void* pvParameters)
{
	TickType_t pxPreviousWakeTime = xTaskGetTickCount();
	//��ʱ��ת����tick��
	TickType_t xTimeIncrement = pdMS_TO_TICKS(1000);
	while(1)
	{		
		vTaskDelayUntil(&pxPreviousWakeTime,xTimeIncrement );
		LED1_ON;
		printf("LED_Task Running,LED_ON     Time_ON   = %d \r\n",xTaskGetTickCount());

		vTaskDelayUntil(&pxPreviousWakeTime,xTimeIncrement ); 
			LED1_OFF;     	 		
		printf("LED_Task Running,LED_OFF    time_OFF  = %d \r\n",xTaskGetTickCount());
	}
}

static void KEY_Task(void* pvParameters)
{
	
	while(1)
	{
		if(KEY_ON == Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN))
		{
			vTaskSuspend(LED_Task_Handle);
			printf("\r\nLED_Task�Ѿ������� \r\n");
		}
		if(KEY_ON == Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
		{
			vTaskResume(LED_Task_Handle);
			printf("\r\nLED_Task�Ѿ����ָ��� \r\n");
		}
		vTaskDelay(20);
	}
}

#elif 0
//ʵ���ģ���Ϣ�����շ����շ�ʵ��

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//Ӳ��
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"

//ʹ�õ���������
//�ڶ�̬�ڴ��£�ֻ��Ҫ������������������ɣ�ջ��tcb���贴��
static TaskHandle_t AppTaskCreate_Handle;
static QueueHandle_t QUEUE_Task_Handle;
static TaskHandle_t Send_Task_Handle;	
static TaskHandle_t Receive_Task_Handle;	

#define uxQueueLength   4 
#define uxItemSize  		4

/**********  �������� **********/
static void Hardware_Init(void);
static void AppTaskCreate(void);
static void Receive_Task(void* pvParameters);
static void Send_Task(void* pvParameters);
	
int main(void)
{	
	BaseType_t xReturn = pdPASS;
	//Ӳ����ʼ��
  Hardware_Init();
	
	//�ڴ�ĳ�ʼ��RTOS�Զ�������ʼ���������û��Լ����ã�������RTT liteos ��Ҫ�û�����
	
	printf("����һ��STM32_FreeRTOS_��Ϣ���з�����Ϣ��ʵ�飡\n");
	
	//��̬�ڴ��´�������
	xReturn =  xTaskCreate(	(TaskFunction_t) AppTaskCreate,
													(char *) "AppTaskCreate",
													(uint16_t) 512,
													NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&AppTaskCreate_Handle );
	if(pdPASS == xReturn)
	//���õ�����
		vTaskStartScheduler();
	
	while(1)
	{
		printf("ERROR:������δ���óɹ���\n");
	};
}


static void Hardware_Init(void)
{
	/* ע�⣬��FreeRTOS�£������õ��ĸ����飬����һ�μ��� */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	LED_GPIO_Config();

	USART_Config();
	
	Key_GPIO_Config();
}

static void AppTaskCreate(void)
{
	
	BaseType_t xReturn = pdPASS;
	
  taskENTER_CRITICAL();           //�����ٽ�
	
	//������Ϣ����
	QUEUE_Task_Handle = xQueueCreate(uxQueueLength,uxItemSize);
	if(NULL != QUEUE_Task_Handle)
		printf("\r\n��Ϣ���д����ɹ���\r\n");
	
	//�������񴴽�
	xReturn = xTaskCreate(	(TaskFunction_t) Send_Task,
													(char *) "Send_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 3,
													(TaskHandle_t *)&Send_Task_Handle );
	if(pdPASS == xReturn)
		printf("Send_Task�����ɹ��� \n");
	else
		printf("Send_Task����ʧ��...\n");
	
	//�������񴴽�
	xReturn = xTaskCreate(	(TaskFunction_t) Receive_Task,
													(char *) "Receive_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&Receive_Task_Handle );
	if(pdPASS == xReturn)
		printf("Receive_Task�����ɹ��� \n");
	else
		printf("Receive_Task����ʧ��...\n");
	
	vTaskDelete(AppTaskCreate_Handle);

  taskEXIT_CRITICAL();            //�˳��ٽ���
}


static void Send_Task(void* pvParameters)
{
	BaseType_t xReturn;
	uint32_t Send_data1 = 1;
	uint32_t Send_data2 = 2;
	while(1)
	{
		if(KEY_ON == Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN))
		{
			xReturn = xQueueSend(QUEUE_Task_Handle,&Send_data1,0);
			if(pdPASS == xReturn )
			printf("\r\nData1��Ϣ�ѱ����ͣ� \r\n");
		}
		if(KEY_ON == Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
		{
			xQueueSend(QUEUE_Task_Handle,&Send_data2,0);
			if(pdPASS == xReturn )
			printf("\r\nData2��Ϣ�ѱ����ͣ� \r\n");
		}
		vTaskDelay(20);
	}
}

//������Ҫ��������̬��Ϊʲô�˴�û��vTaskDelay?
//��Ϊ���ն�����Ϣ��ʱ��������ղ������ͻ�����û��趨������̬���˴����õ�portMAX_DELAY��32λ���ֵ
static void Receive_Task(void* pvParameters)
{
	BaseType_t xReturn ;
	uint32_t Read_Queue;
	while(1) 
	{
		xReturn = xQueueReceive(QUEUE_Task_Handle,&Read_Queue,portMAX_DELAY);
		if(pdTRUE == xReturn)
		{printf("\r\n���յ�����ϢΪ��%d\r\n",Read_Queue);
			if(Read_Queue == 1)
				LED1_TOGGLE;
			if(Read_Queue == 2 )
				LED2_TOGGLE;
		}
		else 
			printf("\r\nERROR:���յ������ݳ���...\r\n");
	}
}



#elif 0
//ʵ���壺��ֵ�ź�����ͬ��ʵ��

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
//Ӳ��
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"

//ʹ�õ���������
//�ڶ�̬�ڴ��£�ֻ��Ҫ������������������ɣ�ջ��tcb���贴��
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t Send_Task_Handle;	
static TaskHandle_t Receive_Task_Handle;	
static SemaphoreHandle_t BinarySem_Handle;


/**********  �������� **********/
static void Hardware_Init(void);
static void AppTaskCreate(void);
static void Receive_Task(void* pvParameters);
static void Send_Task(void* pvParameters);
	
int main(void)
{	
	BaseType_t xReturn = pdPASS;
	//Ӳ����ʼ��
  Hardware_Init();
	
	//�ڴ�ĳ�ʼ��RTOS�Զ�������ʼ���������û��Լ����ã�������RTT liteos ��Ҫ�û�����
	
	printf("����һ��STM32_FreeRTOS_��Ϣ���з�����Ϣ��ʵ�飡\n");
	
	//��̬�ڴ��´�������
	xReturn =  xTaskCreate(	(TaskFunction_t) AppTaskCreate,
													(char *) "AppTaskCreate",
													(uint16_t) 512,
													NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&AppTaskCreate_Handle );
	if(pdPASS == xReturn)
	//���õ�����
		vTaskStartScheduler();
	
	while(1)
	{
		printf("ERROR:������δ���óɹ���\n");
	};
}



static void AppTaskCreate(void)
{
	
	BaseType_t xReturn = pdPASS;
	
  taskENTER_CRITICAL();           //�����ٽ�
	
	//������ֵ�ź���
	BinarySem_Handle = xSemaphoreCreateBinary();
	if(NULL != BinarySem_Handle)
		printf("\r\n��ֵ�ź��������ɹ���\r\n");
	
	//�������񴴽�
	xReturn = xTaskCreate(	(TaskFunction_t) Send_Task,
													(char *) "Send_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 3,
													(TaskHandle_t *)&Send_Task_Handle );
	if(pdPASS == xReturn)
		printf("Send_Task�����ɹ��� \n");
	else
		printf("Send_Task����ʧ��...\n");
	
	//�������񴴽�
	xReturn = xTaskCreate(	(TaskFunction_t) Receive_Task,
													(char *) "Receive_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&Receive_Task_Handle );
	if(pdPASS == xReturn)
		printf("Receive_Task�����ɹ��� \n");
	else
		printf("Receive_Task����ʧ��...\n");
	
	vTaskDelete(AppTaskCreate_Handle);

  taskEXIT_CRITICAL();            //�˳��ٽ���
}


static void Send_Task(void* pvParameters)
{
	BaseType_t xReturn;
	while(1)
	{
		if(KEY_ON == Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN))
		{
			xReturn = xSemaphoreGive(BinarySem_Handle);
			if(pdPASS == xReturn )
				printf("\r\n��ֵ�ź����ѱ��ͷţ� \r\n");
			else
				printf("\r\n��ֵ�ź����ͷ�ʧ��\r\n");
		}
		if(KEY_ON == Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
		{
			xReturn = xSemaphoreGive(BinarySem_Handle);
			if(pdPASS == xReturn )
				printf("\r\n��ֵ�ź����ѱ��ͷţ� \r\n");
			else
				printf("\r\n��ֵ�ź����ͷ�ʧ��\r\n");
		}
		vTaskDelay(20);
	}
}


static void Receive_Task(void* pvParameters)
{
	BaseType_t xReturn ;
	while(1) 
	{
		//���ղ���ʱ����������״̬�ȴ�
		xReturn = xSemaphoreTake(BinarySem_Handle,portMAX_DELAY);
		if(pdTRUE == xReturn)
			printf("\r\n��ֵ�ź����ѱ����գ�\r\n");
		else	
			printf("\r\n��ֵ�ź�������ʧ�ܣ�\r\n");
		}
}


static void Hardware_Init(void)
{
	/* ע�⣬��FreeRTOS�£������õ��ĸ����飬����һ�μ��� */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	LED_GPIO_Config();

	USART_Config();
	
	Key_GPIO_Config();
}


#elif 1

//ʵ�����������ź���ʵ��(��Ҫ��QUEUE.c�е�configUSE_COUNTING_SEMAPHORES�궨������Ϊ1)

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
//Ӳ��
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"

//ʹ�õ���������
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t Send_Task_Handle;	
static TaskHandle_t Receive_Task_Handle;	
static SemaphoreHandle_t CountSem_Handle;


/**********  �������� **********/
static void Hardware_Init(void);
static void AppTaskCreate(void);
static void Receive_Task(void* pvParameters);
static void Send_Task(void* pvParameters);
	
int main(void)
{	
	BaseType_t xReturn = pdPASS;
	//Ӳ����ʼ��
  Hardware_Init();
	
	//�ڴ�ĳ�ʼ��RTOS�Զ�������ʼ���������û��Լ����ã�������RTT liteos ��Ҫ�û�����
	
	printf("����һ��STM32_FreeRTOS_��Ϣ���з�����Ϣ��ʵ�飡\n");
	
	//��̬�ڴ��´�������
	xReturn =  xTaskCreate(	(TaskFunction_t) AppTaskCreate,
													(char *) "AppTaskCreate",
													(uint16_t) 512,
													NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&AppTaskCreate_Handle );
	if(pdPASS == xReturn)
	//���õ�����
		vTaskStartScheduler();
	
	while(1)
	{
		printf("ERROR:������δ���óɹ���\n");
	};
}



static void AppTaskCreate(void)
{
	
	BaseType_t xReturn = pdPASS;
	
  taskENTER_CRITICAL();           //�����ٽ�
	
	//���������ź���
	CountSem_Handle = xSemaphoreCreateCounting(5,5);
	if(NULL != CountSem_Handle)
		printf("\r\n�����ź��������ɹ���\r\n");
	
	//�������񴴽�
	xReturn = xTaskCreate(	(TaskFunction_t) Send_Task,
													(char *) "Send_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 3,
													(TaskHandle_t *)&Send_Task_Handle );
	if(pdPASS == xReturn)
		printf("Send_Task�����ɹ��� \n");
	else
		printf("Send_Task����ʧ��...\n");
	
	//�������񴴽�
	xReturn = xTaskCreate(	(TaskFunction_t) Receive_Task,
													(char *) "Receive_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&Receive_Task_Handle );
	if(pdPASS == xReturn)
		printf("Receive_Task�����ɹ��� \n");
	else
		printf("Receive_Task����ʧ��...\n");
	
	vTaskDelete(AppTaskCreate_Handle);

  taskEXIT_CRITICAL();            //�˳��ٽ���
}


static void Send_Task(void* pvParameters)
{
	BaseType_t xReturn;
	while(1)
	{

		if(KEY_ON == Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
		{
			//����2����ʱ���ͷ�һ����λ��
			xReturn = xSemaphoreGive(CountSem_Handle);
			if(pdPASS == xReturn )
				printf("\r\nKEY2������,�ͷ�һ��ͣ��λ��\r\n");	
			else
				printf("\r\nKEY2������,ͣ������Ϊ�գ������ͷţ�\r\n");	
		}
		vTaskDelay(20);
	}
}


static void Receive_Task(void* pvParameters)
{
	BaseType_t xReturn ;
	while(1) 
	{
		if(KEY_ON == Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN))
		{
			//����1����ʱ���������ź����Ƿ�ɻ��(���Ƿ����0),�ȴ�ʱ��Ϊ0
			xReturn = xSemaphoreTake(CountSem_Handle,0);
			if(pdPASS == xReturn )
				printf("\r\nKEY1������,�ɹ����뵽ͣ��λ��\r\n");	
			else
				printf("\r\nKEY1������,������˼������ͣ��λ������\r\n");	
		}
		vTaskDelay(20);
	}
}


static void Hardware_Init(void)
{
	/* ע�⣬��FreeRTOS�£������õ��ĸ����飬����һ�μ��� */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	LED_GPIO_Config();

	USART_Config();
	
	Key_GPIO_Config();
}

#endif 
