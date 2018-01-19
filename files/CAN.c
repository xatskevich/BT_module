
#include "main.h"

void CEC_CAN_IRQHandler (void)
{


	if (CAN_GetITStatus(CAN,CAN_IT_FMP0))
	{
		CAN_ClearITPendingBit(CAN, CAN_IT_FMP0);

	}
	CanRxMsg msg_buf;
	CAN_Receive(CAN, CAN_FIFO0, &msg_buf);

	light_mask ^= rear_light_mask;

	switch(msg_buf.StdId){
	case 0x010:		
		can.motor1 = msg_buf.Data[0];
		can.motor2 = msg_buf.Data[1];
		can_on |= ps01;		//индикация наличия пульта
	break;
	case 0x011:	
		can.motor3 = msg_buf.Data[0];
		can_on |= ps03;		//индикация наличия модуля подключения
	break;
	case 0x018:		
		can.level1 = msg_buf.Data[0];
		can.level2 = msg_buf.Data[1];
		can.level3 = msg_buf.Data[2];
		can.level4 = msg_buf.Data[3];
	break;
	case 0x019:		
		can.pressure1 = msg_buf.Data[0];
		can.pressure2 = msg_buf.Data[1];
		can.pressure3 = msg_buf.Data[2];
		can.pressure4 = msg_buf.Data[3];
	break;
	case 0x01A: 	
		can.revolution1 = msg_buf.Data[1]<<8;
		can.revolution1 |= msg_buf.Data[0];
		can.revolution2 = msg_buf.Data[3]<<8;
		can.revolution2 |= msg_buf.Data[2];
	break;
	case 0x028:		
		can.light_comm = msg_buf.Data[2];
		can.light_comm_rear = msg_buf.Data[1];
		can.light_comm_front = msg_buf.Data[0];
	break;
	case 0x040:		
		can.doors = msg_buf.Data[0];
	can_on |= ps40;		//индикация наличия
	break;
	case 0x050:		
		can.roll_left |= 0x3;
		if((msg_buf.Data[0] & 0x3) == 0) can.roll_left &= ~0x3;
		if((msg_buf.Data[0] & 0x3) == 1) can.roll_left &= ~0x2;
		can.stair_left |= 0x3;
		if((msg_buf.Data[0] & 0x3) == 0) can.stair_left &= ~0x3;
		if((msg_buf.Data[0] & 0x3) == 1) can.stair_left &= ~0x2;
		can_on |= ps50;		//индикация наличия
	break;
	case 0x051:		
		can.roll_left |= 0xC;
		if((msg_buf.Data[0] & 0x3) == 0) can.roll_left &= ~0xC;
		if((msg_buf.Data[0] & 0x3) == 1) can.roll_left &= ~0x8;
		can.stair_left |= 0xC;
		if((msg_buf.Data[0] & 0x3) == 0) can.stair_left &= ~0xC;
		if((msg_buf.Data[0] & 0x3) == 1) can.stair_left &= ~0x8;
		can_on |= ps51;		//индикация наличия
	break;
	case 0x052:		
		can.roll_left |= 0x30;
		if((msg_buf.Data[0] & 0x3) == 0) can.roll_left &= ~0x30;
		if((msg_buf.Data[0] & 0x3) == 1) can.roll_left &= ~0x20;
		can.stair_left |= 0x30;
		if((msg_buf.Data[0] & 0x3) == 0) can.stair_left &= ~0x30;
		if((msg_buf.Data[0] & 0x3) == 1) can.stair_left &= ~0x20;
		can_on |= ps52;		//индикация наличия
	break;
	case 0x053:	
		can.roll_left |= 0xC0;
		if((msg_buf.Data[0] & 0x3) == 0) can.roll_left &= ~0xC0;
		if((msg_buf.Data[0] & 0x3) == 1) can.roll_left &= ~0x80;
		can.stair_left |= 0xC0;
		if((msg_buf.Data[0] & 0x3) == 0) can.stair_left &= ~0xC0;
		if((msg_buf.Data[0] & 0x3) == 1) can.stair_left &= ~0x80;
		can_on |= ps53;		//индикация наличия
	break;
	case 0x060:	
		can.roll_right |= 0x3;
		if((msg_buf.Data[0] & 0x3) == 0) can.roll_right &= ~0x3;
		if((msg_buf.Data[0] & 0x3) == 1) can.roll_right &= ~0x2;
		can.stair_right |= 0x3;
		if((msg_buf.Data[0] & 0x3) == 0) can.stair_right &= ~0x3;
		if((msg_buf.Data[0] & 0x3) == 1) can.stair_right &= ~0x2;
		can_on |= ps60;		//индикация наличия
	break;
	case 0x061:	
		can.roll_right |= 0xC;
		if((msg_buf.Data[0] & 0x3) == 0) can.roll_right &= ~0xC;
		if((msg_buf.Data[0] & 0x3) == 1) can.roll_right &= ~0x8;
		can.stair_right |= 0xC;
		if((msg_buf.Data[0] & 0x3) == 0) can.stair_right &= ~0xC;
		if((msg_buf.Data[0] & 0x3) == 1) can.stair_right &= ~0x8;
		can_on |= ps61;		//индикация наличия
	break;
	case 0x062:	
		can.roll_right |= 0x30;
		if((msg_buf.Data[0] & 0x3) == 0) can.roll_right &= ~0x30;
		if((msg_buf.Data[0] & 0x3) == 1) can.roll_right &= ~0x20;
		can.stair_right |= 0x30;
		if((msg_buf.Data[0] & 0x3) == 0) can.stair_right &= ~0x30;
		if((msg_buf.Data[0] & 0x3) == 1) can.stair_right &= ~0x20;
		can_on |= ps62;		//индикация наличия
	break;
	case 0x063:	
		can.roll_right |= 0xC0;
		if((msg_buf.Data[0] & 0x3) == 0) can.roll_right &= ~0xC0;
		if((msg_buf.Data[0] & 0x3) == 1) can.roll_right &= ~0x80;
		can.stair_right |= 0xC0;
		if((msg_buf.Data[0] & 0x3) == 0) can.stair_right &= ~0xC0;
		if((msg_buf.Data[0] & 0x3) == 1) can.stair_right &= ~0x80;
		can_on |= ps63;		//индикация наличия
	break;
	case 0x056:	
		can.roll_rear |= 0x3;
		if((msg_buf.Data[0] & 0x3) == 0) can.roll_rear &= ~0x3;
		if((msg_buf.Data[0] & 0x3) == 1) can.roll_rear &= ~0x2;
		can.stair_rear |= 0x3;
		if((msg_buf.Data[0] & 0x3) == 0) can.stair_rear &= ~0x3;
		if((msg_buf.Data[0] & 0x3) == 1) can.stair_rear &= ~0x2;
	break;
	
	}


}

void roll_inputs(uint8_t msg, uint8_t* roll, uint8_t* stair, uint8_t shift){
	if((msg & 0x3) == 1) *roll |= 1<<shift;		//роллета открыта
	if((msg & 0xC) == 4) *stair |= 1<<shift;		//ступень открыта
	if((msg & 0x3) == 0) *roll &= ~(1<<shift);		//роллета открыта
 	if((msg & 0xC) == 0) *stair &= ~(1<<shift);	//ступень закрыта
	if(*roll & 0xF) light_mask |= left_roll_light_mask;		//индикатор левой роллеты
	else light_mask &= ~(left_roll_light_mask);
	if(*roll & 0xF0) light_mask |= right_roll_light_mask;	//индикатор правой роллеты
	else light_mask &= ~(right_roll_light_mask);
	if(*stair & 0xF) light_mask |= left_stair_light_mask;	//индикатор левой ступени
	else light_mask &= ~(left_stair_light_mask);
	if(*stair & 0xF0) light_mask |= right_stair_light_mask;	//индикатор правой ступени
	else light_mask &= ~(right_stair_light_mask);
}

void Init_CAN(){
	//volatile int i;

	/* CAN register init */
	CAN_DeInit(CAN);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = ENABLE;
	CAN_InitStructure.CAN_AWUM = ENABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;

	/* CAN Baudrate = 250 kBps (CAN clocked at 24 MHz) */
	CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
	CAN_InitStructure.CAN_Prescaler = 6;		//24M / 16 / 6 = 250k
	CAN_Init(CAN, &CAN_InitStructure);
	/* Enable FIFO 0 message pending Interrupt */
	CAN_ITConfig(CAN, CAN_IT_FMP0, ENABLE);
	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	
	//вектор
	NVIC_InitStructure.NVIC_IRQChannel = CEC_CAN_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}
