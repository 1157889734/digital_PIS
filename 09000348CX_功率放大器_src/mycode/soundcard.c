/***********************************************************************************
 * 版权说明   : Copyright (c) 2017-2027   天津市北海通信 技术有限公司
 * 文 件 名   : soundcard.c
 * 作者姓名   : 陈浩             工号:1976
 * 版本号　   : v1.0
 * 创建日期   : 2017年10月12日
 * 文件描述   : 声卡模块
 * 功能说明   :
 * 关联文件   :
 * 其　他　   :
 * 修改日志   :
***********************************************************************************/

#include "stm32f2xx.h"
#include "myI2s.h"
#include "soundcard.h"

//下面两个变量测试及调试时使用
int I2C_SGTL5000_Error;
int I2C_SGTL5000_Timeout;
u8 g_SGTL5000_valid = 0;


static u16 SGTL5000_i2c_write(u16 addr, u16 value);

/***********************声卡型号SGTL5000***********************************/


/*****************************************************************************
 * 函 数 名　: SGTL5000_i2c_init
 * 负 责 人　: 陈浩             工号:1976
 * 创建日期  : 2017年10月12日
 * 函数功能  : 声卡I2C引脚初始化
 * 输入参数  : void  无
 * 输出参数  : 无
 * 返 回 值　: static
 * 调用关系  :
 * 其　它　  :

*****************************************************************************/
static void SGTL5000_i2c_init(void)
{
    I2C_InitTypeDef  I2C_InitStructure;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = I2C_SGTL5000_ADDR;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C_SGTL5000_SPEED;
    /* I2C2 Peripheral Enable */
    I2C_Cmd(I2C_SGTL5000, ENABLE);
    /* Apply I2C2 configuration after enabling it */
    I2C_Init(I2C_SGTL5000, &I2C_InitStructure);
}

/*****************************************************************************
 * 函 数 名　: SGTL5000_Init
 * 负 责 人　: 陈浩             工号:1976
 * 创建日期  : 2017年10月12日
 * 函数功能  : SGTL5000声卡初始化
 * 输入参数  : void  无
 * 输出参数  : 无
 * 返 回 值　: static
 * 调用关系  :
 * 其　它　  :

*****************************************************************************/
static void SGTL5000_Init(void)
{
    SGTL5000_i2c_write(SGTL5000_CHIP_LINREG_CTRL, 0x006c); // VDDIO is set to the source of charge pump    VDDD voltage  1.2V
    //  SGTL5000_i2c_write(SGTL5000_CHIP_REF_CTRL,0x01e1); //Analog Ground Voltage (VAG_VAL)1.55V,   Slow down VAG ramp
    SGTL5000_i2c_write(SGTL5000_CHIP_REF_CTRL, 0x01e4); //Analog Ground Voltage (VAG_VAL)1.55V,   The bias current should be set to 50% of the nominal value
    SGTL5000_i2c_write(SGTL5000_CHIP_LINE_OUT_CTRL, 0x0f1e); //LINEOUT current 0.54mA        LINEOUT AGND(LO_VAGCNTRL)  Voltage 1.55V
    SGTL5000_i2c_write(SGTL5000_CHIP_REF_CTRL, 0x01e5); //Analog Ground Voltage (VAG_VAL)1.55V,   The bias current should be set to 50% of the nominal value,  SMALL_POP
    SGTL5000_i2c_write(SGTL5000_CHIP_SHORT_CTRL, 0x1106); // Enable Zero-cross detect if needed for HP_OUT (bit 5) and ADC (bit 1)
    SGTL5000_i2c_write(SGTL5000_CHIP_ANA_CTRL, 0x0016); //LINEOUT Unmute, headphone outputs mute,  ADC  ZCD  enable,  ADC analog volume  unmute
    //  SGTL5000_i2c_write(SGTL5000_CHIP_ANA_POWER,0x47ff);
    SGTL5000_i2c_write(SGTL5000_CHIP_ANA_POWER, 0x6Aff);
    SGTL5000_i2c_write(SGTL5000_CHIP_DIG_POWER, 0X0073); //Enable ADC BLOCK  DAC BLOCK  DAP BLOCK  IIS DATA IN/OUTPUT
    SGTL5000_i2c_write(SGTL5000_CHIP_LINE_OUT_VOL, 0x0f0f); //LO_VOL_RIGHT(LEFT)   0X0F
    SGTL5000_i2c_write(SGTL5000_CHIP_CLK_CTRL, 0X0004); //44.1K   256*FS
    SGTL5000_i2c_write(SGTL5000_CHIP_I2S_CTRL, 0X0130); //I2S  slave ，32Fs,  16bits，I2S mode，0-LEFT   1-RIGHT
    //  SGTL5000_i2c_write(SGTL5000_CHIP_SSS_CTRL,0X0070);//I2S_IN->DAP->DAC     ADC->I2S_OUT
    //  SGTL5000_i2c_write(SGTL5000_CHIP_SSS_CTRL,0X0000);//I2S_IN->DAC     ADC->DAC
    SGTL5000_i2c_write(SGTL5000_CHIP_SSS_CTRL, 0X0050); //I2S_IN->DAC     ADC->I2S_OUT
    SGTL5000_i2c_write(SGTL5000_CHIP_ADCDAC_CTRL, 0x0000); //Volume Ramp Disable  ADC_HPF_FREEZE/BYPASS Disable
    //  SGTL5000_i2c_write(SGTL5000_CHIP_ADCDAC_CTRL,0x0300);//Volume Rampe  Enable    ADC_HPF_FREEZE/BYPASS Disable
    SGTL5000_i2c_write(SGTL5000_DAP_CTRL, 0x0000);  //DAP Disable     DAP mixer  Disable
}


/*****************************************************************************
 * 函 数 名　: SGTL5000_set_output_vol
 * 负 责 人　: 陈浩             工号:1976
 * 创建日期  : 2017年10月12日
 * 函数功能  : 设置SGTL5000声卡输出音量
 * 输入参数  : u8 val  输出音量值
 * 输出参数  : 无
 * 返 回 值　: static
 * 调用关系  :
 * 其　它　  : 范围0-40  其中0为最大 40 为最小

*****************************************************************************/
static void SGTL5000_set_output_vol(u8 val)
{
    u16 DAC_Vol;
    
    if (val >= 40)
    {
        val = 0xC0;//0xFC-0x3C
    }
    else
    {
        val = val * 2;
    }
    
    //只用了左声道
    DAC_Vol = (0xFC << 8) + (u16)(0x3C + val);
    SGTL5000_i2c_write(SGTL5000_CHIP_DAC_VOL, DAC_Vol);
}


/*****************************************************************************
* 函 数 名　: SGTL5000_set_input_vol
* 负 责 人　: 陈浩             工号:1976
* 创建日期  : 2017年10月12日
* 函数功能  : 设置SGTL5000声卡输入音量
* 输入参数  : u8 val  输入音量值
* 输出参数  : 无
* 返 回 值　: static
* 调用关系  :
* 其　它　  : 范围为0-15  其中0为最大，15为最小

*****************************************************************************/
static void SGTL5000_set_input_vol(u8 val)
{
    u16 ADC_Vol;
    
    if (val >= 15)
    {
        val = 15;
    }
    
    val = 15 - val;
    ADC_Vol = 0x01F0 + (u16)val;
    SGTL5000_i2c_write(SGTL5000_CHIP_ANA_ADC_CTRL, ADC_Vol);
}

/*****************************************************************************
 * 函 数 名　: SGTL5000_i2c_write
 * 负 责 人　: 陈浩             工号:1976
 * 创建日期  : 2017年10月12日
 * 函数功能  : 写SGTL5000声卡I2C
 * 输入参数  : u16 addr   地址
   　　　　    u16 value  数值
 * 输出参数  : 无
 * 返 回 值　: static
 * 调用关系  :
 * 其　它　  :

*****************************************************************************/
static u16 SGTL5000_i2c_write(u16 addr, u16 value)
{
    u8 addr_l;
    u8 addr_h;
    u8 value_l;
    u8 value_h;
    addr_h = (u8)((addr >> 8) & 0x00ff);
    addr_l = (u8)((addr) & 0x00ff);
    value_h = (u8)((value >> 8) & 0x00ff);
    value_l = (u8)((value) & 0x00ff);
    
    while (I2C_GetFlagStatus(I2C_SGTL5000, I2C_FLAG_BUSY))
    {
        I2C_SGTL5000_Timeout++;
        
        if (I2C_SGTL5000_Timeout > I2C_TIMEOUT_MAX)
        {
            I2C_SGTL5000_Error = 1;
            return (u8)(-1);
        }
    }
    
    /*!< Send START condition */
    I2C_GenerateSTART(I2C_SGTL5000, ENABLE);
    
    /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
    while (!I2C_CheckEvent(I2C_SGTL5000, I2C_EVENT_MASTER_MODE_SELECT))
    {
        I2C_SGTL5000_Timeout++;
        
        if (I2C_SGTL5000_Timeout > I2C_TIMEOUT_MAX)
        {
            I2C_SGTL5000_Error = 2;
            return (u8)(-1);
        }
    }
    
    /*!< Send SGTL5000 address for write */
    I2C_Send7bitAddress(I2C_SGTL5000, I2C_SGTL5000_ADDR, I2C_Direction_Transmitter);
    
    /*!< Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        I2C_SGTL5000_Timeout++;
        
        if (I2C_SGTL5000_Timeout > I2C_TIMEOUT_MAX)
        {
            I2C_SGTL5000_Error = 3;
            return (u8)(-1);
        }
    }
    
    /*!< Send the SGTL5000's internal address to read from: Only one byte address */
    I2C_SendData(I2C_SGTL5000, addr_h);
    
    /*!< Test on EV8 and clear it */
    while (I2C_GetFlagStatus(I2C_SGTL5000, I2C_FLAG_BTF) == RESET)
    {
        I2C_SGTL5000_Timeout++;
        
        if (I2C_SGTL5000_Timeout > I2C_TIMEOUT_MAX)
        {
            I2C_SGTL5000_Error = 4;
            return (u8)(-1);
        }
    }
    
    /*!< Send the SGTL5000's internal address to read from: Only one byte address */
    I2C_SendData(I2C_SGTL5000, addr_l);
    
    /*!< Test on EV8 and clear it */
    while (I2C_GetFlagStatus(I2C_SGTL5000, I2C_FLAG_BTF) == RESET)
    {
        I2C_SGTL5000_Timeout++;
        
        if (I2C_SGTL5000_Timeout > I2C_TIMEOUT_MAX)
        {
            I2C_SGTL5000_Error = 5;
            return (u8)(-1);
        }
    }
    
    I2C_SendData(I2C_SGTL5000, value_h);
    
    /*!< Test on EV8 and clear it */
    while (I2C_GetFlagStatus(I2C_SGTL5000, I2C_FLAG_BTF) == RESET)
    {
        I2C_SGTL5000_Timeout++;
        
        if (I2C_SGTL5000_Timeout > I2C_TIMEOUT_MAX)
        {
            I2C_SGTL5000_Error = 6;
            return (u8)(-1);
        }
    }
    
    /*!< Send the SGTL5000's internal address to read from: Only one byte address */
    I2C_SendData(I2C_SGTL5000, value_l);
    
    /*!< Test on EV8 and clear it */
    while (I2C_GetFlagStatus(I2C_SGTL5000, I2C_FLAG_BTF) == RESET)
    {
        I2C_SGTL5000_Timeout++;
        
        if (I2C_SGTL5000_Timeout > I2C_TIMEOUT_MAX)
        {
            I2C_SGTL5000_Error = 7;
            return (u8)(-1);
        }
    }
    
    /*!< Test on EV8 and clear it */
    while (I2C_GetFlagStatus(I2C_SGTL5000, I2C_FLAG_BTF) == RESET)
    {
        I2C_SGTL5000_Timeout++;
        
        if (I2C_SGTL5000_Timeout > I2C_TIMEOUT_MAX)
        {
            I2C_SGTL5000_Error = 8;
            return (u8)(-1);
        }
    }
    
    while (!I2C_CheckEvent(I2C_SGTL5000, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    {
        I2C_SGTL5000_Timeout++;
        
        if (I2C_SGTL5000_Timeout > I2C_TIMEOUT_MAX)
        {
            I2C_SGTL5000_Error = 9;
            return (u8)(-1);
        }
    }
    
    /*!< Send STOP Condition */
    I2C_GenerateSTOP(I2C_SGTL5000, ENABLE);
    return 0;
}


/*****************************************************************************
 * 函 数 名　: SGTL5000_i2c_read
 * 负 责 人　: 陈浩             工号:1976
 * 创建日期  : 2017年10月12日
 * 函数功能  : 读SGTL5000声卡I2C
 * 输入参数  : u16 addr  地址
 * 输出参数  : 无
 * 返 回 值　: static
 * 调用关系  :
 * 其　它　  :

*****************************************************************************/
static u16 SGTL5000_i2c_read(u16 addr)
{
    u8  out_byte_l;
    u8  out_byte_h;
    u16 outdata;
    u8  addr_l;
    u8  addr_h;
    addr_h = (u8)((addr >> 8) & 0x00ff);
    addr_l = (u8)((addr) & 0x00ff);
    I2C_SGTL5000_Timeout = 0;
    I2C_SGTL5000_Error = 0;
    
    while (I2C_GetFlagStatus(I2C_SGTL5000, I2C_FLAG_BUSY))
    {
        I2C_SGTL5000_Timeout++;
        
        if (I2C_SGTL5000_Timeout > I2C_TIMEOUT_MAX)
        {
            I2C_SGTL5000_Error = 10;
            return (u8)(-1);
        }
    }
    
    /*!< Send START condition */
    I2C_GenerateSTART(I2C_SGTL5000, ENABLE);
    /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
    I2C_SGTL5000_Timeout = 0;
    I2C_SGTL5000_Error = 0;
    
    while (!I2C_CheckEvent(I2C_SGTL5000, I2C_EVENT_MASTER_MODE_SELECT))
    {
        I2C_SGTL5000_Timeout++;
        
        if (I2C_SGTL5000_Timeout > I2C_TIMEOUT_MAX)
        {
            I2C_SGTL5000_Error = 11;
            return (u8)(-1);
        }
    }
    
    /*!< Send SGTL5000 address for write */
    I2C_Send7bitAddress(I2C_SGTL5000, I2C_SGTL5000_ADDR, I2C_Direction_Transmitter);
    /*!< Test on EV6 and clear it */
    I2C_SGTL5000_Timeout = 0;
    I2C_SGTL5000_Error = 0;
    
    while (!I2C_CheckEvent(I2C_SGTL5000, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        I2C_SGTL5000_Timeout++;
        
        if (I2C_SGTL5000_Timeout > I2C_TIMEOUT_MAX)
        {
            I2C_SGTL5000_Error = 12;
            return (u8)(-1);
        }
    }
    
    /*!< Send the SGTL5000's internal address to read from: Only one byte address */
    I2C_SendData(I2C_SGTL5000, addr_h);
    /*!< Test on EV8 and clear it */
    I2C_SGTL5000_Timeout = 0;
    I2C_SGTL5000_Error = 0;
    
    while (I2C_CheckEvent(I2C_SGTL5000, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == RESET)
    {
        I2C_SGTL5000_Timeout++;
        
        if (I2C_SGTL5000_Timeout > I2C_TIMEOUT_MAX)
        {
            I2C_SGTL5000_Error = 13;
            return (u8)(-1);
        }
    }
    
    /*!< Send the SGTL5000's internal address to read from: Only one byte address */
    I2C_SendData(I2C_SGTL5000, addr_l);
    /*!< Test on EV8 and clear it */
    I2C_SGTL5000_Timeout = 0;
    I2C_SGTL5000_Error = 0;
    
    while (I2C_GetFlagStatus(I2C_SGTL5000, I2C_FLAG_BTF) == RESET)
    {
        I2C_SGTL5000_Timeout++;
        
        if (I2C_SGTL5000_Timeout > I2C_TIMEOUT_MAX)
        {
            I2C_SGTL5000_Error = 14;
            return (u8)(-1);
        }
    }
    
    /*!< Send STRAT condition a second time */
    I2C_GenerateSTART(I2C_SGTL5000, ENABLE);
    /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
    I2C_SGTL5000_Timeout = 0;
    I2C_SGTL5000_Error = 0;
    
    while (!I2C_CheckEvent(I2C_SGTL5000, I2C_EVENT_MASTER_MODE_SELECT))
    {
        I2C_SGTL5000_Timeout++;
        
        if (I2C_SGTL5000_Timeout > I2C_TIMEOUT_MAX)
        {
            I2C_SGTL5000_Error = 15;
            return (u8)(-1);
        }
    }
    
    /*!< Send SGTL5000 address for read */
    I2C_Send7bitAddress(I2C_SGTL5000, I2C_SGTL5000_ADDR, I2C_Direction_Receiver);
    /* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
    I2C_SGTL5000_Timeout = 0;
    I2C_SGTL5000_Error = 0;
    
    while (I2C_GetFlagStatus(I2C_SGTL5000, I2C_FLAG_ADDR) == RESET)
    {
        I2C_SGTL5000_Timeout++;
        
        if (I2C_SGTL5000_Timeout > I2C_TIMEOUT_MAX)
        {
            I2C_SGTL5000_Error = 16;
            return (u8)(-1);
        }
    }
    
    /*!< Disable Acknowledgment */
    I2C_AcknowledgeConfig(I2C_SGTL5000, ENABLE);
    /* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
    (void)I2C_SGTL5000->SR2;
    /* Wait for the byte to be received */
    I2C_SGTL5000_Timeout = 0;
    I2C_SGTL5000_Error = 0;
    
    while (I2C_GetFlagStatus(I2C_SGTL5000, I2C_FLAG_RXNE) == RESET)
    {
        I2C_SGTL5000_Timeout++;
        
        if (I2C_SGTL5000_Timeout > I2C_TIMEOUT_MAX)
        {
            I2C_SGTL5000_Error = 17;
            return (u8)(-1);
        }
    }
    
    /*!< Read the byte received from the SGTL5000 */
    out_byte_h = I2C_ReceiveData(I2C_SGTL5000);
    I2C_AcknowledgeConfig(I2C_SGTL5000, DISABLE);
    /* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
    (void)I2C_SGTL5000->SR2;
    /* Wait for the byte to be received */
    I2C_SGTL5000_Timeout = 0;
    I2C_SGTL5000_Error = 0;
    
    while (I2C_GetFlagStatus(I2C_SGTL5000, I2C_FLAG_RXNE) == RESET)
    {
        I2C_SGTL5000_Timeout++;
        
        if (I2C_SGTL5000_Timeout > I2C_TIMEOUT_MAX)
        {
            I2C_SGTL5000_Error = 18;
            return (u8)(-1);
        }
    }
    
    /*!< Read the byte received from the SGTL5000 */
    out_byte_l = I2C_ReceiveData(I2C_SGTL5000);
    /*!< Send STOP Condition */
    I2C_GenerateSTOP(I2C_SGTL5000, ENABLE);
    outdata = (u16)(out_byte_h << 8 | out_byte_l);
    return outdata;
}
/***********************声卡型号SGTL5000***********************************/
/**************************************************************************/




/*****************************UDA1341*************************************/

/*****************************************************************************
 * 函 数 名　: L3_delay
 * 负 责 人　: 陈浩             工号:1976
 * 创建日期  : 2017年10月12日
 * 函数功能  : L3延迟
 * 输入参数  : void  无
 * 输出参数  : 无
 * 返 回 值　:
 * 调用关系  :
 * 其　它　  :

*****************************************************************************/
void L3_delay(void)
{
    int i;

    for (i = 0; i < 5; i++);
}

/*****************************************************************************
 * 函 数 名　: L3_send_byte
 * 负 责 人　: 陈浩             工号:1976
 * 创建日期  : 2017年10月12日
 * 函数功能  : L3发送数据
 * 输入参数  : u8 indata   数据
   　　　　    int l3mode  模式
 * 输出参数  : 无
 * 返 回 值　:
 * 调用关系  :
 * 其　它　  :

*****************************************************************************/
void L3_send_byte(u8 indata, int l3mode)
{
    u8 i;
    write_L3_mode(l3mode);
    write_L3_clock(1);
    write_L3_data(0);
    L3_delay();
    
    for (i = 0; i < 8; i++)
    {
        if ( (u8)((indata >> i) & 0x01) == 0x00 )
        {
            write_L3_data(0);
            write_L3_data(0);
        }
        else
        {
            write_L3_data(0);
            write_L3_data(1);
        }
        
        write_L3_clock(0);
        L3_delay();
        write_L3_clock(1);
        L3_delay();
    }
    
    L3_delay();
    write_L3_mode(1);
    L3_delay();
}

/*****************************************************************************
 * 函 数 名　: L3_uda1341_set_input_gain
 * 负 责 人　: 陈浩             工号:1976
 * 创建日期  : 2017年10月12日
 * 函数功能  : 设置uda1341声卡输入音量
 * 输入参数  : u8 dbvalue  输入音量
 * 输出参数  : 无
 * 返 回 值　: 0
 * 调用关系  :
 * 其　它　  :

*****************************************************************************/
int L3_uda1341_set_input_gain(u8 dbvalue)
{
    L3_send_byte((0x14 | 0x00), L3MODE_ADDR);     //selecte Data0
    
    if (dbvalue > 45)
    {
        dbvalue = 45;
    }
    
    dbvalue = (dbvalue * 2) / 3;
    L3_send_byte(0xc0, L3MODE_DATA);
    L3_send_byte((0xe0 | dbvalue), L3MODE_DATA);  //set channel 1 gain
    return (0);
}

/*****************************************************************************
 * 函 数 名　: L3_uda1341_set_volume
 * 负 责 人　: 陈浩             工号:1976
 * 创建日期  : 2017年10月12日
 * 函数功能  : 设置uda1341声卡输出音量
 * 输入参数  : u8 dbvalue  输出音量
 * 输出参数  : 无
 * 返 回 值　: 0:设置成功 -1:设置失败
 * 调用关系  :
 * 其　它　  :

*****************************************************************************/
int L3_uda1341_set_volume(u8 dbvalue)
{
    if (dbvalue == 0)
    {
        L3_send_byte((0x14 | 0x00), L3MODE_ADDR);
        L3_send_byte(0x00, L3MODE_DATA);
        return (0);
    }
    else if (dbvalue < 61)
    {
        L3_send_byte((0x14 | 0x00), L3MODE_ADDR);
        L3_send_byte(dbvalue + 1, L3MODE_DATA);
        return (0);
    }
    else if (dbvalue <= 100)
    {
        L3_send_byte((0x14 | 0x00), L3MODE_ADDR);
        L3_send_byte(0x6f, L3MODE_DATA);
        return (0);
    }
    else
    {
        return (-1);
    }
}

/*****************************************************************************
 * 函 数 名　: L3_write_uda1341_status
 * 负 责 人　: 陈浩             工号:1976
 * 创建日期  : 2017年10月12日
 * 函数功能  : 写uda1341声卡状态
 * 输入参数  : void  无
 * 输出参数  : 无
 * 返 回 值　:
 * 调用关系  :
 * 其　它　  :

*****************************************************************************/
void L3_write_uda1341_status(void)
{
    L3_send_byte((0x14 | 0x02), L3MODE_ADDR);     //selecte status
    L3_send_byte(0x60, L3MODE_DATA);  //reset 256fs
    L3_send_byte(0x20, L3MODE_DATA);  //data format 256fs
    L3_send_byte(0x83, L3MODE_DATA);      //input gain 0dB
}

/*****************************************************************************
 * 函 数 名　: L3_write_uda1341_data
 * 负 责 人　: 陈浩             工号:1976
 * 创建日期  : 2017年10月12日
 * 函数功能  : 写uda1341声卡数据
 * 输入参数  : void  无
 * 输出参数  : 无
 * 返 回 值　:
 * 调用关系  :
 * 其　它　  :

*****************************************************************************/
void L3_write_uda1341_data(void)
{
    L3_send_byte((0x14 | 0x00), L3MODE_ADDR);     //selecte Data0
    L3_send_byte(0x0f, L3MODE_DATA);  //volume
    L3_send_byte(0x81, L3MODE_DATA);
    L3_send_byte(0xc2, L3MODE_DATA);
    L3_send_byte(0xe3, L3MODE_DATA);  //input mixer
    L3_send_byte(0xc1, L3MODE_DATA);
    L3_send_byte(0xff, L3MODE_DATA);  //turn input channel 2 to off
    L3_send_byte(0xc0, L3MODE_DATA);
    L3_send_byte(0xf0, L3MODE_DATA);  //set input channel 1 gain
}

/*****************************************************************************
 * 函 数 名　: L3_init_uda1341
 * 负 责 人　: 陈浩             工号:1976
 * 创建日期  : 2017年10月12日
 * 函数功能  : uda1341声卡初始化
 * 输入参数  : void  无
 * 输出参数  : 无
 * 返 回 值　:
 * 调用关系  :
 * 其　它　  :

*****************************************************************************/
void L3_init_uda1341(void)
{
    L3_write_uda1341_status();
    L3_write_uda1341_data();
}


/*****************************************************************************
 * 函 数 名　: soundcard_init
 * 负 责 人　: 陈浩             工号:1976
 * 创建日期  : 2017年10月12日
 * 函数功能  : 初始化声卡
 * 输入参数  : void  无
 * 输出参数  : 无
 * 返 回 值　:
 * 调用关系  :
 * 其　它　  :

*****************************************************************************/
void soundcard_init(void)
{
    u16 temp = 0;
    SGTL5000_i2c_init();
    Delay(2000);
    temp = SGTL5000_i2c_read(SGTL5000_CHIP_ID);
    
    if ((temp & 0xff00) == 0xa000) //声卡SGTL5000的CHIP_ID
    {
        SGTL5000_Init();
        g_SGTL5000_valid = 1;
    }
    else
    {
        L3_init_uda1341();
    }
}

/*****************************************************************************
 * 函 数 名　: soundcard_set_input_vol
 * 负 责 人　: 陈浩             工号:1976
 * 创建日期  : 2017年10月12日
 * 函数功能  : 设置声卡输入音量
 * 输入参数  : u8 vol  输入音量值
 * 输出参数  : 无
 * 返 回 值　:
 * 调用关系  :
 * 其　它　  :

*****************************************************************************/
void soundcard_set_input_vol(u8 vol)
{
    if (g_SGTL5000_valid)
    {
        SGTL5000_set_input_vol(vol);
    }
    else
    {
        L3_uda1341_set_input_gain(vol);
    }
}

/*****************************************************************************
 * 函 数 名　: soundcard_set_output_vol
 * 负 责 人　: 陈浩             工号:1976
 * 创建日期  : 2017年10月12日
 * 函数功能  : 设置声卡输出音量
 * 输入参数  : u8 vol  输出音量值
 * 输出参数  : 无
 * 返 回 值　:
 * 调用关系  :
 * 其　它　  :

*****************************************************************************/
void soundcard_set_output_vol(u8 vol)
{
    if (g_SGTL5000_valid)
    {
        SGTL5000_set_output_vol(vol);
    }
    else
    {
        L3_uda1341_set_volume(vol);
    }
}

/*****************************************************************************
 * 函 数 名　: write_L3_data
 * 负 责 人　: 陈浩             工号:1976
 * 创建日期  : 2017年10月12日
 * 函数功能  : L3写数据
 * 输入参数  : int bitvalue  数据
 * 输出参数  : 无
 * 返 回 值　:
 * 调用关系  :
 * 其　它　  :

*****************************************************************************/
void write_L3_data(int bitvalue)
{
    if (bitvalue > 0)
    {
        GPIO_SetBits(PORT_L3DATA, PIN_L3DATA);
    }
    else
    {
        GPIO_ResetBits(PORT_L3DATA, PIN_L3DATA);
    }
}

/*****************************************************************************
 * 函 数 名　: write_L3_clock
 * 负 责 人　: 陈浩             工号:1976
 * 创建日期  : 2017年10月12日
 * 函数功能  : L3发时钟
 * 输入参数  : int bitvalue  时钟数据
 * 输出参数  : 无
 * 返 回 值　:
 * 调用关系  :
 * 其　它　  :

*****************************************************************************/
void write_L3_clock(int bitvalue)
{
    if (bitvalue > 0)
    {
        GPIO_SetBits(PORT_L3CLK, PIN_L3CLK);
    }
    else
    {
        GPIO_ResetBits(PORT_L3CLK, PIN_L3CLK);
    }
}

/*****************************************************************************
 * 函 数 名　: write_L3_mode
 * 负 责 人　: 陈浩             工号:1976
 * 创建日期  : 2017年10月12日
 * 函数功能  : L3写模式
 * 输入参数  : int bitvalue  模式数据
 * 输出参数  : 无
 * 返 回 值　:
 * 调用关系  :
 * 其　它　  :

*****************************************************************************/
void write_L3_mode(int bitvalue)
{
    if (bitvalue > 0)
    {
        GPIO_SetBits(PORT_L3MODE, PIN_L3MODE);
    }
    else
    {
        GPIO_ResetBits(PORT_L3MODE, PIN_L3MODE);
    }
}
