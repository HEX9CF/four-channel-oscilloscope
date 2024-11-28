// 引脚的上一次切换状态的时间
unsigned long updateTime[11];

// 引脚的当前状态
int state[11];

void setup() {
	// 初始化引脚
	for (int i = 2; i <= 12; i++) {
		// 设置为输出模式
		pinMode(i, OUTPUT);

		// 默认状态为LOW
		state[i - 2] = LOW;

		// 初始化更新时间
		updateTime[i - 2] = 0;
	}
}

void loop() {
	unsigned long currentTime = millis();  // 获取当前时间
	for (int i = 2; i <= 12; i++) {
		if (currentTime - updateTime[i - 2] >= (i - 1) * 1000) {
			// 切换引脚状态
			if (state[i - 2] == LOW) {
				state[i - 2] = HIGH;
			} else {
				state[i - 2] = LOW;
			}

			// 更新引脚的状态
			digitalWrite(i, state[i - 2]);

			// 更新更新时间
			updateTime[i - 2] = currentTime;
		}
	}
}
