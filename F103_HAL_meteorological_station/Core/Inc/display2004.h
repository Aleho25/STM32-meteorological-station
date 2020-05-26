/*
 * display2004.h
 *
 *  Created on: May 16, 2020
 *      Author: niche
 */
#include "stm32f1xx_hal.h"
#include <string.h>
#include "I2Cdev.h"

void UARTdev_init(UART_HandleTypeDef * huart);   // функция для хранения номера используемого UART

#define LCD_ADDR (0x27 << 1)       // адрес дисплея, сдвинутый на 1 бит влево (HAL работает с I2C-адресами, сдвинутыми на 1 бит влево)

#define PIN_RS    (1 << 0)         // если на ножке 0, данные воспринимаются как команда, если 1 - как символы для вывода
#define PIN_EN    (1 << 2)         // бит, по изменению сост. которого считывается информация
#define BACKLIGHT (1 << 3)         // управление подсветкой

#define LCD_DELAY_MS 5             // пауза перед высвечиванием символа


void I2C_Scan ();
void I2C_send(uint8_t data, uint8_t flags);
void LCD_SendString(char *str);
void LCD_Setup();

#ifndef INC_DISPLAY2004_H_
#define INC_DISPLAY2004_H_



#endif /* INC_DISPLAY2004_H_ */
