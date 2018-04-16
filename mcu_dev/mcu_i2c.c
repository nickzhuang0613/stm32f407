/**
 * @file            mcu_i2c.c
 * @brief           IOģ��I2C
 * @author          test
 * @date            2017��10��30�� ����һ
 * @version         ����
 * @par             ��Ȩ���� (C), 2013-2023
 * @par History:
 * 1.��    ��:      2017��10��30�� ����һ
 *   ��    ��:      �ݼ�ȸ������
 *   �޸�����:      �����ļ�
    	1 Դ����ݼ�ȸ���������С�
		2 �������ڵ�������ҵ��;�����׿��������۳��⣩��������Ȩ��
		3 �ݼ�ȸ�����Ҳ��Դ��빦�����κα�֤����ʹ�������в��ԣ�����Ը���
		4 �������޸�Դ�벢�ַ���������ֱ�����۱���������������뱣��WUJIQUE��Ȩ˵����
		5 �緢��BUG�����Ż�����ӭ�������¡�����ϵ��code@wujique.com
		6 ʹ�ñ�Դ�����൱����ͬ����Ȩ˵����
		7 ���ַ����Ȩ��������ϵ��code@wujique.com
		8 һ�н���Ȩ���ݼ�ȸ���������С�
*/

#include "stm32f4xx.h"
#include "wujique_log.h"
#include "mcu_i2c.h"


#define MCU_I2C_PORT GPIOD
#define MCU_I2C_SCL GPIO_Pin_6
#define MCU_I2C_SDA GPIO_Pin_7

#define MCU_I2C_TIMEOUT 250

/**
 *@brief:      mcu_i2c_delay
 *@details:    I2C�ź���ʱ����
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     static
 */
static void mcu_i2c_delay(void)
{
    //Delay(1);//��ʱ��I2Cʱ��
    u32 i = 70;

    for(;i>0;i--);
}

/**
 *@brief:      mcu_i2c_sda_input
 *@details:    ��I2C sda IO����Ϊ����
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
void mcu_i2c_sda_input(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = MCU_I2C_SDA;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����ģʽ  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(MCU_I2C_PORT, &GPIO_InitStructure);//��ʼ��   
}
/**
 *@brief:      mcu_i2c_sda_output
 *@details:       ��I2C sda IO����Ϊ���
 *@param[in]  void  
 *@param[out]  ��
 *@retval:     
 */
void mcu_i2c_sda_output(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = MCU_I2C_SDA;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(MCU_I2C_PORT, &GPIO_InitStructure);//��ʼ��

}
/**
 *@brief:      mcu_i2c_readsda
 *@details:    ��SDA����
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     static
 */
static s32 mcu_i2c_readsda(void)
{
    if(Bit_SET == GPIO_ReadInputDataBit(MCU_I2C_PORT, MCU_I2C_SDA))
        return 1;
    else
        return 0;
}
/**
 *@brief:      mcu_i2c_sda
 *@details:       SDA����ߵ͵�ƽ
 *@param[in]  u8 sta  
 *@param[out]  ��
 *@retval:     static
 */
static void mcu_i2c_sda(u8 sta)
{
    if(sta == 1)
    {
        GPIO_SetBits(MCU_I2C_PORT,MCU_I2C_SDA);    
    }
    else if(sta == 0)
    {
        GPIO_ResetBits(MCU_I2C_PORT, MCU_I2C_SDA);
    }
    else
    {
    
    }
}

/**
 *@brief:      mcu_i2c_scl
 *@details:    ʱ��SCL����ߵ͵�ƽ
 *@param[in]   u8 sta  
 *@param[out]  ��
 *@retval:     static
 */
static void mcu_i2c_scl(u8 sta)
{
    if(sta == 1)
    {
        GPIO_SetBits(MCU_I2C_PORT, MCU_I2C_SCL);    
    }
    else if(sta == 0)
    {
        GPIO_ResetBits(MCU_I2C_PORT, MCU_I2C_SCL);
    }
    else
    {
    
    }    
}
/**
 *@brief:      mcu_i2c_start
 *@details:    ����startʱ��
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     static
 */
static void mcu_i2c_start(void)
{
    mcu_i2c_sda_output();
    
    mcu_i2c_sda(1);  
    mcu_i2c_scl(1);

    mcu_i2c_delay();
    mcu_i2c_sda(0);

    mcu_i2c_delay();
    mcu_i2c_scl(0);
}
/**
 *@brief:      mcu_i2c_stop
 *@details:    ����I2C STOPʱ��
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     static
 */
static void mcu_i2c_stop(void)
{
    mcu_i2c_sda_output();

    mcu_i2c_scl(0);
    mcu_i2c_sda(0);   
    mcu_i2c_delay();
    
    mcu_i2c_scl(1);
    mcu_i2c_delay();
    
    mcu_i2c_sda(1);
    mcu_i2c_delay();
}

/**
 *@brief:      mcu_i2c_wait_ack
 *@details:       �ȴ�ACK�ź�
 *@param[in]  void  
 *@param[out]  ��
 *@retval:     static
 */
static s32 mcu_i2c_wait_ack(void)
{
    u8 time_out = 0;
    
    //sdaת����
    mcu_i2c_sda_input();
    mcu_i2c_sda(1);
    mcu_i2c_delay();
    
    mcu_i2c_scl(1);
    mcu_i2c_delay();
    
    while(1)
    {
        time_out++;
        if(time_out > MCU_I2C_TIMEOUT)
        {
            mcu_i2c_stop();
            wjq_log(LOG_INFO, "i2c:wait ack time out!\r\n");
            return 1;
        }

        if(0 == mcu_i2c_readsda())
        {   
            break;
        }
    }
    
    mcu_i2c_scl(0);
    
    return 0;
}
/**
 *@brief:      mcu_i2c_ack
 *@details:       ����ACK�ź�
 *@param[in]  void  
 *@param[out]  ��
 *@retval:     static
 */
static void mcu_i2c_ack(void)
{
    mcu_i2c_scl(0);
    mcu_i2c_sda_output();
    
    mcu_i2c_sda(0);
    mcu_i2c_delay();
    
    mcu_i2c_scl(1);
    mcu_i2c_delay();
    
    mcu_i2c_scl(0);
}
/**
 *@brief:      mcu_i2c_writebyte
 *@details:       I2C����дһ���ֽ�����
 *@param[in]  u8 data  
 *@param[out]  ��
 *@retval:     static
 */
static s32 mcu_i2c_writebyte(u8 data)
{
    u8 i = 0;

    mcu_i2c_sda_output();

    mcu_i2c_scl(0);
    
    while(i<8)
    {
    
        if((0x80 & data) == 0x80)
        {
            mcu_i2c_sda(1);   
        }
        else
        {
            mcu_i2c_sda(0);
        }
        
        mcu_i2c_delay();

        mcu_i2c_scl(1);
        mcu_i2c_delay();
        
        mcu_i2c_scl(0);
        mcu_i2c_delay();
        
        data = data <<1;
        i++;
    }
		return 0;
}
/**
 *@brief:      mcu_i2c_readbyte
 *@details:       I2C���߶�һ���ֽ�����
 *@param[in]  void  
 *@param[out]  ��
 *@retval:     static
 */
static u8 mcu_i2c_readbyte(void)
{
    u8 i = 0;
    u8 data = 0;

    mcu_i2c_sda_input();
    
    while(i<8)
    {
        mcu_i2c_scl(0);
        mcu_i2c_delay();
        
        mcu_i2c_scl(1);

        data = (data <<1);

        if(1 == mcu_i2c_readsda())
        {
            data =data|0x01;    
        }
        else
        {
            data = data & 0xfe;
        }

        mcu_i2c_delay();
        
        i++;
    }

    return data;
}

/**
 *@brief:      mcu_i2c_transfer
 *@details:    �м������¿�ʼλ�Ĵ�������
 *@param[in]   u8 addr   
               u8 rw    0 д��1 ��    
               u8* data  
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_i2c_transfer(u8 addr, u8 rw, u8* data, s32 datalen)
{
    s32 i;
    u8 ch;

    #if 0//����IO���Ƿ���ͨ
    while(1)
    {
        uart_printf("test \r\n");
        mcu_i2c_scl(1);
        mcu_i2c_sda(1); 
        Delay(5);
        mcu_i2c_scl(0);
        mcu_i2c_sda(0); 
        Delay(5);
    }   
    #endif
    
    //������ʼ
    mcu_i2c_start();
    //���͵�ַ+��д��־
    //����ADDR
    if(rw == MCU_I2C_MODE_W)
    {
        addr = ((addr<<1)&0xfe);
        //uart_printf("write\r\n");
    }
    else
    {
        addr = ((addr<<1)|0x01);
        //uart_printf("read\r\n");
    }
    
    mcu_i2c_writebyte(addr);
    mcu_i2c_wait_ack();

    i = 0;
    while(i < datalen)
    {
        //���ݴ���
        if(rw == MCU_I2C_MODE_W)//д
        {
            ch = *(data+i);
            mcu_i2c_writebyte(ch);
            mcu_i2c_wait_ack();
            
        }
        else if(rw == MCU_I2C_MODE_R)//��
        {
            ch = mcu_i2c_readbyte();  
            mcu_i2c_ack();
            *(data+i) = ch;
        }
        i++;
    }

    //���ͽ���
    mcu_i2c_stop();
    return 0;
}

/**
 *@brief:      mcu_i2c_init
 *@details:    ��ʼ��I2C�ӿ�
 *@param[in]   void  
 *@param[out]  ��
 *@retval:     
 */
s32 mcu_i2c_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = MCU_I2C_SCL | MCU_I2C_SDA;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(MCU_I2C_PORT, &GPIO_InitStructure);//��ʼ��

    //��ʼ��IO��״̬
    mcu_i2c_scl(1);
    mcu_i2c_sda(1);
	
	return 0;
}


/*
	����ͷʹ��SCCB�ӿڣ���ʵ����I2C
	��ST�ٷ�����ͷ��ֲ��ʹ�õ���Ӳ��I2C
*/
#define CAREMA_SCCB_I2C 	I2C2 //ʹ��I2C2������
#define SCCB_TIMEOUT_MAX    10000
	
void SCCB_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef  I2C_InitStruct;	  
	/****** Configures the I2C1 used for OV9655 camera module configuration *****/
	/* I2C1 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	
	/* GPIOB clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 
	
	/* Connect I2C2 pins to AF4 */
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_I2C2);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_I2C2);
	
	/* Configure I2C2 GPIOs */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	/* Configure I2C2 */
	/* I2C DeInit */
	I2C_DeInit(CAREMA_SCCB_I2C);
	  
	///* Enable the I2C peripheral */
	//I2C_Cmd(CAREMA_SCCB_I2C, ENABLE);
	
	/* Set the I2C structure parameters */
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0xFE;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_ClockSpeed = 10000;
	
	/* Initialize the I2C peripheral w/ selected parameters */
	I2C_Init(CAREMA_SCCB_I2C, &I2C_InitStruct);
	
	I2C_Cmd(CAREMA_SCCB_I2C, ENABLE);
	
	I2C_AcknowledgeConfig(CAREMA_SCCB_I2C, ENABLE);

}
	
uint8_t bus_sccb_writereg(uint8_t DeviceAddr, uint16_t Addr, uint8_t Data)
{
	
	uint32_t timeout = SCCB_TIMEOUT_MAX;
	
	/* Generate the Start Condition */
	I2C_GenerateSTART(CAREMA_SCCB_I2C, ENABLE);
	
	/* Test on I2C1 EV5 and clear it */
	timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
	  /* If the timeout delay is exceeded, exit with error code */
	  if ((timeout--) == 0) return 0xFF;
	}
	 
	/* Send DCMI selected device slave Address for write */
	I2C_Send7bitAddress(CAREMA_SCCB_I2C, DeviceAddr, I2C_Direction_Transmitter);
	
	/* Test on I2C1 EV6 and clear it */
	timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
	  /* If the timeout delay is exceeded, exit with error code */
	  if ((timeout--) == 0) return 0xFF;
	}
	
	/* Send I2C1 location address LSB */
	I2C_SendData(CAREMA_SCCB_I2C, (uint8_t)(Addr));
	
	/* Test on I2C1 EV8 and clear it */
	timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
	  /* If the timeout delay is exceeded, exit with error code */
	  if ((timeout--) == 0) return 0xFF;
	}
	
	/* Send Data */
	I2C_SendData(CAREMA_SCCB_I2C, Data);
	
	/* Test on I2C1 EV8 and clear it */
	timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
	  /* If the timeout delay is exceeded, exit with error code */
	  if ((timeout--) == 0) return 0xFF;
	}  
	
	/* Send I2C1 STOP Condition */
	I2C_GenerateSTOP(CAREMA_SCCB_I2C, ENABLE);
	
	/* If operation is OK, return 0 */
	return 0;

}
	
uint8_t bus_sccb_readreg(uint8_t DeviceAddr, uint16_t Addr)
{
  uint32_t timeout = SCCB_TIMEOUT_MAX;
  uint8_t Data = 0;

  /* Generate the Start Condition */
  I2C_GenerateSTART(CAREMA_SCCB_I2C, ENABLE);

  /* Test on I2C1 EV5 and clear it */
  timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
	/* If the timeout delay is exceeded, exit with error code */
	if ((timeout--) == 0) return 0xFF;
  }
  
  /* Send DCMI selected device slave Address for write */
  I2C_Send7bitAddress(CAREMA_SCCB_I2C, DeviceAddr, I2C_Direction_Transmitter);
 
  /* Test on I2C1 EV6 and clear it */
  timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
	/* If the timeout delay is exceeded, exit with error code */
	if ((timeout--) == 0) return 0xFF;
  }

  /* Send I2C1 location address LSB */
  I2C_SendData(CAREMA_SCCB_I2C, (uint8_t)(Addr));

  /* Test on I2C1 EV8 and clear it */
  timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
	/* If the timeout delay is exceeded, exit with error code */
	if ((timeout--) == 0) return 0xFF;
  } 
  
  /* Clear AF flag if arised */
  CAREMA_SCCB_I2C->SR1 |= (uint16_t)0x0400;

  /* Generate the Start Condition */
  I2C_GenerateSTART(CAREMA_SCCB_I2C, ENABLE);
  
  /* Test on I2C1 EV6 and clear it */
  timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
	/* If the timeout delay is exceeded, exit with error code */
	if ((timeout--) == 0) return 0xFF;
  } 
  
  /* Send DCMI selected device slave Address for write */
  I2C_Send7bitAddress(CAREMA_SCCB_I2C, DeviceAddr, I2C_Direction_Receiver);

  /* Test on I2C1 EV6 and clear it */
  timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {
	/* If the timeout delay is exceeded, exit with error code */
	if ((timeout--) == 0) return 0xFF;
  }
 
  /* Prepare an NACK for the next data received */
  I2C_AcknowledgeConfig(CAREMA_SCCB_I2C, DISABLE);

  /* Test on I2C1 EV7 and clear it */
  timeout = SCCB_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(CAREMA_SCCB_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
  {
	/* If the timeout delay is exceeded, exit with error code */
	if ((timeout--) == 0) return 0xFF;
  }

  /* Prepare Stop after receiving data */
  I2C_GenerateSTOP(CAREMA_SCCB_I2C, ENABLE);

  /* Receive the Data */
  Data = I2C_ReceiveData(CAREMA_SCCB_I2C);

  /* return the read data */
  return Data;
}
	
