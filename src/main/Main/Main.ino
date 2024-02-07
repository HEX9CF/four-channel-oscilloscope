#include <Wire.h> 
#include <U8g2lib.h>
#include <LiquidCrystal_I2C.h>

// 设置I2C地址为0x27，16列2行
LiquidCrystal_I2C lcd(0x27, 16, 2);  

// CLK = 4, MOSI = 3, CS = 2
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/4, /* data=*/3, /* cs=*/2);

byte voltageValues[4][64];  // 用于存储电压值的数组

void setup() {
  u8g2.begin();
  
  lcd.init();                // 初始化LCD
  lcd.backlight();           // 打开背光

  // 初始化电压值数组
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 64; i++) {
      voltageValues[j][i] = 0;
    }
  }
}

void loop() {
  // 从A0到A3引脚读取电压值
  for (int j = 0; j < 4; j++) {
    byte voltage = analogRead(j) / 4;  // 除以4以将电压值减少到8位

    // 将新的电压值添加到数组中，并移除最旧的电压值
    for (int i = 0; i < 63; i++) {
      voltageValues[j][i] = voltageValues[j][i + 1];
    }
    voltageValues[j][63] = voltage;
  }

  // 在LCD12864上绘制信号图像
  u8g2.firstPage();
  do {
    for (int j = 0; j < 4; j++) {
      int x = (j % 2) * 64; // 计算区域的x坐标
      int y = (j / 2) * 32; // 计算区域的y坐标

      u8g2.drawFrame(x, y, 64, 32);
      for (int i = 1; i < 64; i++) {
        int x1 = x + (i - 1);
        int y1 = y + 32 - voltageValues[j][i - 1] / 8;
        int x2 = x + i;
        int y2 = y + 32 - voltageValues[j][i] / 8;

        u8g2.drawLine(x1, y1, x2, y2);
      }
      // Draw channel label
      u8g2.setFont(u8g2_font_4x6_mf);
      char ch[2] = {'A', '0'};
      ch[1] = '0' + j;
      u8g2.drawStr(x + 3, y + 8, ch);
    }
  } while (u8g2.nextPage());

  // 在LCD1602上显示参数
  lcd.setCursor(0, 0);
  float a0 = voltageValues[0][63] * (5.0 / 255.0);  // 转换为实际电压值
  lcd.print("A0=");
  lcd.print(a0, 2);

  lcd.setCursor(8, 0);
  float a1 = voltageValues[1][63] * (5.0 / 255.0);  // 转换为实际电压值
  lcd.print("A1=");
  lcd.print(a1, 2);

  lcd.setCursor(0, 1);
  float a2 = voltageValues[2][63] * (5.0 / 255.0);  // 转换为实际电压值
  lcd.print("A2=");
  lcd.print(a2, 2);

  lcd.setCursor(8, 1);
  float a3 = voltageValues[3][63] * (5.0 / 255.0);  // 转换为实际电压值
  lcd.print("A3=");
  lcd.print(a3, 2);

  delay(20);
}
