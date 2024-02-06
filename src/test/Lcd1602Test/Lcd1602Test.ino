#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// 设置I2C地址为0x27，16列2行
LiquidCrystal_I2C lcd(0x27, 16, 2);  
 
void setup()
{
  lcd.init();                // 初始化LCD
  lcd.backlight();           // 打开背光
  lcd.setCursor(0,0);       // 将光标设置到第一行第一列
  lcd.print("Hello, world!");  // 在LCD上打印"Hello, world!"
}
 
void loop()
{
  // 这里我们不需要做任何事情，所以loop函数为空
}
