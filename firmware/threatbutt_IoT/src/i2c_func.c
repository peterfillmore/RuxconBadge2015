#include "i2c_func.h"

void I2C1_IRQHandler(void){
   //if(I2C_GetFlagStatus(I2C1, I2C_FLAG_NACKF) == SET){
   // I2C_ClearFlag(I2C1, I2C_FLAG_NACKF);
   //}
}

//initialize the i2c periperal
void init_i2c(void){
    //RCC_APBPeriphClockCmd(RCC_APBPeriph_SYSCFG, ENABLE); //enable for i2c fast mode
    //SYSCFG_I2CFastModePlusConfig(SYSCFG_CFGR1_I2C_FMP_PB6|SYSCFG_CFGR1_I2C_FMP_PB7, ENABLE);
        
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
        //.I2C_Timing = 0x20310A0D,
        .I2C_Timing = 0x0010020A,
        .I2C_AnalogFilter = I2C_AnalogFilter_Enable,
        .I2C_DigitalFilter = 0x00,
        .I2C_Mode = I2C_Mode_I2C,
        .I2C_OwnAddress1 = 0x00,
        .I2C_Ack = I2C_Ack_Enable,
        .I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit
    };
    I2C_Init(I2C1, &I2C_InitStructure);
    //I2C_ITConfig(USART1, I2C_IT_NACKI, ENABLE);
    //NVIC_EnableIRQ(I2C1_IRQn);
    I2C_Cmd(I2C1, ENABLE);
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

void i2c_out(uint8_t val){
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
	I2C_SendData(I2C1, val);

	//Ensure that the transfer complete reload flag is Set, essentially a standard
	//TC flag
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TCR) == RESET);

	//Clear the stop flag for the next potential transfer
	I2C_ClearFlag(I2C1, I2C_FLAG_STOPF);
}

void I2C_start(uint8_t i2caddress, uint8_t i2cdirection){
    I2C_SlaveAddressConfig(I2C1, i2caddress); 
    
    I2C_MasterRequestConfig(I2C1, i2cdirection);
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) == SET);
    
	//"Handle" a transfer - The STM32F0 series has a shocking I2C interface...
	//...Regardless! Send the address of the HMC sensor down the I2C Bus and generate
	//a start saying we're going to write one byte. I'll be completely honest,
	//the I2C peripheral doesn't make too much sense to me and a lot of the code is
	//from the Std peripheral library
	I2C_TransferHandling(I2C1, i2caddress, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET);
}



