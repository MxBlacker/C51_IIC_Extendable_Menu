#include <REGX52.H>
#include "stdint.h"
#include "SSD1306.h"
#include "Menu.h"

#define HIGH	1
#define LOW		0
#define HOLD_THRESHOLD 100

void delay_us(uint16_t ms);
void delay_ms(uint16_t ms);

uint8_t Button_Counter, Hold_Counter[4];
uint8_t prev_state[4], cur_state[4];
uint8_t i, temp;

void main(void)
{
    
    Menu_Init();
    
    // 初始化所有计数器
    for (i = 0; i < 4; i++) {
        Hold_Counter[i] = 0;
    }
    
    while (1)
    {
        Button_Counter++;
        
        if (Button_Counter > 9) {  // 约10ms扫描一次
            
            for (temp = 0; temp < 4; temp++) {
                // 读取当前状态
                prev_state[temp] = cur_state[temp];
                cur_state[temp] = (temp == 0) ? P3_1 : 
                                  (temp == 1) ? P3_0 : 
                                  (temp == 2) ? P3_2 : P3_3;
                
                // 下降沿检测（按下瞬间）
                if (cur_state[temp] == LOW && prev_state[temp] == HIGH) {
                    // 短按触发一次
                    switch (temp) {
                        case 0: Menu_Up(); break;
                        case 1: Menu_Down(); break;
                        case 2: Menu_Forward(); break;
                        case 3: Menu_Backward(); break;
                    }
                    Hold_Counter[temp] = 0;  // 重置长按计数器
                }

                // 长按检测（按钮保持按下）
                else if (cur_state[temp] == LOW && prev_state[temp] == LOW) {
                    Hold_Counter[temp]++;
                    if (Hold_Counter[temp] >= HOLD_THRESHOLD) {
                        if (Hold_Counter[temp] % 2 == 0) {  // 长按时每100ms触发一次
                            switch (temp) {
                                case 0: Menu_Up(); break;
                                case 1: Menu_Down(); break;
                                case 2: Menu_Forward(); break;
                                case 3: Menu_Backward(); break;
                            }
                        }
                    }
                } else {
                    Hold_Counter[temp] = 0;  // 按钮松开，重置计数器
                }
            }
            
            Button_Counter = 0;
        }
        
        delay_ms(1);  // 适当延时，避免CPU过载
    }
}

void delay_us(uint16_t us)
{
	us /= 10;
	while(--us);
}

void delay_ms(uint16_t ms)
{
	while(--ms)	delay_us(950);
}