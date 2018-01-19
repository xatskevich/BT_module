
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
uint16_t can_on;	//ПС-02
									//ПС-03
									//ПС-01
									//ПС-50
									//ПС-51
									//ПС-52
									//ПС-53
									//ПС-60
									//ПС-61
									//ПС-62
									//ПС-63
									//ПС-56
									//ПС-40
uint8_t pwm_count,
levels;	//0-3 - вода
		//4-7 - пена
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

void write(uint8_t msg){
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
	USART_SendData(USART1, msg);
}
