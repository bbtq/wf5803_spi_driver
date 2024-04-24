/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *Multiprocessor communication mode routine:
 *Master:USART1_Tx(PD5)\USART1_Rx(PD6).
 *This routine demonstrates that USART1 receives the data sent by CH341 and inverts
 *it and sends it (baud rate 115200).
 *
 *Hardware connection:PD5 -- Rx
 *                     PD6 -- Tx
 *
 */

#include "debug.h"
#include "wf.h"


/* Global define */


/* Global Variable */
vu8 val;

/*********************************************************************
 * @fn      USARTx_CFG
 *
 * @brief   Initializes the USART2 & USART3 peripheral.
 *
 * @return  none
 */
void USARTx_CFG(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

    /* USART1 TX-->D.5   RX-->D.6 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    wf_init();
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif
    printf("SystemClk:%d\r\n",SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    long reading = 0;
    float press = 0;//kPa
    float temp = 0;//�?
    float fDat;
    int a = 1;
    float press_air = 101.0f;

    USARTx_CFG();

    while(1)
    {
        uint8_t* p1 = GetSensorData();
          reading = p1[0];
          reading = reading << 8;
          reading |= p1[1];
          reading = reading << 8;
          reading |= p1[2];
          if (reading >= 8388608) {
            fDat = (int32_t)(reading - 16777216) / 8388608.0f;
          } else {
            fDat = reading / 8388608.0f;
          }
          press = fDat * 125 + 17.5;//WF5803_1BAR
          printf("press= %d.%d \r\n",(int)press,((int)(press*100.0f)%100));
          if (a) {press_air = press ; a = 0;}
                    press = press_air > press ? press_air : press;
                    int deep_100 = (int)(100*((press - press_air)/0.098f));
                    //printf("press = %d.%d   ",(int)press,((int)(press*10000.0f)%10000));
                    printf("deep  = %d.%d cm   ",deep_100/100,deep_100%100);
          //          Serial.print("press=");
          //          Serial.print(press);

                    reading = p1[3];
                    reading = reading << 8;
                    reading |= p1[4];
                    if (reading > 32768)
                    {
                      temp = (reading - 65844) / 256.0f;
                    }
                    else
                    {
                      temp = (reading-308) / 256.0f;
                    }

                    printf(", temp= %d.%d \r\n",(int)temp,((int)(temp*100.0f)%100));

          Delay_Ms(250);
    }
}
