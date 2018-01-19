
#include "main.h"

uint16_t light_mask;	//0 - осв отсеков
						//1 - фара-искатель
						//2 - строб
						//3 - двери справа
						//4 - осв справа
						//5 - зад прожектор
						//6 - отсек справа
						//7 - ступень справа
						//8 - задний отсек
						//9 - маяк
						//10 - отсек слева
						//11 - ступень слева
						//12 - осв слева
						//13 - СГУ
						//14 - двери слева
uint16_t light_out;
uint8_t is_idle, left, right;
uint8_t butt, blink, can_on, pwm_count, can_error, light_buf,
levels,	//0-3 - вода
		//4-7 - пена
buttons, butt_mask;	//0 - СГУ
					//1 - освещение слева
					//2 - маяк
					//3 - освещение отсеков
					//4 - задний прожектор
					//5 - освещение справа
					//6 - строб
					//7 - фара-искатель
uint8_t b[16], level[8];

communicate can;

CanTxMsg TxMessage;
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
CAN_InitTypeDef CAN_InitStructure;
CAN_FilterInitTypeDef CAN_FilterInitStructure;
SPI_InitTypeDef SPI_InitStructure;
USART_InitTypeDef USART_InitStructure;

int main(void)
{
	uint32_t temp;

	Init_IWDG();			//сторожевой

	Init_RCC();			//тактирование блоков
	Init_GPIO();		//инициализация портов
	Init_SPI();
	Init_CAN();
	Init_Timer();

    while(1)
    {


    }
}
