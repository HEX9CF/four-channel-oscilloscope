#include <U8g2lib.h>

// CLK = 4, MOSI = 3, CS = 2
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 4, /* data=*/ 3, /* cs=*/ 2);

#define WIDTH 128
#define HEIGHT 64
#define MID_HEIGHT 32

void setup() {
  u8g2.begin();
}

void loop() {
  static int shift = 0;

  u8g2.clearBuffer(); // 清除缓冲区

  for (int x = 0; x < WIDTH; x++) {
    int y = MID_HEIGHT + (MID_HEIGHT - 10) * sin((x + shift) * 2.0 * PI / WIDTH);
    u8g2.drawPixel(x, y);
  }

  u8g2.sendBuffer(); // 将缓冲区的内容发送到显示器

  shift++; // 每次循环移动正弦波
  if (shift >= WIDTH) {
    shift = 0; // 如果正弦波移动到屏幕外，就从头开始
  }

  delay(20); // 控制动画的速度
}
