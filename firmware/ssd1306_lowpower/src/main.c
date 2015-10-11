#include <string.h>
#include "stm32f0xx_conf.h"
#include "i2c_func.h"
#include "u8g_arm.h"

#include "threatbutt.h"

//#include "stm32f0xx_type.h"
//#include "stm32f0xx_rcc.h"
//#include "stm32f0xx_gpio.h"
//#include "stm32f0xx_usart.h"
//#include "stm32f0xx_i2c.h"

uint32_t timercounter = 0;

int i = 0;
u8g_t u8g; 

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
//   .GPIO_Mode = GPIO_Mode_AF,
//    .GPIO_OType = GPIO_OType_OD,
//    .GPIO_PuPd = GPIO_PuPd_UP
//};



USART_InitTypeDef USART2_InitStruct;

void USART2_IRQHandler(void){
    GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_SET);
};

void SysTick_Handler(void) {
    timercounter++;
}

void draw(uint8_t pos)
{
  //u8g_SetFont(&u8g, u8g_font_unifont);
  //u8g_DrawStr(&u8g,  5, 12+pos, "Fuck You");
  //u8g_DrawBox(&u8g, 1,1+pos,100,100);
  u8g_DrawXBM(&u8g, pos,0, threatbutt_width, threatbutt_height, threatbutt_bits);
}

int main(void)
{
    //RCC_ClocksTypeDef Clocks;
    uint8_t pos = 0;
     
    SystemInit();
    SystemCoreClockUpdate(); //update the system clock variable
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_Init(GPIOA, &GPIOA_InitStruct);
     
    timercounter = 0;
    //set systick to 1 every 1uS 
    SysTick_Config(SystemCoreClock/8);
    
    GPIO_SetBits(GPIOA, GPIO_Pin_10);
    //GPIO_SetBits(GPIOA, GPIO_Pin_8);
    //GPIO_SetBits(GPIOA, GPIO_Pin_9);

    //init_i2c();
    //I2C_TransferHandling(I2C1, 0x78, 0, I2C_Reload_Mode, I2C_Generate_Stop | I2C_Generate_Start_Write);
    
    static BitAction toggle = Bit_SET;
    //I2C_SlaveAddressConfig(I2C1, 0x78); 
    
    //I2C_MasterRequestConfig(I2C1, I2C_Direction_Transmitter);
    int i =0;
    //init_i2c(); 
    //I2C_WrReg(0x80, 0xAE);
    //I2C_WrReg(0x80, 0x8D);
    //I2C_WrReg(0x80, 0x14);
    //I2C_WrReg(0x80, 0xAF); 
    
    //init the u8g library
    u8g_InitComFn(&u8g,  &u8g_dev_ssd1306_128x64_i2c, u8g_com_hw_i2c_fn);
    //u8g_Begin(&u8g); 
    u8g_SetDefaultForegroundColor(&u8g);
    //u8g_SetContrast(&u8g, 0x01); 
    //u8g_SetDefaultBackgroundColor(&u8g);

    //u8g_SleepOff(&u8g);
           
    //while(1){
    while(1){
        //asm("wfi");  
        
        u8g_FirstPage(&u8g);
        do
        {
          draw(pos);
        } while ( u8g_NextPage(&u8g) );
        GPIO_SetBits(GPIOA, GPIO_Pin_10);
        u8g_Delay(40);
        GPIO_ResetBits(GPIOA, GPIO_Pin_10);
        u8g_Delay(40);
        GPIO_SetBits(GPIOA, GPIO_Pin_9);
        u8g_Delay(40);
        GPIO_ResetBits(GPIOA, GPIO_Pin_9);
        u8g_Delay(40);
        GPIO_SetBits(GPIOA, GPIO_Pin_8);
        u8g_Delay(40);
        GPIO_ResetBits(GPIOA, GPIO_Pin_8);
        u8g_Delay(40);
        
 
 
         /* refresh screen after some delay */
         ///* update position */
         pos -=10;
         //pos &= ;     
        //while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) == SET);

        //I2C_TransferHandling(I2C1, 0x78, 2, I2C_Reload_Mode, I2C_Generate_Start_Write);
        ////while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET);
        ////I2C_SendData(I2C1, 0x78);
        //while(I2C_GetFlagStatus(I2C1, I2C_ISR_TXIS) == RESET)I2C_SendData(I2C1, 0x80);
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
