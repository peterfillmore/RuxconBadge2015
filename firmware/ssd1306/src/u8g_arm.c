#include "stm32f0xx_conf.h"
#include "u8g_arm.h"

//generic arm delay procedure
//
uint32_t control;

static void _delay_system_ticks_sub(uint32_t sys_ticks){
    uint32_t start_val, end_val, curr_val;
    uint32_t load;

    start_val = SysTick->VAL;
    start_val &= 0x0ffffffUL;
    end_val = start_val;

    if(end_val < sys_ticks)
    {
        load = SysTick->LOAD;
        load &=0xffffffUL;
        end_val += load;
    }
    end_val -= sys_ticks;
    
    if(start_val >= end_val){
        for(;;){
            curr_val = SysTick->VAL;
            curr_val &= 0x0ffffffUL;
            if(curr_val <= end_val)
                break;
            if(curr_val > start_val)
                break;
        }
    }
    else{
        for(;;){
            curr_val = SysTick->VAL;
            curr_val &= 0x0ffffffUL;
            if(curr_val <= end_val && curr_val > start_val)
                break;
            }
        }
}

void delay_system_ticks(uint32_t sys_ticks){
    uint32_t load4;
    load4 = SysTick->LOAD;
    load4 &= 0x0ffffffUL;
    load4 >>= 2;
    while(sys_ticks > load4){
        sys_ticks -= load4;
        _delay_system_ticks_sub(load4);
    }
    _delay_system_ticks_sub(sys_ticks);
}

void delay_micro_seconds(uint32_t us){
    uint32_t sys_ticks;
    sys_ticks = SystemCoreClock;
    sys_ticks /= 1000000;
    sys_ticks *= us;
    delay_system_ticks(sys_ticks);
}

void u8g_Delay(uint16_t val){
    delay_micro_seconds(1000UL * (uint32_t)val);
}

void u8g_MicroDelay(void)
{
    delay_micro_seconds(1);
}

void u8g_10MicroDelay(void){
    delay_micro_seconds(10);
}

void u8g_i2c_init(uint8_t options)
{
  init_i2c();
}

/* sla includes also the r/w bit */
uint8_t u8g_i2c_start(uint8_t sla)
{  
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) == SET);
    I2C_TransferHandling(I2C1, sla, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET);
    return 1;
}

uint8_t u8g_i2c_send_byte(uint8_t data)
{
	//Send the address of the register we wish to write to
	I2C_SendData(I2C1, control);

	//Ensure that the transfer complete reload flag is Set, essentially a standard
	//TC flag
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TCR) == RESET);

	//Now that the HMC5883L knows which register we want to write to, send the address
	//again and ensure the I2C peripheral doesn't add any start or stop conditions
	I2C_TransferHandling(I2C1, 0x78, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);

	//Again, wait until the transmit interrupted flag is set
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET);

	//Send the value you wish you write to the register
	I2C_SendData(I2C1, data);
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TCR) == RESET);
    return 1;
}

void u8g_i2c_stop(void)
{
   //Wait for the stop flag to be set indicating a stop condition has been sent
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET);

	//Clear the stop flag for the next potential transfer
	I2C_ClearFlag(I2C1, I2C_FLAG_STOPF); 
}

uint8_t u8g_com_hw_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  register uint8_t *ptr = arg_ptr;
  switch(msg)
  {
    case U8G_COM_MSG_STOP:
        break;
    
    case U8G_COM_MSG_INIT:
        init_i2c(); 
        //I2C_WrReg(0x80, 0xAE);
        //I2C_WrReg(0x80, 0x8D);
        //I2C_WrReg(0x80, 0x14);
        //I2C_WrReg(0x80, 0xAF); 
        u8g_MicroDelay();
        break;
    
    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
        if(arg_val == 0){
            control = 0;  //Send a Command (D/C# pin is low)
        }
        else{
            control = 0x40; //send data 
        }
        u8g_10MicroDelay(); 
        break;

    case U8G_COM_MSG_CHIP_SELECT:
      break;
      
    case U8G_COM_MSG_RESET:
      break;
      
    case U8G_COM_MSG_WRITE_BYTE:
        I2C_WrReg(control, arg_val); 
        //u8g_i2c_start(0x78);
        //u8g_i2c_send_byte(arg_val);
        //u8g_i2c_stop();
        u8g_MicroDelay();
        break;
    case U8G_COM_MSG_WRITE_SEQ:
    case U8G_COM_MSG_WRITE_SEQ_P:
        while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) == SET);
		I2C_TransferHandling(I2C1, 0x78, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
	    //Ensure the transmit interrupted flag is set
	    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET);
	    //Send the address of the register we wish to write to
	    I2C_SendData(I2C1, control);

	    //Ensure that the transfer complete reload flag is Set, essentially a standard
	    //TC flag
	    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TCR) == RESET);
	    I2C_TransferHandling(I2C1, 0x78, arg_val+1, I2C_AutoEnd_Mode, I2C_No_StartStop);
	    //Again, wait until the transmit interrupted flag is set
	    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET);
	    //Send the value you wish you write to the register
        while( arg_val > 0 )
        {
	        I2C_SendData(I2C1, *ptr++);
            arg_val--;
	        while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET);
            //Wait for the stop flag to be set indicating a stop condition has been sent
        }
	    I2C_TransferHandling(I2C1, 0x78, 0, I2C_AutoEnd_Mode, I2C_Generate_Stop);
	    while(I2C_GetFlagStatus(I2C1,I2C_FLAG_STOPF) == RESET);
	    I2C_ClearFlag(I2C1, I2C_FLAG_STOPF); 
        u8g_MicroDelay();
        break;
  }
  return 1;
}
