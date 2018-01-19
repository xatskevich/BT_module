
#include "main.h"

void TIM14_IRQHandler(void){	//0.777 с

	TIM14->SR &= ~TIM_SR_UIF;	//сброс флага

//моргалка
	blink ^= 1;

//анализ CAN
	if (can_on == 0) { 		//CAN сообщений не было
		if (blink) { 		//моргание
			levels = 0xff;
		} else {
			levels = 0;
		}
	}
	can_on = 0;
}

void TIM16_IRQHandler(void){	//1 мс
	uint8_t i;

	TIM16->SR &= ~TIM_SR_UIF;	//сброс флага

//счетчик ШИМ от 0 до 8
		if (++pwm_count == 8) pwm_count = 0;

//ШИМ индикаторов
	for(i=0; i<15; i++){
		if(pwm_count >= b[i]) light_out &= ~(1 << i);
		else light_out |= (1 << i);
	}

//ШИМ уровней
	if (pwm_count >= level[0]) reset_water_14; //вода 1/4
	else set_water_14;
	if (pwm_count >= level[1]) reset_water_12; //вода 1/2
	else set_water_12;
	if (pwm_count >= level[2]) reset_water_34; //вода 3/4
	else set_water_34;
	if (pwm_count >= level[3]) reset_water_1; //вода 1
	else set_water_1;
	if (pwm_count >= level[4]) reset_foam_14; //пена 1/4
	else set_foam_14;
	if (pwm_count >= level[5]) reset_foam_12; //пена 1/2
	else set_foam_12;
	if (pwm_count >= level[6]) reset_foam_34; //пена 3/4
	else set_foam_34;
	if (pwm_count >= level[7]) reset_foam_1; //пена 1
	else set_foam_1;

	SPI_I2S_SendData16(SPI1, light_out);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	set_strobe;
	reset_strobe;
}

void TIM17_IRQHandler(void){	//40 мс
	uint8_t temp, tmp, i;
	static uint8_t can_cnt;

	TIM17->SR &= ~TIM_SR_UIF;	//сброс флага

	IWDG_ReloadCounter();

//изменение скважности ШИМ индикаторов
	for(i=0; i<15; i++){
		if(light_mask & (1<<i)){
			if(b[i] < 8) b[i]++;
		} else {
			if(b[i] > 0) b[i]--;
		}
	}

//изменение скважности ШИМ уровней
		for(i=0; i<8; i++){
			if(levels & (1<<i)){
				if(level[i] < 8) level[i]++;
			} else {
				if(level[i] > 0) level[i]--;
			}
		}



//CAN посылки
	if (++can_cnt == 4) {
		can_cnt = 0;
		tmp = 0xC0; //1 байт
		if (buttons & sgu_mask) tmp |= 0x1;
		if (buttons & search_mask) tmp |= 0x4;
		if (buttons & strobe_mask) tmp |= 0x10;
		TxMessage.Data[0] = tmp;
		tmp = 0xF0;
		if (buttons & beam_mask) tmp |= 0x1;
		if (buttons & rear_mask) tmp |= 0x4;
		TxMessage.Data[1] = tmp;
		tmp = 0xC0;
		if (buttons & left_mask) tmp |= 0x1;
		if (buttons & right_mask) tmp |= 0x4;
		if (buttons & inside_mask) tmp |= 0x10;
		TxMessage.Data[2] = tmp;

		TxMessage.IDE = CAN_Id_Standard;
		TxMessage.StdId = 0x028;
		TxMessage.DLC = 3;
		CAN_Transmit(CAN, &TxMessage);
		if (CAN_GetLastErrorCode(CAN)) { //ошибка отправки
			//GPIO_SetBits(GPIOA, GPIO_Pin_9);
			light_mask ^= 0x540;
		}

	}
}

void delay_ms (uint16_t dl){
	TIM3->CNT = 0;
	TIM3->PSC = 48000;		//период 1мс
	TIM3->ARR = dl;		//
	TIM3->CR1 |= TIM_CR1_CEN;	//запуск таймера
	TIM3->SR &= ~TIM_SR_UIF;	//сброс флага
	while((TIM3->SR&TIM_SR_UIF)==0);
	TIM3->CR1 &= ~TIM_CR1_CEN;
}

void Init_Timer(){

	//таймер контроля CAN
	TIM14->PSC = sys_clock / 10 - 1;
	TIM14->ARR = 7770;		//период 777 мс
	TIM14->DIER |= TIM_DIER_UIE;
	TIM14->CR1 |= TIM_CR1_CEN;

	//таймер
	TIM16->PSC = sys_clock / 100 - 1;
	TIM16->ARR = 100;		//период 1 мс
	TIM16->DIER |= TIM_DIER_UIE;
	TIM16->CR1 |= TIM_CR1_CEN;

	//таймер опроса кнопок
	TIM17->PSC = sys_clock / 100 - 1;
	TIM17->ARR = 4000;		//период 40 мс
	TIM17->DIER |= TIM_DIER_UIE;
	TIM17->CR1 |= TIM_CR1_CEN;

	//вектор
	NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM16_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM17_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}
