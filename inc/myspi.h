/*
 * myspi.h
 *
 *  Created on: 2024Äê4ÔÂ22ÈÕ
 *      Author: Admin
 */

#ifndef USER_MYSPI_H_
#define USER_MYSPI_H_


void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SwapByte(uint8_t ByteSend);

#endif /* USER_MYSPI_H_ */
