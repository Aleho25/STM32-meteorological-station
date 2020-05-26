/*
 * display2004.c
 *
 *  Created on: May 16, 2020
 *      Author: niche
 */

#include "display2004.h"
#include <string.h>

static UART_HandleTypeDef * UARTdev_huart;
void UARTdev_init(UART_HandleTypeDef * huart)
{
	UARTdev_huart = huart;
}

extern I2C_HandleTypeDef hi2c1;

/*
void I2C_Scan ()
{
	HAL_StatusTypeDef res;                                                    // переменная, содержащая статус
	char info[] = "Scanning I2C bus...\r\n";                                  // сообщение о начале процедуры
	HAL_UART_Transmit(UARTdev_huart, (uint8_t*)info, strlen(info), HAL_MAX_DELAY);  // отправка сообщения по UART
	/* &huart5 - адрес используемого UART
	 * (uint8_t*)info - указатель на значение для отправки
	 * strlen(info) - длина отправляемого сообщения (ф-ция strlen указывает количество символов)
	 * HAL_MAX_DELAY - задержка
	 */
/*	for(uint16_t i = 0; i < 128; i++)                                         // перебор всех возможных адресов
	{
		res = HAL_I2C_IsDeviceReady(I2Cdev_hi2c, i << 1, 1, HAL_MAX_DELAY);                   // проверяем, готово ли устройство по адресу i для связи
	    if(res == HAL_OK)                                                     // если да, то
	    {
	    	char msg[64];
	    	// преобразование адреса i, на который откликнулись, в строку в виде 16тиричного значения:
	    	snprintf(msg, sizeof(msg), "0x%02X", i);
	    	// отправка номера откликнувшегося адреса
	    	HAL_UART_Transmit(UARTdev_huart, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
	    	// переход на новую строчку
	    	HAL_UART_Transmit(UARTdev_huart, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);
	    }
	    else HAL_UART_Transmit(UARTdev_huart, (uint8_t*)".", 1, HAL_MAX_DELAY);
	}
	HAL_UART_Transmit(UARTdev_huart, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);
} */

void I2C_send(uint8_t data, uint8_t flags)
{
	HAL_StatusTypeDef res;
	    for(;;) {                                                                     // бесконечный цикл
	        res = HAL_I2C_IsDeviceReady(&hi2c1, LCD_ADDR, 1, HAL_MAX_DELAY);          // проверяем, готово ли устройство по адресу lcd_addr для связи
	        if(res == HAL_OK) break;                                                  // если да, то выходим из бесконечного цикла
	    }

	uint8_t up = data & 0xF0;                 // операция И с 1111 0000, приводит к обнулению последних бит с 0 по 3, остаются биты с 4 по 7
	uint8_t lo = (data << 4) & 0xF0;          // тоже самое, но data сдвигается на 4 бита влево, т.е. в этой
	                                           // переменной остаются  биты с 0 по 3
	uint8_t data_arr[4];
	data_arr[0] = up|flags|BACKLIGHT|PIN_EN;  // 4-7 биты содержат информацию, биты 0-3 конфигурируют работу
	data_arr[1] = up|flags|BACKLIGHT;         // ублирование сигнала, на выводе Е в этот раз 0
	data_arr[2] = lo|flags|BACKLIGHT|PIN_EN;
	data_arr[3] = lo|flags|BACKLIGHT;

	HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, data_arr, sizeof(data_arr), HAL_MAX_DELAY);
	HAL_Delay(LCD_DELAY_MS);
}


void LCD_SendString(char *str)
{
    // *char по сути является строкой
	while(*str) {                                   // пока строчка не закончится
		I2C_send((uint8_t)(*str), 1);               // передача первого символа строки
        str++;                                      // сдвиг строки налево на 1 символ
    }
}


void LCD_Setup()
{
	I2C_send(0b00110000,0);   // 8ми битный интерфейс
	I2C_send(0b00000010,0);   // установка курсора в начале строки
	I2C_send(0b00001100,0);   // нормальный режим работы
	I2C_send(0b00000001,0);   // очистка дисплея
}
