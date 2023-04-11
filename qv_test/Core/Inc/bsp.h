#ifndef BSP_H
#define BSP_H



#define BSP_TICKS_PER_SEC    1000U
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA


void BSP_init(void);
void BSP_SysTick_Handler(void);
void BSP_ledOn(void);
void BSP_ledOff(void);
void BSP_Uart_Init();
void BSP_Uart_DeInit();
void BSP_ParseRsp(uint8_t rsp);
void BSP_UART_Receive(void);
void BSP_UART_Send(uint8_t cmd);
void BSP_UART_Error_Handler(void);

//extern QActive *the_Ticker0;

#endif /* BSP_H */
