#include <Wire.h> 
#include <U8g2lib.h>
#include <LiquidCrystal_I2C.h>

// 设置I2C地址为0x27，16列2行
LiquidCrystal_I2C lcd(0x27, 16, 2);  

// CLK = 4, MOSI = 3, CS = 2
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/4, /* data=*/3, /* cs=*/2);

int voltageValues[128];  // 用于存储电压值的数组
int minVoltage = 1023;  // 最小电压值
int maxVoltage = 0;  // 最大电压值
long totalVoltage = 0;  // 电压总和，用于计算平均值

void setup() {
  u8g2.begin();
  
  lcd.init();                // 初始化LCD
  lcd.backlight();           // 打开背光

  // 初始化电压值数组
  for (int i = 0; i < 128; i++) {
    voltageValues[i] = 0;
  }
}

void loop() {
  // 从A0引脚读取电压值
  int voltage = analogRead(A0);

  // 更新最大和最小电压值
  if (voltage < minVoltage) {
    minVoltage = voltage;
  }
  if (voltage > maxVoltage) {
    maxVoltage = voltage;
  }

  // 更新电压总和
  totalVoltage -= voltageValues[0];
  totalVoltage += voltage;

  // 将新的电压值添加到数组中，并移除最旧的电压值
  for (int i = 0; i < 127; i++) {
    voltageValues[i] = voltageValues[i + 1];
  }
  voltageValues[127] = voltage;

  // 在LCD12864上绘制信号图像
  u8g2.firstPage();
  do {
    u8g2.drawFrame(0, 0, 128, 64);
    for (int i = 1; i < 128; i++) {
      u8g2.drawLine(i - 1, 64 - voltageValues[i - 1] / 16, i, 64 - voltageValues[i] / 16);
    }
  } while (u8g2.nextPage());

  // 在LCD1602上显示参数
  lcd.setCursor(0, 0);
  lcd.print("Val=");
  lcd.print(voltage * (5.0 / 1023.0), 2);  // 转换为实际电压值
  lcd.setCursor(8, 0);
  lcd.print("Avg=");
  lcd.print((totalVoltage / 128.0) * (5.0 / 1023.0), 2);  // 计算并显示平均电压值
  lcd.setCursor(0, 1);
  lcd.print("Min=");
  lcd.print(minVoltage * (5.0 / 1023.0), 2);  // 转换为实际电压值
  lcd.setCursor(8, 1);
  lcd.print("Max=");
  lcd.print(maxVoltage * (5.0 / 1023.0), 2);  // 转换为实际电压值


  delay(20);
}
