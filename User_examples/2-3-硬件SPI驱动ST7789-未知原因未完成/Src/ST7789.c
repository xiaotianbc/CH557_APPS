//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：STC51
//1.8寸TFT_ST7735液晶驱动
//********************************************************************************
#include <stdio.h>
#include <string.h>
#include "CH557X.H"
#include "../Inc/ST7789.h"
#define uchar unsigned char
#define uint unsigned int



//---------------------------液晶屏接线说明-------------------------------------//
//接线前请参考液晶屏说明书第10页引脚定义
//#define bl P7_0
//sbit bl = P1 ^ 4;    //接模块BL引脚，背光可以采用IO控制或者PWM控制，也可以直接接到高电平常亮
#define scl P3_7
//sbit scl = P1 ^ 5;   //接模块CLK引脚,接裸屏Pin9_SCL
#define sda P3_5
//sbit sda = P1 ^ 3;   //接模块DIN/MOSI引脚，接裸屏Pin8_SDA
#define rs P4_3
//sbit rs = P3 ^ 4;    //接模块D/C引脚，接裸屏Pin7_A0
#define reset P3_6
//sbit reset = P3 ^ 5; //接模块RST引脚，接裸屏Pin6_RES
//---------------------------End of液晶屏接线---------------------------------//

extern uchar __code Zk_ASCII8X16[];

void delay_ms(uint time)
{
    uint i, j;
    for (i = 0; i < time; i++)
        for (j = 0; j < 250; j++)
            ;
}

__bit busy;

void HWSPI_Init()
{
    P3_MOD_OC &= ~((1 << 7) | (1 << 5)); //SCK1, MOSI1 推挽输出
    P3_DIR_PU |= ((1 << 7) | (1 << 5));
    SPI1_CK_SE = 0x40;           //设置SPI1时钟分频系数： SPI速度 = 主频/SPI1_CK_SE
    SPI1_CTRL &= ~(bS1_CLR_ALL); // 重要！！！！不操作会导致无法工作：该位为1 清空SPI1中断标志和FIFO，需要软件清零，
    SPI1_CTRL |= bS1_SCK_OE;     //SPI1的SCK1 输出使能控制位，该位为1允许SCK1输出，如果bS1_2_WIRE=0，那么将同时允许MOSI1 输出使能；该位为0 禁止输出
}

//向SPI总线传输一个8位数据
void SPI_WriteData(uchar Data)
{
    SPI1_DATA = Data;
    while (!(SPI1_STAT & 0x08))
        ;
}
//向液晶屏写一个8位指令
void Lcd_WriteIndex(uchar Data)
{
    rs = 0;
    SPI_WriteData(Data);
}
//向液晶屏写一个8位数据
void Lcd_WriteData(uchar Data)
{
    rs = 1;
    SPI_WriteData(Data);
}
//向液晶屏写一个16位数据
void LCD_WriteData_16Bit(unsigned int Data)
{
    rs = 1;
    SPI_WriteData(Data >> 8); //写入高8位数据
    SPI_WriteData(Data);      //写入低8位数据
}

void Reset()
{
    reset = 0;
    delay_ms(100);
    reset = 1;
    delay_ms(100);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//液晶屏HWSPI初始化 for S6D02A1
void lcd_initial()
{
    HWSPI_Init();
    Reset(); //Reset before LCD Init.

    //LCD Init For 1.44Inch LCD Panel with ST7735R.
    // Lcd_WriteIndex(0x11); //Sleep exit
    delay_ms(120);

    Lcd_WriteIndex(0x36);
    Lcd_WriteData(0x00);

    Lcd_WriteIndex(0x3A);
    Lcd_WriteData(0x05);

    Lcd_WriteIndex(0xB2);
    Lcd_WriteData(0x0C);
    Lcd_WriteData(0x0C);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x33);
    Lcd_WriteData(0x33);

    Lcd_WriteIndex(0xB7);
    Lcd_WriteData(0x35);

    Lcd_WriteIndex(0xBB);
    Lcd_WriteData(0x19);

    Lcd_WriteIndex(0xC0);
    Lcd_WriteData(0x2C);

    Lcd_WriteIndex(0xC2);
    Lcd_WriteData(0x01);

    Lcd_WriteIndex(0xC3);
    Lcd_WriteData(0x12);

    Lcd_WriteIndex(0xC4);
    Lcd_WriteData(0x20);

    Lcd_WriteIndex(0xC6);
    Lcd_WriteData(0x0F);

    Lcd_WriteIndex(0xD0);
    Lcd_WriteData(0xA4);
    Lcd_WriteData(0xA1);

    Lcd_WriteIndex(0xE0);
    Lcd_WriteData(0xD0);
    Lcd_WriteData(0x04);
    Lcd_WriteData(0x0D);
    Lcd_WriteData(0x11);
    Lcd_WriteData(0x13);
    Lcd_WriteData(0x2B);
    Lcd_WriteData(0x3F);
    Lcd_WriteData(0x54);
    Lcd_WriteData(0x4C);
    Lcd_WriteData(0x18);
    Lcd_WriteData(0x0D);
    Lcd_WriteData(0x0B);
    Lcd_WriteData(0x1F);
    Lcd_WriteData(0x23);

    Lcd_WriteIndex(0xE1);
    Lcd_WriteData(0xD0);
    Lcd_WriteData(0x04);
    Lcd_WriteData(0x0C);
    Lcd_WriteData(0x11);
    Lcd_WriteData(0x13);
    Lcd_WriteData(0x2C);
    Lcd_WriteData(0x3F);
    Lcd_WriteData(0x44);
    Lcd_WriteData(0x51);
    Lcd_WriteData(0x2F);
    Lcd_WriteData(0x1F);
    Lcd_WriteData(0x1F);
    Lcd_WriteData(0x20);
    Lcd_WriteData(0x23);

    Lcd_WriteIndex(0x21);

    Lcd_WriteIndex(0x11);

    Lcd_WriteIndex(0x29);
}

/*************************************************
函数名：LCD_Set_Region
功能：设置lcd显示区域，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
void Lcd_SetRegion(unsigned int x_start, unsigned int y_start, unsigned int x_end, unsigned int y_end)
{

    Lcd_WriteIndex(0x2a);
    Lcd_WriteData(0x00);
    Lcd_WriteData(x_start);
    Lcd_WriteData(0x00);
    Lcd_WriteData(x_end);

    Lcd_WriteIndex(0x2b);
    Lcd_WriteData(0x00);
    Lcd_WriteData(y_start);
    Lcd_WriteData(0x00);
    Lcd_WriteData(y_end);
    Lcd_WriteIndex(0x2c);
}

void PutPixel(uint x_start, uint y_start, uint color)
{
    Lcd_SetRegion(x_start, y_start, x_start + 1, y_start + 1);
    LCD_WriteData_16Bit(color);
}

void dsp_single_colour(unsigned int color)
{
    uchar i, j;
    Lcd_SetRegion(0, 0, 240 - 1, 240 - 1);
    for (i = 0; i < 240; i++)
        for (j = 0; j < 240; j++)
            LCD_WriteData_16Bit(color);
}

void dsp_single_colour_by_line(int color, int line)
{
    uchar i, j;
    Lcd_SetRegion(0, 0, 128 - 1, 10 + line * 16 - 1);
    for (i = 0; i < 10 + line * 16; i++)
        for (j = 0; j < 128; j++)
            LCD_WriteData_16Bit(color);
}
#include "../Inc/ST7789_font.h"

//采用设置窗口方法填充字库数据，相比Gui_DrawFont_GBK16更快速
void Fast_DrawFont_GBK16(uint x, uint y, uint fc, uint bc, uchar *s)
{
    unsigned char i, j;
    unsigned short k;
    uint HZnum;
    HZnum = sizeof(hz16) / sizeof(typFNT_GBK16);
    while (*s)
    {
        if ((*s) >= 128)
        {
            for (k = 0; k < HZnum; k++)
            {
                if ((hz16[k].Index[0] == *(s)) && (hz16[k].Index[1] == *(s + 1)))
                {
                    Lcd_SetRegion(x, y, x + 16 - 1, y + 16 - 1);
                    for (i = 0; i < 16 * 2; i++)
                    {
                        for (j = 0; j < 8; j++)
                        {
                            if (hz16[k].Msk[i] & (0x80 >> j))
                                LCD_WriteData_16Bit(fc);
                            else
                            {
                                if (fc != bc)
                                    LCD_WriteData_16Bit(bc);
                            }
                        }
                    }
                }
            }
            s += 2;
            x += 16;
        }

        else
            s += 1;
    }
}
#if 0
//显示一个英文字符
void Display_ASCII8X16(uint x0, uint y0, uchar *s)
{
    int i, j, k, x, y, xx;

    unsigned char qm;

    long int ulOffset;

    char ywbuf[32], temp[2];

    for (i = 0; i < strlen((char *)s); i++)
    {
        if (((unsigned char)(*(s + i))) >= 161)
        {
            temp[0] = *(s + i);
            temp[1] = '\0';
            return;
        }

        else
        {
            qm = *(s + i);

            ulOffset = (long int)(qm)*16;

            for (j = 0; j < 16; j++)
            {
                ywbuf[j] = Zk_ASCII8X16[ulOffset + j];
            }

            for (y = 0; y < 16; y++)
            {
                for (x = 0; x < 8; x++)
                {
                    k = x % 8;

                    if (ywbuf[y] & (0x80 >> k))
                    {
                        xx = x0 + x + i * 8;
                        PutPixel(xx, y + y0, BLACK);
                    }
                }
            }
        }
    }
}
#endif
// bit busy;

// void SPI_Isr() interrupt 9
// {
//     SPSTAT = 0xc0; //清中断标志
//     busy = 0;
// }
/*
int main1()
{
    uchar c[20];
    init_all_gpio_to_std8051_status();
   SPI_WriteData_by_HW_Init();
    lcd_initial();
   // bl = 1;
    dsp_single_colour(GREEN);
    while (1)
    {
        //  dsp_single_colour_by_line(GREEN, 1); //绿色
        //  sprintf(c, "total:%d", i++);
        //  Display_ASCII8X16(10, 10, c);

      //  delay_ms(10);
        dsp_single_colour(RED);   //红色
        dsp_single_colour(GREEN); //绿色
        dsp_single_colour(BLUE);  //蓝色
        dsp_single_colour(WHITE); //白色
      //  delay_ms(500);
       // dsp_single_colour(WHITE); //白色
       // Display_Desc();           //版本
       
    }
}
*/

void Font_Test(void)
{
    //中文显示测试全动电子欢迎您因为专业所以注
    dsp_single_colour(WHITE);
    Fast_DrawFont_GBK16(8, 10, BLUE, YELLOW, "中文测试");
    Fast_DrawFont_GBK16(16, 50, BLUE, WHITE, "欢迎您");
    Fast_DrawFont_GBK16(8, 70, WHITE, BLUE, "因为专注");
    Fast_DrawFont_GBK16(8, 90, WHITE, BLUE, "所以专业");
    Fast_DrawFont_GBK16(16, 150, BLUE, WHITE, "欢迎您");
    Fast_DrawFont_GBK16(16, 190, BLUE, WHITE, "欢迎您");
    delay_ms(1800);
}

uchar __code Zk_ASCII8X16[] =
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x18, 0x08, 0xc8, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x7f, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0xe0,
        0x60, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x10,
        0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x0c, 0x0c, 0x0c, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x6c, 0x38, 0x10, 0x00, 0x7c, 0xc6, 0x70, 0x1c,
        0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x6c, 0x00, 0x78,
        0x0c, 0x7c, 0xcc, 0xcc, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x7c, 0xc6, 0x70, 0xc0, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x6c, 0x96, 0x7c, 0xd0, 0xd6, 0x6c, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0x06, 0x06, 0xc6,
        0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x6c, 0xc6, 0xc6, 0xfe,
        0xc6, 0xc6, 0x6c, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x10, 0x38, 0x28, 0x6c, 0x44, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0x06, 0xfe, 0xc6, 0x7c, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x66, 0x66, 0x66, 0xf6, 0x06,
        0x06, 0x66, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x6c, 0x38, 0xf8, 0x0c, 0x7e, 0xc6,
        0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x1b, 0x18,
        0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xd8, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xfe, 0x86, 0x0c, 0x18, 0x0c, 0x06, 0x06, 0xc6, 0x7c, 0x00, 0x00,
        0x00, 0x00, 0x76, 0xdc, 0x00, 0x00, 0x78, 0x0c, 0x7c, 0xcc, 0xcc, 0x76, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xdc, 0x00, 0x00, 0x7c, 0xc6, 0x06, 0x06,
        0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xdc, 0x00, 0x00, 0xdc,
        0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x76, 0x99, 0x9f, 0x98, 0x99, 0x6f, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x06, 0x06,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c,
        0x66, 0x66, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3c,
        0x3c, 0x3c, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xc6, 0xc6, 0xc6, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x6c, 0x6c, 0xfe, 0x6c, 0x6c, 0x6c, 0xfe, 0x6c, 0x6c,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x7c, 0xc6, 0xc2, 0xc0, 0x7c, 0x06,
        0x86, 0xc6, 0x7c, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3,
        0xc6, 0x0c, 0x18, 0x30, 0x63, 0xc3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x38, 0x6c, 0x6c, 0x38, 0x76, 0xdc, 0xcc, 0xcc, 0x76, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x30, 0x30, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x30, 0x60, 0x60, 0x60, 0x60, 0x60,
        0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x0c, 0x06, 0x06,
        0x06, 0x06, 0x06, 0x0c, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x6c, 0x38, 0xfe, 0x38, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x7e, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18,
        0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x02, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xce, 0xde, 0xf6, 0xe6, 0xc6, 0xc6, 0x7c,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x78, 0x18, 0x18, 0x18,
        0x18, 0x18, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0x06,
        0x0c, 0x18, 0x30, 0x60, 0xc6, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x7c, 0xc6, 0x06, 0x06, 0x3c, 0x06, 0x06, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x0c, 0x1c, 0x3c, 0x6c, 0xcc, 0xfe, 0x0c, 0x0c, 0x1e, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xc0, 0xc0, 0xc0, 0xfc, 0x06, 0x06,
        0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x60, 0xc0, 0xc0,
        0xfc, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe,
        0xc6, 0x06, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0x7e, 0x06, 0x06, 0x0c,
        0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
        0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
        0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x06, 0x0c, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x7e, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x0c,
        0x18, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6,
        0x0c, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x7c, 0xc6, 0xc6, 0xde, 0xde, 0xde, 0xdc, 0xc0, 0x7c, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x10, 0x38, 0x6c, 0xc6, 0xc6, 0xfe, 0xc6, 0xc6, 0xc6, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x66, 0x66, 0x66, 0x7c, 0x66, 0x66,
        0x66, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x66, 0xc2, 0xc0,
        0xc0, 0xc0, 0xc2, 0x66, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8,
        0x6c, 0x66, 0x66, 0x66, 0x66, 0x66, 0x6c, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xfe, 0x66, 0x62, 0x68, 0x78, 0x68, 0x62, 0x66, 0xfe, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x66, 0x62, 0x68, 0x78, 0x68, 0x60, 0x60,
        0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x66, 0xc2, 0xc0, 0xc0,
        0xde, 0xc6, 0x66, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6,
        0xc6, 0xc6, 0xfe, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x3c, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x1e, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0xcc, 0xcc, 0x78,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe6, 0x66, 0x6c, 0x6c, 0x78, 0x6c,
        0x6c, 0x66, 0xe6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x60, 0x60,
        0x60, 0x60, 0x60, 0x62, 0x66, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xc6, 0xee, 0xfe, 0xd6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xc6, 0xe6, 0xf6, 0xfe, 0xde, 0xce, 0xc6, 0xc6, 0xc6, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x6c, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6,
        0x6c, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x66, 0x66, 0x66,
        0x7c, 0x60, 0x60, 0x60, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c,
        0xc6, 0xc6, 0xc6, 0xd6, 0xde, 0x7c, 0x0c, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xfc, 0x66, 0x66, 0x66, 0x7c, 0x6c, 0x66, 0x66, 0xe6, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc6, 0x60, 0x38, 0x0c, 0xc6, 0xc6,
        0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x5a, 0x18, 0x18, 0x18,
        0x18, 0x18, 0x18, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6,
        0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x6c, 0x38, 0x10, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xd6, 0xfe, 0xee, 0xc6,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0x6c, 0x38, 0x6c,
        0xc6, 0xc6, 0xc6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6,
        0x6c, 0x38, 0x38, 0x38, 0x38, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xfe, 0xc6, 0x8c, 0x18, 0x30, 0x60, 0xc2, 0xc6, 0xfe, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x3c, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3c, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0x70, 0x38, 0x1c, 0x0e,
        0x06, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x0c, 0x0c, 0x0c,
        0x0c, 0x0c, 0x0c, 0x0c, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x6c,
        0xc6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe,
        0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0c,
        0x7c, 0xcc, 0xcc, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x60,
        0x60, 0x78, 0x6c, 0x66, 0x66, 0x66, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xc0, 0xc0, 0xc6, 0x7c, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x1c, 0x0c, 0x0c, 0x3c, 0x6c, 0xcc, 0xcc, 0xcc, 0x76,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xc6, 0xfe,
        0xc0, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x36, 0x32,
        0x30, 0x7c, 0x30, 0x30, 0x30, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x76, 0xcc, 0xcc, 0xcc, 0x7c, 0x0c, 0xcc, 0x78, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xe0, 0x60, 0x60, 0x6c, 0x76, 0x66, 0x66, 0x66, 0xe6, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x38, 0x18, 0x18, 0x18,
        0x18, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x0e,
        0x06, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
        0x60, 0x60, 0x66, 0x6c, 0x78, 0x6c, 0x66, 0xe6, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0xfe, 0xd6, 0xd6, 0xd6,
        0xd6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x66,
        0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x7c, 0xc6, 0xc6, 0xc6, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0xdc, 0x66, 0x66, 0x66, 0x7c, 0x60, 0x60, 0xf0, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xcc, 0xcc, 0xcc, 0x7c, 0x0c,
        0x0c, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0x76, 0x66,
        0x60, 0x60, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x7c, 0xc6, 0x70, 0x1c, 0xc6, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x10, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30, 0x36, 0x1c, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x76, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0x6c,
        0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc6,
        0xc6, 0xc6, 0xd6, 0xfe, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xc6, 0x6c, 0x38, 0x38, 0x6c, 0xc6, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xc6, 0xc6, 0xc6, 0xc6, 0x7e, 0x06, 0x0c, 0x78,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xcc, 0x18, 0x30, 0x66,
        0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x18, 0x18, 0x18, 0x70,
        0x18, 0x18, 0x18, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18,
        0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x70, 0x18, 0x18, 0x18, 0x0e, 0x18, 0x18, 0x18, 0x70, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x76, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00};
