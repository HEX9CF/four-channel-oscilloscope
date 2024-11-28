#include <LiquidCrystal_I2C.h>
#include <U8g2lib.h>
#include <Wire.h>

// 定义LCD1602引脚
// I2C地址
const byte I2C_ADDRESS = 0x27;
// 列数
const byte LCD1602_COLUMN = 16;
// 行数
const byte LCD1602_ROW = 2;

// 定义LCD1602
LiquidCrystal_I2C lcd(I2C_ADDRESS, LCD1602_COLUMN, LCD1602_ROW);

// 定义LCD12864引脚
// 时钟 CLK
const byte LCD12864_CLK = 4;  // clock
// 数据 MOSI
const byte LCD12864_MOSI = 3;  // data
// 片选 CS
const byte LCD12864_CS = 2;	 // cs

// 定义LCD12864
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, LCD12864_CLK, LCD12864_MOSI,
								 LCD12864_CS);

// 通道数
const byte CHANNEL_COUNT = 4;

// 采样数
const byte SAMPLE_COUNT = 64;

// 详细参数通道切换时间
const int SWITCH_TIME = 2000;

// 延迟时间
// 20ms采样一次，采样频率为50Hz
const int DELAY_TIME = 20;

// 通道标签偏移量
const int LABEL_X_OFFSET = 3;
const int LABEL_Y_OFFSET = 8;

// 当前时间
unsigned long currentTime = 0;

// 上次更新时间
unsigned long updateTime = 0;

// 采样值
byte sampleValues[CHANNEL_COUNT][SAMPLE_COUNT];

// 初始化
void setup() {
	// 初始化LCD1602
	lcd.init();
	// 打开LCD1602背光
	lcd.backlight();

	// 初始化LCD12864
	u8g2.begin();

	// 初始化采样值数组
	for (int j = 0; j < CHANNEL_COUNT; j++) {
		for (int i = 0; i < SAMPLE_COUNT; i++) {
			sampleValues[j][i] = 0;
		}
	}

	// 初始化时间
	currentTime = 0;
	updateTime = 0;

	// 初始化串口
	// Serial.begin(9600);
}

// 主循环
void loop() {
	// 更新当前时间
	currentTime = millis();

	if (currentTime - updateTime >= DELAY_TIME) {
		// 更新更新时间
		updateTime = currentTime;

		// 从A0到A3引脚读取采样值
		for (int j = 0; j < CHANNEL_COUNT; j++) {
			// 将采样值减少到8位
			int sampleValue = analogRead(j) / 4;

			// 将采样值向左移动一个位置
			for (int i = 0; i < SAMPLE_COUNT - 1; i++) {
				sampleValues[j][i] = sampleValues[j][i + 1];
			}
			// 将新的采样值添加到数组中
			sampleValues[j][SAMPLE_COUNT - 1] = sampleValue;
		}

		// 计算通道
		int chan = currentTime % (CHANNEL_COUNT * SWITCH_TIME) / SWITCH_TIME;

		// 计算实际值
		float actVal = sampleValues[chan][SAMPLE_COUNT - 1] * (5.0 / 255.0);

		// 初始化
		float avgVal = 0;
		float maxVal = 0;
		float minVal = 255;

		for (int j = 0; j < SAMPLE_COUNT; j++) {
			// 计算实际值
			float tmp = sampleValues[chan][j] * (5.0 / 255.0);

			// 计算平均值
			avgVal += tmp / SAMPLE_COUNT;

			// 计算最大值
			maxVal = max(maxVal, tmp);

			// 计算最小值
			minVal = min(minVal, tmp);
		}

		// 输出到串口
		// Serial.print("A");
		// Serial.print(chan);
		// Serial.print("=");
		// Serial.print(actVal, 2);
		// Serial.print(", Avg=");
		// Serial.print(avgVal, 2);
		// Serial.print(", Min=");
		// Serial.print(minVal, 2);
		// Serial.print(", Max=");
		// Serial.print(maxVal, 2);
		// Serial.println();

		// 在LCD1602上显示参数
		// 显示实际值
		lcd.setCursor(0, 0);
		// lcd.print("A0=");
		lcd.print("A");
		lcd.print(chan);
		lcd.print("=");
		lcd.print(actVal, 2);

		// 显示平均值
		lcd.setCursor(8, 0);
		// lcd.print("A1=");
		lcd.print("Avg");
		lcd.print(avgVal, 2);

		// 显示最小值
		lcd.setCursor(0, 1);
		// lcd.print("A2=");
		lcd.print("Min");
		lcd.print(minVal, 2);

		// 显示最大值
		lcd.setCursor(8, 1);
		// lcd.print("A3=");
		lcd.print("Max");
		lcd.print(maxVal, 2);

		// 在LCD12864上绘制信号图像
		u8g2.firstPage();
		do {
			// 绘制各个通道的信号图像
			for (int j = 0; j < CHANNEL_COUNT; j++) {
				// 计算区域坐标
				int x = (j % 2) * 64;
				int y = (j / 2) * 32;

				// 绘制边框
				u8g2.drawFrame(x, y, 64, 32);
				for (int i = 1; i < SAMPLE_COUNT; i++) {
					// 计算坐标
					int x1 = x + (i - 1);
					int y1 = y + 31 - sampleValues[j][i - 1] / 8;
					int x2 = x + i;
					int y2 = y + 31 - sampleValues[j][i] / 8;

					// 绘制线段
					u8g2.drawLine(x1, y1, x2, y2);
				}

				// 设置字体
				u8g2.setFont(u8g2_font_4x6_mf);

				// 生成字符串
				char ch[3];
				sprintf(ch, "A%d", j);

				// 绘制通道标签
				u8g2.drawStr(x + LABEL_X_OFFSET, y + LABEL_Y_OFFSET, ch);
			}
		} while (u8g2.nextPage());
	}
}
