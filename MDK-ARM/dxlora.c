#include "dxlora.h"
#include "usart.h"
#include "oled.h"


void DXLORA_Init() {
    // 1. 进入AT模式
    char enterAT[] = "+++";
    HAL_UART_Transmit(&huart2, (uint8_t*)enterAT, strlen(enterAT), 100);
	  HAL_UART_Transmit(&huart3, (uint8_t*)enterAT, strlen(enterAT), 100);
    HAL_Delay(1000);  // 等待模块响应

    // 2. 设置定点传输模式
    char setMode[] = "AT+MODE1\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)setMode, strlen(setMode), 100);
	  HAL_UART_Transmit(&huart3, (uint8_t*)setMode, strlen(setMode), 100);
    HAL_Delay(100);

    // 3. 设置速率等级
    char setLevel[] = "AT+LEVEL1\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)setLevel, strlen(setLevel), 100);
	  HAL_UART_Transmit(&huart3, (uint8_t*)setLevel, strlen(setLevel), 100);
    HAL_Delay(100);

    // 4. 设置设备地址（示例：0x0001）
    char setMac[] = "AT+MAC0001\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)setMac, strlen(setMac), 100);
		char setMac1[] = "AT+MAC0002\r\n";
		HAL_UART_Transmit(&huart3, (uint8_t*)setMac1, strlen(setMac1), 100);
    HAL_Delay(100);

    // 5. 设置信道（示例：信道1）
    char setChannel[] = "AT+CHANNEL01\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)setChannel, strlen(setChannel), 100);
		HAL_UART_Transmit(&huart3, (uint8_t*)setChannel, strlen(setChannel), 100);
    HAL_Delay(100);

    // 6. 重启模块使配置生效
    char reset[] = "AT+RESET\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)reset, strlen(reset), 100);
	  HAL_UART_Transmit(&huart3, (uint8_t*)reset, strlen(reset), 100);
    HAL_Delay(1000);  // 等待模块重启
}


void senddata()
{
	uint8_t data[] = "000201aabbcc";
  HAL_UART_Transmit(&huart2, data, sizeof(data)-1, 100);  // 阻塞发送，超时100ms
}

void send(uint8_t inst[])
{

 HAL_UART_Transmit(&huart2, inst, sizeof(inst)-1, 100);  // 阻塞发送，超时100ms
}



