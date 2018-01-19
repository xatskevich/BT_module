
#include "main.h"

void USART2_IRQHandler (void)
{
	uint8_t rcv;
	static uint8_t rcv_cnt, rcv_buf[7];
	
	if (USART_GetITStatus(USART2, USART_IT_RXNE)) {
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
	
	rcv = USART_ReceiveData(USART2);
	
	if(rcv_cnt == 0){		//начало передачи
		switch(rcv){
			case 0xAE:	//запрос силовых
			if(can_on & ps40) write(0x40); else write(0);
			if(can_on & ps50) write(0x50); else write(0);
			if(can_on & ps51) write(0x51); else write(0);
			if(can_on & ps52) write(0x52); else write(0);
			if(can_on & ps53) write(0x53); else write(0);
			if(can_on & ps60) write(0x60); else write(0);
			if(can_on & ps61) write(0x61); else write(0);
			if(can_on & ps62) write(0x62); else write(0);
			if(can_on & ps63) write(0x63); else write(0);
			if(can_on & ps56) write(0x56); else write(0);
			break;
			case 0xAC:	//конфиг системы
				rcv_buf[0] = 0xAC;
				rcv_cnt = 6;
			break;
			case 0xAD:	//конфиг силового
				rcv_buf[0] = 0xAD;
				rcv_cnt = 3;
			break;
			case 0xAF:	//передача
				
			break;
		} 
	}else {	//не первый байт по UART
			rcv_buf[rcv_cnt] = rcv;
			rcv_cnt--;
			
			if(rcv_cnt == 0){		//если все байты приняты
				if(rcv_buf[0] == 0xAC){
					TxMessage.IDE = CAN_Id_Standard;
					TxMessage.StdId=0x0AC;
					TxMessage.DLC=6;
					TxMessage.Data[0] = rcv_buf[6];		//
					TxMessage.Data[1] = rcv_buf[5];
					TxMessage.Data[2] = rcv_buf[4];
					TxMessage.Data[3] = rcv_buf[3];		//
					TxMessage.Data[4] = rcv_buf[2];
					TxMessage.Data[5] = rcv_buf[1];
					CAN_Transmit(CAN, &TxMessage);
					if(CAN_GetLastErrorCode(CAN)){		//ошибка отправки

					}
				}
				if(rcv_buf[0] == 0xAD){
					TxMessage.IDE = CAN_Id_Standard;
					TxMessage.StdId=0x0AD;
					TxMessage.DLC=3;
					TxMessage.Data[0] = rcv_buf[3];		//
					TxMessage.Data[1] = rcv_buf[2];
					TxMessage.Data[2] = rcv_buf[1];
					CAN_Transmit(CAN, &TxMessage);
					if(CAN_GetLastErrorCode(CAN)){		//ошибка отправки

					}
				}
			}
		}
}

void Init_SPI(){

	SPI_InitTypeDef SPI_InitStructure;

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx; //полный дуплекс
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b; // передаем по 8 бит
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // Полярность и
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; // фаза тактового сигнала
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; // Управлять состоянием сигнала NSS программно
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; // Предделитель SCK
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; // Первым отправляется старший бит
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; // Режим - мастер
	SPI_Init(SPI1, &SPI_InitStructure); //Настраиваем SPI1
	SPI_Cmd(SPI1, ENABLE); // Включаем модуль SPI1....

	// Поскольку сигнал NSS контролируется программно, установим его в единицу
	// Если сбросить его в ноль, то наш SPI модуль подумает, что
	// у нас мультимастерная топология и его лишили полномочий мастера.
	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
}

void Init_UART(){
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = usart_BR;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}


void Init_GPIO(){

	GPIO_StructInit(&GPIO_InitStructure);

	//цифровые выходы
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//CAN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//подключение портов
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_4);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_4);

	//GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//подключение портов
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
	
	//SPI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//подключение портов
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_0);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_0);
	
	

}
