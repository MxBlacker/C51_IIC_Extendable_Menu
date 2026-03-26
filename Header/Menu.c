#include <REGX52.H>
#include "stdint.h"
#include "SSD1306.h"
#include "Menu.h"

/* ==============================================================================================
                                        界面定义
   ============================================================================================== */
Interface_TypeDef code interface[2] = {
    /* MAIN_MENU */
    {
        MAIN_MENU,           									// .ID
        4,                  									// .option_count
        {"INT16", "FLOAT", "OPTIONS", "SAVE"},  						// .option_text
        {EDITABLE, FLOAT_EDITABLE, SUBINTERFACE, INTERACTIBLE},  				// .option_mode
        {0, 0, 0},                                              // .option_action
        {MAIN_MENU, MAIN_MENU, OPTION_MENU, MAIN_MENU},                    // .subinterface
        MAIN_MENU,           									// .super_interface
    },
    /* OPTION_MENU */
    {
        OPTION_MENU,                                            // .ID
        3,                                                      // .option_count
        {"STEP", "sINT16", "sFLOAT"},                                    // .option_text
        {EDITABLE, READ_EEP, READ_EEP},                                   // .option_mode
        {0, 0, 0},                                                 // .option_action
        {MAIN_MENU, MAIN_MENU},                                 // .subinterface
        MAIN_MENU,                                              // .super_interface
    },
};

/* ==============================================================================================
                                        全局变量定义
   ============================================================================================== */

interface_id            xdata current_interface;              // 目前的界面ID
uint8_t                 xdata current_option_index;           // 目前选的选项的index
uint8_t                 xdata current_mode;                   // 目前的模式：EDIT_MODE 和 SELECT_MODE

float					xdata value[2][3] = {0, 0, 0, 1, 0, 0};

/* ==============================================================================================
                                        内部函数定义
   ============================================================================================== */


/* ==============================================================================================
                                        外部函数定义
   ============================================================================================== */

/** 
 * @brief 菜单的初始化函数
 * @note 直接使用 `Menu_Init();` 即可
 * @return void
 */
void Menu_Init(void)
{
	OLED_Init();
	OLED_Clear();
	OLED_Display_On();
    current_interface = MAIN_MENU;
    current_option_index = 0;
    current_mode = SELECT_MODE;
    Menu_Refresh();
}

static uint8_t xdata i;
/** 
 * @brief 刷新界面
 * @note 根据当前的信息在OLED上打印信息。
 * @return void
 */
void Menu_Refresh(void)
{
	 
    // 显示文本 'TEXT'
    for (i = 0; i < CURRENT_OPTION_COUNT; i++) {
        if (current_option_index == i) {
            OLED_ShowString(0, i, "> ", 6, 0);
        } else {
			OLED_ShowString(0, i, "  ", 6, 0);
		}
        OLED_ShowString(16, i, interface[current_interface].option_text[i], 6, 0);
    }

    // 显示模式 'EDT / RUN'
    if (current_mode == EDIT_MODE) {
        OLED_ShowString(0, 7, "EDIT", 6, 0);
    } else {
        OLED_ShowString(0, 7, "SLEC", 6, 0);
    }

    // 显示数据
    for (i = 0; i < CURRENT_OPTION_COUNT; i++) {
        if (interface[current_interface].option_mode[i] == EDITABLE) {
            OLED_ShowNum(72, i, (int)value[current_interface][i], 3, 6, 0);
        } else if (interface[current_interface].option_mode[i] == FLOAT_EDITABLE) {
			OLED_ShowFloat(72, i, value[current_interface][i] * 100, 0);
		} else if (interface[current_interface].option_mode[i] == READ_EEP) {
			// WIP
		}
    }
	
	OLED_Refresh_Gram();
}

/** 
 * @brief 向上操作，对应 E5
 * @note 和按钮热血沸腾的组合技
 * @return void
 */
void Menu_Up(void)
{
    if (current_mode == SELECT_MODE) {
        current_option_index = ((current_option_index - 1) + CURRENT_OPTION_COUNT) % CURRENT_OPTION_COUNT;
    } else if (current_mode == EDIT_MODE) {
		if (current_interface == MAIN_MENU && current_option_index == 0) {
			CURRENT_OPTION_VALUE = (CURRENT_OPTION_VALUE + 1 * value[OPTION_MENU][0]);
		} else if (CURRENT_OPTION_MODE == FLOAT_EDITABLE){
			CURRENT_OPTION_VALUE = (CURRENT_OPTION_VALUE + 0.01 * value[OPTION_MENU][0]);
		} else {
			CURRENT_OPTION_VALUE = (CURRENT_OPTION_VALUE + 1);
		}
    }

    Menu_Refresh();
}

/** 
 * @brief 向下操作，对应E4
 * @note 和按钮热血沸腾的组合技
 * @return void
 */
void Menu_Down(void)
{
    if (current_mode == SELECT_MODE) {
        current_option_index = ((current_option_index + 1) + CURRENT_OPTION_COUNT) % CURRENT_OPTION_COUNT;
    } else if (current_mode == EDIT_MODE) {
		if (current_interface == MAIN_MENU && current_option_index == 0) {
			CURRENT_OPTION_VALUE = (CURRENT_OPTION_VALUE - 1 * value[OPTION_MENU][0]);
		} else if (CURRENT_OPTION_MODE == FLOAT_EDITABLE){
			CURRENT_OPTION_VALUE = (CURRENT_OPTION_VALUE - 0.01 * value[OPTION_MENU][0]);
		} else {
			CURRENT_OPTION_VALUE = (CURRENT_OPTION_VALUE - 1);
		}
    }

    Menu_Refresh();
}

/** 
 * @brief 确定，前进操作，对应E3
 * @note 和按钮热血沸腾的组合技
 * @return void
 */
void Menu_Forward(void)
{
    if (current_mode == SELECT_MODE) {
        if (CURRENT_OPTION_MODE == SUBINTERFACE) {
            // 转到下级菜单
            current_interface = interface[current_interface].subinterface[current_option_index];
            current_option_index = 0;
            OLED_Clear();
        } else if (CURRENT_OPTION_MODE == EDITABLE || CURRENT_OPTION_MODE == FLOAT_EDITABLE) {
            // 切换到编辑模式
            current_mode = EDIT_MODE;
        } else if (CURRENT_OPTION_MODE == INTERACTIBLE) {
            // 这一块写启动函数
            // Menu_Event(interface[current_interface].option_action[current_option_index]);
        }
    } else if (current_mode == EDIT_MODE) {
        // 切换回选择模式
        current_mode = SELECT_MODE;
    }

    Menu_Refresh();
}

/** 
 * @brief 取消，后退操作，对应E2
 * @note 和按钮热血沸腾的组合技
 * @return void
 */
void Menu_Backward(void)
{
    if (current_mode == SELECT_MODE) {
        // 返回上级菜单 //
        current_interface = interface[current_interface].super_interface;
		if (current_interface > 10) {
			current_interface = MAIN_MENU;
		}
        current_option_index = 0;
        OLED_Clear();
    } else if (current_mode == EDIT_MODE) {
        // 切换回选择模式
        current_mode = SELECT_MODE;
    }

    Menu_Refresh();
}
