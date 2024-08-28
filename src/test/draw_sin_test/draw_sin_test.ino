#include <Wire.h> 
#include <U8g2lib.h>
#include <LiquidCrystal_I2C.h>

// 设置I2C地址为0x27，16列2行
LiquidCrystal_I2C lcd(0x27, 16, 2);  

// CLK = 4, MOSI = 3, CS = 2
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/4, /* data=*/3, /* cs=*/2);
 
// 定义sin函数的参数
float A = 1;
float w = 0.1;
float p = 0;

void setup() {
  u8g2.begin();
  
  lcd.init();                // 初始化LCD
  lcd.backlight();           // 打开背光
}

void loop() {
  u8g2.firstPage();
  do {
    // 绘制边框
    u8g2.drawFrame(0, 0, 128, 64);

    // 绘制坐标轴
    u8g2.drawLine(0, 32, 128, 32);  // x轴
    u8g2.drawLine(64, 0, 64, 64);  // y轴

    // 绘制刻度
    for (int i = 10; i < 128; i += 10) {
      u8g2.drawLine(i, 30, i, 34);  // x轴刻度
    }
    for (int i = 10; i < 64; i += 10) {
      u8g2.drawLine(62, i, 66, i);  // y轴刻度
    }

    // 绘制sin函数
    int old_y = 32 + A * sin(w * 0 + p) * 30;  // 调整y的范围以适应显示屏
    for (int x = 1; x < 127; x++) {
      int new_y = 32 + A * sin(w * x + p) * 30;  // 调整y的范围以适应显示屏
      u8g2.drawLine(x-1, old_y, x, new_y);
      old_y = new_y;
    }

  } while (u8g2.nextPage());

  // 计算并显示0,0处的y值
  float y = A * sin(w * 64 + p);
  lcd.setCursor(0, 0);
  lcd.print("A=");
  lcd.print(A);
  lcd.setCursor(0, 1);
  lcd.print("w=");
  lcd.print(w);
  lcd.setCursor(8, 0);
  lcd.print("p=");
  lcd.print(p);
  lcd.setCursor(8, 1);
  lcd.print("y=");
  lcd.print(y);

  // 更新参数
  p += 0.1;
  if (p > 2 * PI) {
    p -= 2 * PI;
  }

  delay(100);
}
