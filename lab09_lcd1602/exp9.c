#include <reg52.h>
#include <intrins.h>

typedef unsigned char uchar;
typedef unsigned int  uint;

#define LCD_DATA P0
sbit LCD_RS = P2^0;
sbit LCD_RW = P2^1;
sbit LCD_EN = P2^2;

void Delay1ms(uint ms);
void lcd_busy(void);
void lcd_wcmd(uchar cmd);
void lcd_wdat(uchar dat);
void lcd_clr(void);
void lcd_init(void);

/******** 1ms 延时（12MHz 晶振）********/
void Delay1ms(uint ms)
{
    uint i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

/******** 读忙信号 ********/
void lcd_busy(void)
{
    uchar sta;
    LCD_DATA = 0xFF;
    do {
        LCD_RS = 0;
        LCD_RW = 1;
        _nop_();
        LCD_EN = 1;
        _nop_();
        sta = LCD_DATA;
        LCD_EN = 0;
    } while (sta & 0x80);
}

/******** 写命令 ********/
void lcd_wcmd(uchar cmd)
{
    lcd_busy();
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_DATA = cmd;
    _nop_();
    LCD_EN = 1;
    _nop_();
    LCD_EN = 0;
}

/******** 写数据 ********/
void lcd_wdat(uchar dat)
{
    lcd_busy();
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_DATA = dat;
    _nop_();
    LCD_EN = 1;
    _nop_();
    LCD_EN = 0;
}

/******** 清屏 ********/
void lcd_clr(void)
{
    lcd_wcmd(0x01);
    Delay1ms(5);
}

/******** LCD 初始化 ********/
void lcd_init(void)
{
    Delay1ms(15);      /* 等待 LCD 电源稳定 */
    lcd_wcmd(0x38);    /* 16*2 显示，5*7 点阵，8 位数据 */
    Delay1ms(5);
    lcd_wcmd(0x38);
    Delay1ms(5);
    lcd_wcmd(0x38);
    Delay1ms(5);
    lcd_wcmd(0x0c);    /* 显示开、关光标 */
    Delay1ms(5);
    lcd_wcmd(0x06);    /* 移动光标 */
    Delay1ms(5);
    lcd_wcmd(0x01);    /* 清除 LCD 显示内容 */
}

uchar code line1[] = {"Hello,Welcome"};
uchar code line2[] = {"51 MCU World!"};

void main(void)
{
    uchar i = 0;
    lcd_init();
    lcd_wcmd(0x02 | 0x80);   /* 从第一行第二列开始写 */
    while (line1[i] != '\0')
    {
        lcd_wdat(line1[i]);  /* 写入第一行字符 */
        Delay1ms(200);       /* 延时越长，飞入效果越明显 */
        i++;
    }

    i = 0;
    lcd_wcmd(0x42 | 0x80);   /* 从第二行第二列开始写 */
    while (line2[i] != '\0')
    {
        lcd_wdat(line2[i]);  /* 写入第二行字符 */
        Delay1ms(200);
        i++;
    }

    while (1);  /* 写完等待 */
}
