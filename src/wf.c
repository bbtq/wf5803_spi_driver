/*
 * wf.c
 *
 *  Created on: 2024年4月22日
 *      Author: Admin
 */
#include <debug.h>
#include "myspi.h"
#include <string.h>

void WFSensor_WriteReg(uint8_t Address, uint8_t value)
    {
        MySPI_Start();
        MySPI_SwapByte(0x00);
        MySPI_SwapByte(Address);
        MySPI_SwapByte(value);
        MySPI_Stop();
    }

uint8_t  WFSensor_ReadReg(uint8_t  addr)
    {
        uint8_t value;
        MySPI_Start();
        MySPI_SwapByte(0x80);//SPI写地址命令
        MySPI_SwapByte(addr);
        value = MySPI_SwapByte(0xFF);//SPI读数�?
        MySPI_Stop();
        return value;
    }

void wf_init(void)
    {
        MySPI_Init();
        MySPI_Stop();

        WFSensor_WriteReg(0x00, 0x01);
    }

uint8_t RawData[5];
int8_t* GetSensorData()
{
  uint8_t buf[5];
  uint8_t error = 0;
  //重置数据
  memset(RawData, 0, sizeof(uint8_t) * 5);
  //采集温度数据
  WFSensor_WriteReg(0x30, 0x08);
  Delay_Ms(50);
  RawData[3] = WFSensor_ReadReg(0x09);
  RawData[4] = WFSensor_ReadReg(0x0A);

 /* WFSensor_ReadRegMulti(0x0A,2,buf);
  RawData[3] = buf[1];
  RawData[4] = buf[0];*/
  //采集气压数据
  WFSensor_WriteReg(0x30, 0x09);
  Delay_Ms(50);
  RawData[0] = WFSensor_ReadReg(0x06);
  RawData[1] = WFSensor_ReadReg(0x07);
  RawData[2] = WFSensor_ReadReg(0x08);

  /* WFSensor_ReadRegMulti(0x08,3,buf);
  RawData[0] = buf[2];
  RawData[1] = buf[1];
  RawData[2] = buf[0];*/
/*
  WFSensor_ReadRegMulti(0x0A,5,buf);
  RawData[0] = buf[4];
  RawData[1] = buf[3];
  RawData[2] = buf[2];
  RawData[3] = buf[1];
  RawData[4] = buf[0];*/
  for(int i=0;i<5;i++){
    //printf(RawData[i], HEX);
    //printf(",");
  }
  return RawData;
}
