#include <Wire.h> 
#include <U8g2lib.h>
#include <LiquidCrystal_I2C.h>

// 设置I2C地址为0x27，16列2行
LiquidCrystal_I2C lcd(0x27, 16, 2);  

// CLK = 4, MOSI = 3, CS = 2
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/4, /* data=*/3, /* cs=*/2);
 
void setup()
{
  u8g2.begin();
  lcd.init();                // 初始化LCD
  lcd.backlight();           // 打开背光
  lcd.setCursor(0,0);       // 将光标设置到第一行第一列
  lcd.print("Hello, world!");  // 在LCD上打印"Hello, world!"
}
 
void loop() {
	u8g2.firstPage();
	do {
		u8g2.setFont(u8g2_font_ncenB08_tr);	 // 选择字体
		u8g2.drawStr(0, 24, "Hello, world!");  // 在坐标(0,24)处开始写入文本
	} while (u8g2.nextPage());
	delay(1000);
}



