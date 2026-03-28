#ifndef __MENU_H_
#define __MENU_H_

#define OLED_MAX_LINE                   (8)
#define CURRENT_INTERFACE      (interface[current_interface])
#define CURRENT_OPTION_COUNT   (CURRENT_INTERFACE.option_count)
#define CURRENT_OPTION_MODE    (CURRENT_INTERFACE.option_mode[current_option_index])
#define CURRENT_OPTION_VALUE   (value[current_interface][current_option_index])

typedef enum interface_id 
{
    MAIN_MENU,              // - 主菜单
    OPTION_MENU,            // - 设置

} interface_id;

/**
 * @brief 选项模式枚举
 * @note 用于判断当前选项的模式，以判断交互后的操作
 */
enum option_mode 
{
    NONE_MODE,      // - 无模式
    PURE_TEXT,      // - 纯文本模式
    SUBINTERFACE,   // - 子界面模式
    EDITABLE,       // - 可编辑模式
	FLOAT_EDITABLE,
    INTERACTIBLE,   // - 可交互模式
    READ_EEP,       // - 数据需要从eeproam读取
};

enum MENU_MODE
{
    EDIT_MODE,
    SELECT_MODE,
};

// EEPROM 地址分配
#define EEPROM_ADDR_INT16       0x00    // int16 存储地址（2字节）
#define EEPROM_ADDR_FLOAT       0x02    // floot 存储地址（4字节）

/* ==============================================================================================
                                        结构体定义
   ======================================================= ======================================= */
/**
 * @brief 界面结构体定义
 * @note 这个结构体定义了一个界面里应该有什么
 * @param ID interface_id
 * @param option_count 选项数量
 * @param option_text  选项文本
 * @param option_mode  选项模式
 * @param option_value EDITABLE
 * @param option_action INTERACTIBLE
 * @param subinterface SUBINTERFACE
 * @param value_range  EDITABLE范围限制
 * @param super_interface 父级界面索引
 */
typedef struct 
{
    unsigned char   ID;                                         // 菜单ID，1字节
    unsigned char   option_count;                               // 选项数量，1字节
    char            option_text[OLED_MAX_LINE][10];             // 选项文本，80字节
    unsigned char   option_mode[OLED_MAX_LINE];                 // 选项模式，8字节
    unsigned char   option_action[OLED_MAX_LINE];               // 交互动作，8字节
    unsigned char   subinterface[OLED_MAX_LINE];                // 子界面，8字节
    unsigned char   super_interface;                            // 父级界面，1字节
} Interface_TypeDef;

/* ==============================================================================================
                                        函数声明
   ============================================================================================== */

void                    Menu_Init                       (void);
void                    Menu_Refresh                    (void);
void                    Menu_Up                         (void);
void                    Menu_Down                       (void);
void                    Menu_Forward                    (void);
void                    Menu_Backward                   (void);
double                  Menu_GetValue                   (interface_id ID, int option_index);
void                    Menu_SetValue                   (interface_id ID, int option_index, int value);

#endif
