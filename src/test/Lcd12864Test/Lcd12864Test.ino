#include <U8g2lib.h>

// CLK = 4, MOSI = 3, CS = 2
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/4, /* data=*/3, /* cs=*/2);

void setup() { u8g2.begin(); }

void loop() {
	u8g2.firstPage();
	do {
		u8g2.setFont(u8g2_font_ncenB08_tr);	 // 选择字体
		u8g2.drawStr(0, 24, "Hello, world!");  // 在坐标(0,24)处开始写入文本
	} while (u8g2.nextPage());
	delay(1000);
}
