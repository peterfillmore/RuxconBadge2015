#include <string.h>
#include "stm32f0xx_conf.h"
#include "i2c_func.h"
#include "u8g_arm.h"

#include "threatbutt.h"

uint32_t timercounter = 0;

int i = 0;
int line_valid = 0;
int resp_valid = 0; //check if we're in a valid response
int attacker_valid = 0;
int vector_valid = 0;
int ip_valid = 0;

u8g_t u8g; 

//char rxbuffer[256];
char attacker[128];
char vector[128];
char ip_addr[64];

//bool redledon = false;
//bool orangeledon = false;
//bool greenledon = false;

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

GPIO_InitTypeDef USART1_GPIO_InitStruct = {
    .GPIO_Pin = GPIO_Pin_3,
    .GPIO_Speed = GPIO_Speed_50MHz,
    .GPIO_Mode = GPIO_Mode_AF,
    .GPIO_OType = GPIO_OType_PP,
    .GPIO_PuPd = GPIO_PuPd_UP
};
USART_InitTypeDef USART1_InitStruct = {
    .USART_BaudRate = 9600,
    .USART_WordLength = USART_WordLength_8b,
    .USART_StopBits = USART_StopBits_1,
    .USART_Parity = USART_Parity_No,
    .USART_HardwareFlowControl = USART_HardwareFlowControl_None,
    .USART_Mode = USART_Mode_Rx
};

NVIC_InitTypeDef NVIC_InitStructure = {
    .NVIC_IRQChannel = USART1_IRQn,
    .NVIC_IRQChannelPriority = 0,
    .NVIC_IRQChannelCmd = ENABLE
};


//GPIO_InitTypeDef GPIOB_InitStruct = {
//    .GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7,
//    .GPIO_Speed = GPIO_Speed_2MHz,
//   .GPIO_Mode = GPIO_Mode_AF,
//    .GPIO_OType = GPIO_OType_OD,
//    .GPIO_PuPd = GPIO_PuPd_UP
//};


//void processJSON(char *response, unsigned int len){
//    int i=0;
//    char *comma_ptr;
//    char *colon_ptr; 
//    char attackerstr[] = "\"attacker\"";
//    char vectorstr[] = "\"vector\"";
//
//    while(response[i] != '{')
//       i++;
//    if(response[i] == '{'){
//       comma_ptr = strtok(&(response[i]),",");  
//       while(comma_ptr !=NULL){
//           colon_ptr = strtok(comma_ptr, ":");  
//           while(colon_ptr != NULL){
//                if(strncmp(colon_ptr, attackerstr, sizeof(attackerstr)) == 0){ 
//                    colon_ptr = strtok(NULL, ":");  
//                    strncpy(details.attacker, colon_ptr, sizeof(details.attacker));                              
//                } 
//                else if(strncmp(colon_ptr, vectorstr, sizeof(vectorstr)) == 0){
//                    colon_ptr = strtok(NULL, ":");  
//                    strncpy(details.vector, colon_ptr, sizeof(details.vector));                              
//                } 
//            }
//            comma_ptr = strtok(NULL, ",");
//       }
//    }
//}



void USART1_IRQHandler(void){
     NVIC_DisableIRQ(USART1_IRQn);
     //static int tx_index = 0;
    static int rx_index = 0;
    static int attacker_index = 0;
    static int vector_index = 0;
    static int ip_index = 0;
    static char rx_buffer[256];
    static char at_buf[128];
    static char vec_buf[128];
    static char ip_buf[64];
    if(i==0){ 
        GPIO_SetBits(GPIOA, GPIO_Pin_10);
        i=1;
    }else{
        GPIO_ResetBits(GPIOA, GPIO_Pin_10);
        i=0;
    }
    
    //if(USART_GetITStatus(USART1,USART_IT_TXE)!=RESET)//Transmitthestringinaloop
    //{
    //    USART_SendData(USART1,StringLoop[tx_index++]);
    //
    //    if(tx_index>=(sizeof(StringLoop)-1))
    //        tx_index=0;
    //}
   //while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET){ 
    //while(USART_GetFlagStatus(USART1, USART_IT_RXNE)==SET)//Receivedcharactersmodifystring
    if(USART_GetITStatus(USART1, USART_IT_RXNE)!=RESET)//Receivedcharactersmodifystring
    {
        char rx = USART_ReceiveData(USART1); 
        if(rx == '{'){
           resp_valid = 1;
           ip_valid = 1; 
        }
        else if(rx == ';'){
           resp_valid = 1;
           ip_valid = 0; 
           if(attacker_valid == 1){
                attacker_valid = 0;
                vector_valid = 1;
           }
           else {
               vector_valid = 0;
               attacker_valid = 1;
           }
        } 
        else if(resp_valid && rx == '}'){ //line end
           if(rx_index !=0){
              //memcpy(rxbuffer, rx_buffer, rx_index);
              memcpy(ip_addr, ip_buf, ip_index);
              memcpy(attacker, at_buf, attacker_index);
              memcpy(vector, vec_buf, vector_index);
              memset(rx_buffer, 0x00, sizeof(rx_buffer));
              //rxbuffer[rx_index] = ',';
              //rxbuffer[rx_index] = 0;
              ip_addr[ip_index] = 0;
              attacker[attacker_index] = 0;
              vector[vector_index] = 0;
              rx_index = 0;
              attacker_index = 0;
              vector_index = 0;
              ip_index = 0;
              line_valid = 1;
              resp_valid = 0;
              ip_valid = 0;
              attacker_valid = 0;
              vector_valid = 0;
           }
        }
        else if(resp_valid && attacker_valid){
            if(attacker_index == sizeof(at_buf)){
               attacker_index = 0;
            }
            at_buf[attacker_index++] = rx;
            rx_index++; 
        }
        else if(resp_valid && vector_valid){
            if(vector_index == sizeof(vec_buf)){
               vector_index = 0;
            } 
            vec_buf[vector_index++] = rx;
            rx_index++; 
        }
        else if(resp_valid && ip_valid){
            if(ip_index == sizeof(ip_buf)){
               ip_index = 0;
            } 
            ip_buf[ip_index++] = rx;
            rx_index++; 
        }else { 
            rx_buffer[rx_index++] = rx;
        }
    }
    NVIC_EnableIRQ(USART1_IRQn);
}


void SysTick_Handler(void) {
    static uint16_t tick = 0;
    //switch (tick++) {
  	//    case 50:
    //        GPIO_SetBits(GPIOA, GPIO_Pin_10);
    //        break;
  	//    case 100:
    //        GPIO_SetBits(GPIOA, GPIO_Pin_9);
    //        break;
  	//    case 150:
    //        GPIO_SetBits(GPIOA, GPIO_Pin_8);
  	//        break;
    //    case 200:
    //        GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    //        break;
  	//    case 250:
    //        GPIO_ResetBits(GPIOA, GPIO_Pin_9);
    //        break;
  	//    case 300:
    //        GPIO_ResetBits(GPIOA, GPIO_Pin_10);
  	//        tick = 0; 
    //        break;
    //    default:
    //       break; 
    //}	
    timercounter++;
}

void initUSART1(){
   //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
   
   GPIO_Init(GPIOA, &USART1_GPIO_InitStruct);
   NVIC_Init(&NVIC_InitStructure);
   USART_Init(USART1, &USART1_InitStruct);
   
//enable the interrupt 
   USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
   NVIC_EnableIRQ(USART1_IRQn);
   USART_Cmd(USART1, ENABLE);
   //memset(rxbuffer, 0x00, sizeof(rxbuffer));
}


void draw(uint8_t pos)
{
  //u8g_SetFont(&u8g, u8g_font_unifont);
  //char tokenstr[256]; 
  //u8g_DrawBox(&u8g, 1,1+pos,100,100);
  u8g_DrawXBM(&u8g, pos-64,0, threatbutt_width, threatbutt_height, threatbutt_bits);
  if(line_valid == 1){
      u8g_SetFont(&u8g, u8g_font_profont22r);
      u8g_DrawStr(&u8g, pos, 20, ip_addr);
      u8g_SetFont(&u8g, u8g_font_gdr11r);
      u8g_DrawStr(&u8g, pos, 40, attacker);
      //token = strtok(NULL, ';'); 
       
      u8g_DrawStr(&u8g, pos, 60, vector);
      //while(tokstr != NULL) {
      //  tokstr = strtok(NULL,";");
      //  u8g_DrawStr(&u8g, pos, 40, tokstr);
      //  } 
  }
}

int main(void)
{
    //RCC_ClocksTypeDef Clocks;
    uint8_t pos = 128+64;
     
    SystemInit();
    SystemCoreClockUpdate(); //update the system clock variable
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_Init(GPIOA, &GPIOA_InitStruct);
     
    timercounter = 0;
    //set systick to 1 every 1uS 
    SysTick_Config(SystemCoreClock/8);
    initUSART1();
    GPIO_SetBits(GPIOA, GPIO_Pin_10);
   
    //initialize the watchdog
    
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(0x00);
    //while(IWDG_GetFlagStatus(IWDG_FLAG_PVU)==SET);
    IWDG_SetReload(0xFFFF);
    //while(IWDG_GetFlagStatus(IWDG_FLAG_RVU)==SET);
    //IWDG_SetWindowValue(0x0000);
    //while(IWDG_GetFlagStatus(IWDG_FLAG_PVU)==SET);
    IWDG_ReloadCounter();
    IWDG_Enable();

    static BitAction toggle = Bit_SET;
    
    int i =0;
   
    //init the u8g library
    u8g_InitComFn(&u8g,  &u8g_dev_ssd1306_128x64_i2c, u8g_com_hw_i2c_fn);
    u8g_SetDefaultForegroundColor(&u8g);
           
    while(1){
        u8g_FirstPage(&u8g);
        do
        {
          IWDG_ReloadCounter();
          draw(pos);
        } while ( u8g_NextPage(&u8g) );
         
         /* refresh screen after some delay */
         ///* update position */
        if(pos < 128+128){  
            pos--;
        }else
        { pos = 128+128;
        }
    }
}
  
