/*
 * wf.h
 *
 *  Created on: 2024Äê4ÔÂ22ÈÕ
 *      Author: Admin
 */

#ifndef USER_WF_H_
#define USER_WF_H_

void WFSensor_WriteReg(uint8_t Address, uint8_t value);
void wf_init(void);
int8_t* GetSensorData();

#endif /* USER_WF_H_ */
