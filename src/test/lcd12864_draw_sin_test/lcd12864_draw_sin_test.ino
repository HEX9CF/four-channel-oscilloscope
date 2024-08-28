#include <U8g2lib.h>

// CLK = 4, MOSI = 3, CS = 2
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 4, /* data=*/ 3, /* cs=*/ 2);

#define WIDTH 128
#define HEIGHT 64
#define MID_HEIGHT 32
#define SCALE 10 // 刻度间隔

void setup() {
  u8g2.begin();
}

void loop() {
  static int shift = 0;

  u8g2.clearBuffer(); // 清除缓冲区

  // 画坐标轴
  u8g2.drawLine(0, MID_HEIGHT, WIDTH, MID_HEIGHT); // X轴
  u8g2.drawLine(WIDTH / 2, 0, WIDTH / 2, HEIGHT); // Y轴

  // 画刻度
  for (int i = 0; i <= WIDTH; i += SCALE) {
    u8g2.drawLine(i, MID_HEIGHT - 2, i, MID_HEIGHT + 2); // X轴刻度
  }
  for (int i = 0; i <= HEIGHT; i += SCALE) {
    u8g2.drawLine(WIDTH / 2 - 2, i, WIDTH / 2 + 2, i); // Y轴刻度
  }

  // 画正弦波
  int lastY = MID_HEIGHT + (MID_HEIGHT - 10) * sin(shift * 2.0 * PI / WIDTH);
  for (int x = 1; x < WIDTH; x++) {
    int y = MID_HEIGHT + (MID_HEIGHT - 10) * sin((x + shift) * 2.0 * PI / WIDTH);
    u8g2.drawLine(x - 1, lastY, x, y);
    lastY = y;
  }

  u8g2.sendBuffer(); // 将缓冲区的内容发送到显示器

  shift++; // 每次循环移动正弦波
  if (shift >= WIDTH) {
    shift = 0; // 如果正弦波移动到屏幕外，就从头开始
  }

  delay(20); // 控制动画的速度
}
