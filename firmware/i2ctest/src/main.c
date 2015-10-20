#include <string.h>
#include "stm32f0xx_conf.h"
//#include "stm32f0xx_type.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_i2c.h"


uint32_t timercounter = 0;

int i = 0;

GPIO_InitTypeDef GPIOC_InitStruct = {
    .GPIO_Pin = GPIO_Pin_All,
    .GPIO_Speed = GPIO_Speed_2MHz,
    .GPIO_Mode = GPIO_Mode_OUT
};

GPIO_InitTypeDef GPIOA_InitStruct = {
    .GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10,
    .GPIO_Speed = GPIO_Speed_50MHz,
    .GPIO_Mode = GPIO_Mode_OUT,
    .GPIO_OType = GPIO_OType_PP,
    .GPIO_PuPd = GPIO_PuPd_NOPULL
};

//GPIO_InitTypeDef GPIOB_InitStruct = {
//    .GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7,
//    .GPIO_Speed = GPIO_Speed_2MHz,
//    .GPIO_Mode = GPIO_Mode_AF,
//    .GPIO_OType = GPIO_OType_OD,
//    .GPIO_PuPd = GPIO_PuPd_UP
//};



USART_InitTypeDef USART2_InitStruct;
I2C_InitTypeDef I2C1_InitStruct;

void USART2_IRQHandler(void){
    GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_SET);
};

void SysTick_Handler(void) {
    timercounter++;
    //    //.static BitAction toggle = Bit_SET;
    //    o
    //.GPIO_WriteBit(GPIOC, GPIO_Pin_9, toggle);
    //.if ((timercounter % 100) == 0){ 
    //.if(toggle == Bit_SET){
    //.    //puts("Pin state is ON");
    //.    toggle = Bit_RESET;
    //.}
    //.else {
    //.    //puts("Pin state is OFF");
    //.    toggle = Bit_SET;
    //.}  
    //.}
}

//initialize the i2c periperal
void init_i2c(){
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_1);

    GPIO_InitTypeDef GPIOB_InitStruct = {
        .GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode = GPIO_Mode_AF,
        .GPIO_OType = GPIO_OType_OD,
        .GPIO_PuPd = GPIO_PuPd_UP
    };
    GPIO_Init(GPIOB, &GPIOB_InitStruct);
    GPIO_PinLockConfig(GPIOB, GPIO_PinSource6);
    GPIO_PinLockConfig(GPIOB, GPIO_PinSource7);
    
    I2C_InitTypeDef I2C_InitStructure = {
        .I2C_Timing = 0x20310A0D,
        .I2C_AnalogFilter = I2C_AnalogFilter_Enable,
        .I2C_DigitalFilter = 0x00,
        .I2C_Mode = I2C_Mode_I2C,
        .I2C_OwnAddress1 = 0x00,
        .I2C_Ack = I2C_Ack_Enable,
        .I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit
    };
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);
}

void I2C_PCF_send(int address, int length)
     {

              while ((I2C1->ISR & I2C_ISR_TXE)==0);    //while TXE ==0, buffer is full
                   I2C1->CR2|=(address<<0)|(length<<16)| I2C_CR2_AUTOEND ;    //address SLAVE 7bits
                   I2C1->CR2 &=~ I2C_CR2_RD_WRN;                        //write
                        I2C1->CR2 |= I2C_CR2_START;
                             while ((I2C1->ISR & I2C_ISR_TXE)==0);    //while TXE ==0, buffer is full
                                  I2C1->TXDR=0xff;//example data
                              }


void I2C_WrReg(uint8_t Reg, uint8_t Val){
    //Wait until I2C isn't busy
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) == SET);

	//"Handle" a transfer - The STM32F0 series has a shocking I2C interface...
	//...Regardless! Send the address of the HMC sensor down the I2C Bus and generate
	//a start saying we're going to write one byte. I'll be completely honest,
	//the I2C peripheral doesn't make too much sense to me and a lot of the code is
	//from the Std peripheral library
	I2C_TransferHandling(I2C1, 0x78, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);

	//Ensure the transmit interrupted flag is set
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET);

	//Send the address of the register we wish to write to
	I2C_SendData(I2C1, Reg);

	//Ensure that the transfer complete reload flag is Set, essentially a standard
	//TC flag
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TCR) == RESET);

	//Now that the HMC5883L knows which register we want to write to, send the address
	//again and ensure the I2C peripheral doesn't add any start or stop conditions
	I2C_TransferHandling(I2C1, 0x78, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);

	//Again, wait until the transmit interrupted flag is set
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET);

	//Send the value you wish you write to the register
	I2C_SendData(I2C1, Val);

	//Wait for the stop flag to be set indicating a stop condition has been sent
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET);

	//Clear the stop flag for the next potential transfer
	I2C_ClearFlag(I2C1, I2C_FLAG_STOPF);
}

int main(void)
{
    //RCC_ClocksTypeDef Clocks;
     
    SystemInit();

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_Init(GPIOA, &GPIOA_InitStruct);
     
    timercounter = 0;
    SysTick_Config(SystemCoreClock/48);
    
    GPIO_SetBits(GPIOA, GPIO_Pin_10);

    for(i=0;i<100000000;i++); 
    init_i2c();
    for(i=0;i<100000000;i++); 
    I2C_TransferHandling(I2C1, 0x78, 0, I2C_Reload_Mode, I2C_Generate_Stop | I2C_Generate_Start_Write);
    
    static BitAction toggle = Bit_SET;
    //GPIO_WriteBit(GPIOC, GPIO_Pin_8, toggle);
    I2C_SlaveAddressConfig(I2C1, 0x78); 
    
    I2C_MasterRequestConfig(I2C1, I2C_Direction_Transmitter);
    //I2C_SlaveAddressConfig(I2C1, 0x78);
    int i =0;
    I2C_WrReg(0x80, 0xAE);
    I2C_WrReg(0x80, 0x8D);
    I2C_WrReg(0x80, 0x14);
    I2C_WrReg(0x80, 0xAF); 
    while(1){
        
        //while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) == SET);

        //I2C_TransferHandling(I2C1, 0x78, 2, I2C_Reload_Mode, I2C_Generate_Start_Write);
        ////while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET);
        ////I2C_SendData(I2C1, 0x78);
        //while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET);
        //I2C_SendData(I2C1, 0x80);
        //while(I2C_GetFlagStatus(I2C1, I2C_ISR_TCR) == RESET);
        //I2C_SendData(I2C1, 0xAE);
        //while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET);
        //I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);
        //I2C_TransferHandling(I2C1, 0x78, 2, I2C_Reload_Mode, I2C_Generate_Start_Write);
        //while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET);
        //I2C_SendData(I2C1, 0x78);
        //while(I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET);
        //I2C_SendData(I2C1, 0x80);
        //while(I2C_GetFlagStatus(I2C1, I2C_ISR_TCR) == RESET);
        //I2C_SendData(I2C1, 0x8D);
        //while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET);
        //I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

        //while(I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET);
        //I2C_SendData(I2C1, 0x80);
        //while(I2C_GetFlagStatus(I2C1, I2C_ISR_TCR) == RESET);
        //I2C_SendData(I2C1, 0x14);
        //while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET);
        //I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

        //while(I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET);
        //I2C_SendData(I2C1, 0x80);
        //while(I2C_GetFlagStatus(I2C1, I2C_ISR_TCR) == RESET);
        //I2C_SendData(I2C1, 0xAF);
        //while(I2C_GetFlagStatus(I2C1, I2C_ISR_STOPF) == RESET);
        //I2C_ClearFlag(I2C1, I2C_ICR_STOPCF);

 //while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    //overflow the buffer 
    /* While the bus is busy */
        //while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    
    /* Send START condition */

        /* Test on EV8 and clear it */
    //while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send STOP condition */
                  //    GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
      //    USART_SendData(USART2, 'X'); 
      //  }
      //while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET){
        //overflowbuffer[i++] = (char)USART_ReceiveData(USART2);
        //USART_SendData(USART2, 'X');
        //GPIO_WriteBit(GPIOC, GPIO_Pin_8, toggle);
        //if(toggle == Bit_SET) toggle = Bit_RESET;
        //else toggle = Bit_SET;
        //}
            //    GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
      //    USART_SendData(USART2, 'X'); 
      //  }
      //GPIO_WriteBit(GPIOC, GPIO_Pin_8, toggle);
      //    if(toggle == Bit_SET){
      //      toggle = Bit_RESET;
      //  }
      //    else {
      //      toggle = Bit_SET;
      //    }
        //USART_ClearFlag(USART1, USART_FLAG_RXNE);
    }
}
#ifdef __CC_ARM
// retarget fputc()
int fputc(int c, FILE *stream) {
    return (int)ITM_SendChar((uint32_t)c);
}
#endif
