#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// 初始化LCD12864
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 4, /* data=*/ 3, /* cs=*/ 2);

// 初始化LCD1602
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // 启动LCD12864
  u8g2.begin();

  // 启动LCD1602
  lcd.begin(16, 2);
  lcd.backlight();
}

void loop() {
  // 在LCD12864上显示文字
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,24,"Hello, LCD12864!");
  } while ( u8g2.nextPage() );

  // 在LCD1602上显示文字
  lcd.setCursor(0, 0);
  lcd.print("Hello, LCD1602!");
  
  delay(1000);
}
