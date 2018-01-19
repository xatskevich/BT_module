
#include "main.h"


void TIM14_IRQHandler(void){	//0.777 с

	TIM14->SR &= ~TIM_SR_UIF;	//

	light_mask = 0;
//определение CAN
	if(can_on & ps01) light_mask |= rear_roll_light_mask;
	if(can_on & ps02) light_mask |= sgu_light_mask;
	if(can_on & ps03) light_mask |= search_light_mask;
	if(can_on & ps40) light_mask |= left_door_light_mask | right_door_light_mask;
	
	if(can_on & ps50) set_water_1;
	else reset_water_1;
	if(can_on & ps51) set_water_34;
	else reset_water_34;
	if(can_on & ps52) set_water_12;
	else reset_water_12;
	if(can_on & ps53) set_water_14;
	else reset_water_14;
	if(can_on & ps60) set_foam_1;
	else reset_foam_1;
	if(can_on & ps61) set_foam_34;
	else reset_foam_34;
	if(can_on & ps62) set_foam_12;
	else reset_foam_12;
	if(can_on & ps63) set_foam_14;
	else reset_foam_14;
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
	if (pwm_count >= level[0]) reset_water_14; //отсек слева 4
	else set_water_14;
	if (pwm_count >= level[1]) reset_water_12; //отсек слева 3
	else set_water_12;
	if (pwm_count >= level[2]) reset_water_34; //отсек слева 2
	else set_water_34;
	if (pwm_count >= level[3]) reset_water_1; //отсек слева 1
	else set_water_1;
	if (pwm_count >= level[4]) reset_foam_14; //отсек справа 4
	else set_foam_14;
	if (pwm_count >= level[5]) reset_foam_12; //отсек справа 3
	else set_foam_12;
	if (pwm_count >= level[6]) reset_foam_34; //отсек справа 2
	else set_foam_34;
	if (pwm_count >= level[7]) reset_foam_1; //отсек справа 1
	else set_foam_1;

	SPI_I2S_SendData16(SPI1, light_out);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	set_strobe;
	reset_strobe;
}

void TIM17_IRQHandler(void){	//40 мс
	uint8_t i;


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
	
	//таймер наличия CAN
	TIM14->PSC = sys_clock / 10 - 1;
	TIM14->ARR = 7770;		//РїРµСЂРёРѕРґ 777 РјСЃ
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
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
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
