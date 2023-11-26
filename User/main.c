#if 0
//实验1：利用静态内存创建单任务（ 在FreeRTOSConfig.h中需要将configSUPPORT_STATIC_ALLOCATION置1 )
//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
//硬件
#include "bsp_led.h"
#include "bsp_usart.h"

//使用到的栈、TCB、任务句柄
static StackType_t AppTaskCreate_Stack[configMINIMAL_STACK_SIZE];
static StackType_t LED_Task_Stack[configMINIMAL_STACK_SIZE];
static StaticTask_t AppTaskCreate_TCB;
static StaticTask_t LED_Task_TCB;
static TaskHandle_t LED_Task_Handle;	
static TaskHandle_t AppTaskCreate_Handle;


/* 空闲任务任务堆栈 */
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
/* 定时器任务堆栈 */
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];
/* 空闲任务控制块 */
static StaticTask_t Idle_Task_TCB;	
/* 定时器任务控制块 */
static StaticTask_t Timer_Task_TCB;

/**********  函数声明 **********/
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
	//硬件初始化
  Hardware_Init();
	
	printf("这是一个STM32_FreeRTOS_静态创建单任务的实验！\n");
	
	//静态内存下创建任务
	AppTaskCreate_Handle =  xTaskCreateStatic(	(TaskFunction_t) AppTaskCreate,
																					(const char *)  "AppTaskCreate",
																					(uint32_t	) configMINIMAL_STACK_SIZE,
																					 NULL,
																					(UBaseType_t) 2,
																					(StackType_t*) AppTaskCreate_Stack,
																					(StaticTask_t *)&AppTaskCreate_TCB ) ;
	if(NULL != AppTaskCreate_Handle)
	//启用调度器
		vTaskStartScheduler();
	
	while(1)
	{
		printf("ERROR:调度器未启用成功！\n");
	};
}


static void Hardware_Init(void)
{
	/* 注意，在FreeRTOS下，均采用第四个分组，设置一次即可 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	LED_GPIO_Config();

	USART_Config();
}

static void AppTaskCreate(void)
{
  taskENTER_CRITICAL();           //进入临界
	
	//在该任务下创建LED任务
	LED_Task_Handle = xTaskCreateStatic(	(TaskFunction_t) LED_Task,
																				(const char *)  "LED_Task",
																				(uint32_t	) configMINIMAL_STACK_SIZE,
																				 NULL,
																				(UBaseType_t) 3,
																				(StackType_t*) LED_Task_Stack,
																				(StaticTask_t *)&LED_Task_TCB ) ;
	if(NULL != LED_Task_Handle)
		printf("LED_Task创建成功！ \n");
	else
		printf("LED_Task创建失败...\n");
	vTaskDelete(AppTaskCreate_Handle);

  taskEXIT_CRITICAL();            //退出临界区
}

static void LED_Task(void* pvParameters)
{
	while(1)
	{
			LED1_ON;
			vTaskDelay(1000);   /* 延时500个tick */
			printf("LED_Task Running,LED1_ON\r\n");
			
			LED1_OFF;     
			vTaskDelay(1000);   /* 延时500个tick */		 		
			printf("LED_Task Running,LED1_OFF\r\n");
	}
}


//获得空闲任务的内存相关信息
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
								   StackType_t **ppxIdleTaskStackBuffer, 
								   uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer=&Idle_Task_TCB;/* 任务控制块内存 */
	*ppxIdleTaskStackBuffer=Idle_Task_Stack;/* 任务堆栈内存 */
	*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;/* 任务堆栈大小 */
}

//获取定时器任务的内存相关信息
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
									StackType_t **ppxTimerTaskStackBuffer, 
									uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer=&Timer_Task_TCB;/* 任务控制块内存 */
	*ppxTimerTaskStackBuffer=Timer_Task_Stack;/* 任务堆栈内存 */
	*pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;/* 任务堆栈大小 */
}



#elif  0
//实验2：利用动态内存创建多任务(在FreeRTOSConfig.h中需要将configSUPPORT_STATIC_ALLOCATION置0)
//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
//硬件
#include "bsp_led.h"
#include "bsp_usart.h"

//使用到的任务句柄
//在动态内存下，只需要创建任务的任务句柄即可，栈、tcb不需创建
static TaskHandle_t LED1_Task_Handle;	
static TaskHandle_t LED2_Task_Handle;	
static TaskHandle_t AppTaskCreate_Handle;


/**********  函数声明 **********/
static void Hardware_Init(void);
static void AppTaskCreate(void);
static void LED1_Task(void* pvParameters);
static void LED2_Task(void* pvParameters);


int main(void)
{	
	BaseType_t xReturn = pdPASS;
	//硬件初始化
  Hardware_Init();
	
	//内存的初始化RTOS自动帮助初始化，无需用户自己配置，若换了RTT liteos 需要用户配置
	
	printf("这是一个STM32_FreeRTOS_动态创建多任务的实验！\n");
	
	//动态内存下创建任务
	xReturn =  xTaskCreate(	(TaskFunction_t) AppTaskCreate,
													(char *) "AppTaskCreate",
													(uint16_t) 512,
													NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&AppTaskCreate_Handle );
	if(pdPASS == xReturn)
	//启用调度器
		vTaskStartScheduler();
	
	while(1)
	{
		printf("ERROR:调度器未启用成功！\n");
	};
}


static void Hardware_Init(void)
{
	/* 注意，在FreeRTOS下，均采用第四个分组，设置一次即可 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	LED_GPIO_Config();

	USART_Config();
}

static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;
	
  taskENTER_CRITICAL();           //进入临界
	
	//任务1
	xReturn = xTaskCreate(	(TaskFunction_t) LED1_Task,
													(char *) "LED1_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 3,
													(TaskHandle_t *)&LED1_Task_Handle );
	if(pdPASS == xReturn)
		printf("LED1_Task创建成功！ \n");
	else
		printf("LED1_Task创建失败...\n");
	
	//任务2
	xReturn = xTaskCreate(	(TaskFunction_t) LED2_Task,
													(char *) "LED2_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&LED2_Task_Handle );
	if(pdPASS == xReturn)
		printf("LED2_Task创建成功！ \n");
	else
		printf("LED2_Task创建失败...\n");
	

	vTaskDelete(AppTaskCreate_Handle);

  taskEXIT_CRITICAL();            //退出临界区
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
//实验3：利用绝对延时创建多任务 ( 在FreeRTOSConfig.h中需要将configSUPPORT_STATIC_ALLOCATION置0 ) 

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
//硬件
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"

//使用到的任务句柄
//在动态内存下，只需要创建任务的任务句柄即可，栈、tcb不需创建
static TaskHandle_t LED_Task_Handle;	
static TaskHandle_t KEY_Task_Handle;	
static TaskHandle_t AppTaskCreate_Handle;


/**********  函数声明 **********/
static void Hardware_Init(void);
static void AppTaskCreate(void);
static void LED_Task(void* pvParameters);
static void KEY_Task(void* pvParameters);


int main(void)
{	
	BaseType_t xReturn = pdPASS;
	//硬件初始化
  Hardware_Init();
	
	//内存的初始化RTOS自动帮助初始化，无需用户自己配置，若换了RTT liteos 需要用户配置
	
	printf("这是一个STM32_FreeRTOS_绝对延时+按键挂起和恢复任务的实验！\n");
	
	//动态内存下创建任务
	xReturn =  xTaskCreate(	(TaskFunction_t) AppTaskCreate,
													(char *) "AppTaskCreate",
													(uint16_t) 512,
													NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&AppTaskCreate_Handle );
	if(pdPASS == xReturn)
	//启用调度器
		vTaskStartScheduler();
	
	while(1)
	{
		printf("ERROR:调度器未启用成功！\n");
	};
}


static void Hardware_Init(void)
{
	/* 注意，在FreeRTOS下，均采用第四个分组，设置一次即可 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	LED_GPIO_Config();

	USART_Config();
	
	Key_GPIO_Config();
}

static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;
	
  taskENTER_CRITICAL();           //进入临界
	
	//任务1
	xReturn = xTaskCreate(	(TaskFunction_t) LED_Task,
													(char *) "LED_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&LED_Task_Handle );
	if(pdPASS == xReturn)
		printf("LED_Task创建成功！ \n");
	else
		printf("LED_Task创建失败...\n");
	
	//任务2
	xReturn = xTaskCreate(	(TaskFunction_t) KEY_Task,
													(char *) "KEY_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&KEY_Task_Handle );

	vTaskDelete(AppTaskCreate_Handle);

  taskEXIT_CRITICAL();            //退出临界区
}

static void LED_Task(void* pvParameters)
{
	TickType_t pxPreviousWakeTime = xTaskGetTickCount();
	//将时间转换成tick数
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
			printf("\r\nLED_Task已经被挂起！ \r\n");
		}
		if(KEY_ON == Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
		{
			vTaskResume(LED_Task_Handle);
			printf("\r\nLED_Task已经被恢复！ \r\n");
		}
		vTaskDelay(20);
	}
}

#elif 0
//实验四：消息队列收发的收发实验

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//硬件
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"

//使用到的任务句柄
//在动态内存下，只需要创建任务的任务句柄即可，栈、tcb不需创建
static TaskHandle_t AppTaskCreate_Handle;
static QueueHandle_t QUEUE_Task_Handle;
static TaskHandle_t Send_Task_Handle;	
static TaskHandle_t Receive_Task_Handle;	

#define uxQueueLength   4 
#define uxItemSize  		4

/**********  函数声明 **********/
static void Hardware_Init(void);
static void AppTaskCreate(void);
static void Receive_Task(void* pvParameters);
static void Send_Task(void* pvParameters);
	
int main(void)
{	
	BaseType_t xReturn = pdPASS;
	//硬件初始化
  Hardware_Init();
	
	//内存的初始化RTOS自动帮助初始化，无需用户自己配置，若换了RTT liteos 需要用户配置
	
	printf("这是一个STM32_FreeRTOS_消息队列发收信息的实验！\n");
	
	//动态内存下创建任务
	xReturn =  xTaskCreate(	(TaskFunction_t) AppTaskCreate,
													(char *) "AppTaskCreate",
													(uint16_t) 512,
													NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&AppTaskCreate_Handle );
	if(pdPASS == xReturn)
	//启用调度器
		vTaskStartScheduler();
	
	while(1)
	{
		printf("ERROR:调度器未启用成功！\n");
	};
}


static void Hardware_Init(void)
{
	/* 注意，在FreeRTOS下，均采用第四个分组，设置一次即可 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	LED_GPIO_Config();

	USART_Config();
	
	Key_GPIO_Config();
}

static void AppTaskCreate(void)
{
	
	BaseType_t xReturn = pdPASS;
	
  taskENTER_CRITICAL();           //进入临界
	
	//创建消息队列
	QUEUE_Task_Handle = xQueueCreate(uxQueueLength,uxItemSize);
	if(NULL != QUEUE_Task_Handle)
		printf("\r\n消息队列创建成功！\r\n");
	
	//发送任务创建
	xReturn = xTaskCreate(	(TaskFunction_t) Send_Task,
													(char *) "Send_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 3,
													(TaskHandle_t *)&Send_Task_Handle );
	if(pdPASS == xReturn)
		printf("Send_Task创建成功！ \n");
	else
		printf("Send_Task创建失败...\n");
	
	//接收任务创建
	xReturn = xTaskCreate(	(TaskFunction_t) Receive_Task,
													(char *) "Receive_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&Receive_Task_Handle );
	if(pdPASS == xReturn)
		printf("Receive_Task创建成功！ \n");
	else
		printf("Receive_Task创建失败...\n");
	
	vTaskDelete(AppTaskCreate_Handle);

  taskEXIT_CRITICAL();            //退出临界区
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
			printf("\r\nData1信息已被发送！ \r\n");
		}
		if(KEY_ON == Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
		{
			xQueueSend(QUEUE_Task_Handle,&Send_data2,0);
			if(pdPASS == xReturn )
			printf("\r\nData2信息已被发送！ \r\n");
		}
		vTaskDelay(20);
	}
}

//任务都需要进入阻塞态，为什么此处没有vTaskDelay?
//因为接收队列消息的时候如果接收不到，就会进入用户设定的阻塞态，此处设置的portMAX_DELAY，32位最大值
static void Receive_Task(void* pvParameters)
{
	BaseType_t xReturn ;
	uint32_t Read_Queue;
	while(1) 
	{
		xReturn = xQueueReceive(QUEUE_Task_Handle,&Read_Queue,portMAX_DELAY);
		if(pdTRUE == xReturn)
		{printf("\r\n接收到的信息为：%d\r\n",Read_Queue);
			if(Read_Queue == 1)
				LED1_TOGGLE;
			if(Read_Queue == 2 )
				LED2_TOGGLE;
		}
		else 
			printf("\r\nERROR:接收到的数据出错...\r\n");
	}
}



#elif 0
//实验五：二值信号量的同步实验

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
//硬件
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"

//使用到的任务句柄
//在动态内存下，只需要创建任务的任务句柄即可，栈、tcb不需创建
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t Send_Task_Handle;	
static TaskHandle_t Receive_Task_Handle;	
static SemaphoreHandle_t BinarySem_Handle;


/**********  函数声明 **********/
static void Hardware_Init(void);
static void AppTaskCreate(void);
static void Receive_Task(void* pvParameters);
static void Send_Task(void* pvParameters);
	
int main(void)
{	
	BaseType_t xReturn = pdPASS;
	//硬件初始化
  Hardware_Init();
	
	//内存的初始化RTOS自动帮助初始化，无需用户自己配置，若换了RTT liteos 需要用户配置
	
	printf("这是一个STM32_FreeRTOS_消息队列发收信息的实验！\n");
	
	//动态内存下创建任务
	xReturn =  xTaskCreate(	(TaskFunction_t) AppTaskCreate,
													(char *) "AppTaskCreate",
													(uint16_t) 512,
													NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&AppTaskCreate_Handle );
	if(pdPASS == xReturn)
	//启用调度器
		vTaskStartScheduler();
	
	while(1)
	{
		printf("ERROR:调度器未启用成功！\n");
	};
}



static void AppTaskCreate(void)
{
	
	BaseType_t xReturn = pdPASS;
	
  taskENTER_CRITICAL();           //进入临界
	
	//创建二值信号量
	BinarySem_Handle = xSemaphoreCreateBinary();
	if(NULL != BinarySem_Handle)
		printf("\r\n二值信号量创建成功！\r\n");
	
	//发送任务创建
	xReturn = xTaskCreate(	(TaskFunction_t) Send_Task,
													(char *) "Send_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 3,
													(TaskHandle_t *)&Send_Task_Handle );
	if(pdPASS == xReturn)
		printf("Send_Task创建成功！ \n");
	else
		printf("Send_Task创建失败...\n");
	
	//接收任务创建
	xReturn = xTaskCreate(	(TaskFunction_t) Receive_Task,
													(char *) "Receive_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&Receive_Task_Handle );
	if(pdPASS == xReturn)
		printf("Receive_Task创建成功！ \n");
	else
		printf("Receive_Task创建失败...\n");
	
	vTaskDelete(AppTaskCreate_Handle);

  taskEXIT_CRITICAL();            //退出临界区
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
				printf("\r\n二值信号量已被释放！ \r\n");
			else
				printf("\r\n二值信号量释放失败\r\n");
		}
		if(KEY_ON == Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
		{
			xReturn = xSemaphoreGive(BinarySem_Handle);
			if(pdPASS == xReturn )
				printf("\r\n二值信号量已被释放！ \r\n");
			else
				printf("\r\n二值信号量释放失败\r\n");
		}
		vTaskDelay(20);
	}
}


static void Receive_Task(void* pvParameters)
{
	BaseType_t xReturn ;
	while(1) 
	{
		//接收不到时，进入阻塞状态等待
		xReturn = xSemaphoreTake(BinarySem_Handle,portMAX_DELAY);
		if(pdTRUE == xReturn)
			printf("\r\n二值信号量已被接收！\r\n");
		else	
			printf("\r\n二值信号量接收失败！\r\n");
		}
}


static void Hardware_Init(void)
{
	/* 注意，在FreeRTOS下，均采用第四个分组，设置一次即可 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	LED_GPIO_Config();

	USART_Config();
	
	Key_GPIO_Config();
}


#elif 1

//实验六：计数信号量实验(需要将QUEUE.c中的configUSE_COUNTING_SEMAPHORES宏定义设置为1)

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
//硬件
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"

//使用到的任务句柄
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t Send_Task_Handle;	
static TaskHandle_t Receive_Task_Handle;	
static SemaphoreHandle_t CountSem_Handle;


/**********  函数声明 **********/
static void Hardware_Init(void);
static void AppTaskCreate(void);
static void Receive_Task(void* pvParameters);
static void Send_Task(void* pvParameters);
	
int main(void)
{	
	BaseType_t xReturn = pdPASS;
	//硬件初始化
  Hardware_Init();
	
	//内存的初始化RTOS自动帮助初始化，无需用户自己配置，若换了RTT liteos 需要用户配置
	
	printf("这是一个STM32_FreeRTOS_消息队列发收信息的实验！\n");
	
	//动态内存下创建任务
	xReturn =  xTaskCreate(	(TaskFunction_t) AppTaskCreate,
													(char *) "AppTaskCreate",
													(uint16_t) 512,
													NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&AppTaskCreate_Handle );
	if(pdPASS == xReturn)
	//启用调度器
		vTaskStartScheduler();
	
	while(1)
	{
		printf("ERROR:调度器未启用成功！\n");
	};
}



static void AppTaskCreate(void)
{
	
	BaseType_t xReturn = pdPASS;
	
  taskENTER_CRITICAL();           //进入临界
	
	//创建计数信号量
	CountSem_Handle = xSemaphoreCreateCounting(5,5);
	if(NULL != CountSem_Handle)
		printf("\r\n计数信号量创建成功！\r\n");
	
	//发送任务创建
	xReturn = xTaskCreate(	(TaskFunction_t) Send_Task,
													(char *) "Send_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 3,
													(TaskHandle_t *)&Send_Task_Handle );
	if(pdPASS == xReturn)
		printf("Send_Task创建成功！ \n");
	else
		printf("Send_Task创建失败...\n");
	
	//接收任务创建
	xReturn = xTaskCreate(	(TaskFunction_t) Receive_Task,
													(char *) "Receive_Task",
													(uint16_t) 512,
												   NULL,
													(UBaseType_t) 2,
													(TaskHandle_t *)&Receive_Task_Handle );
	if(pdPASS == xReturn)
		printf("Receive_Task创建成功！ \n");
	else
		printf("Receive_Task创建失败...\n");
	
	vTaskDelete(AppTaskCreate_Handle);

  taskEXIT_CRITICAL();            //退出临界区
}


static void Send_Task(void* pvParameters)
{
	BaseType_t xReturn;
	while(1)
	{

		if(KEY_ON == Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
		{
			//按键2按下时，释放一个车位。
			xReturn = xSemaphoreGive(CountSem_Handle);
			if(pdPASS == xReturn )
				printf("\r\nKEY2被按下,释放一个停车位。\r\n");	
			else
				printf("\r\nKEY2被按下,停车场已为空，不能释放！\r\n");	
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
			//按键1按下时，检测计数信号量是否可获得(即是否大于0),等待时间为0
			xReturn = xSemaphoreTake(CountSem_Handle,0);
			if(pdPASS == xReturn )
				printf("\r\nKEY1被按下,成功申请到停车位。\r\n");	
			else
				printf("\r\nKEY1被按下,不好意思，现在停车位已满！\r\n");	
		}
		vTaskDelay(20);
	}
}


static void Hardware_Init(void)
{
	/* 注意，在FreeRTOS下，均采用第四个分组，设置一次即可 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	LED_GPIO_Config();

	USART_Config();
	
	Key_GPIO_Config();
}

#endif 
