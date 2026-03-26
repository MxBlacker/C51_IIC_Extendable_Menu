Welcome to my blog mxblacker.github.io

# C51_IIC_Extendable_Menu
A C51 extendable menu module based on Inter-Integrated Circuit (AKA IIC). Huge shout out to some random guys on CSDN lol.

# 简介
这个菜单框架是我原来在STM32上写的，显然C51达不到STM32那么快的速度，也没有它那么快的速度，所以我做了一些修改，并对内存进行了一些优化，其中包含：
- 对内部RAM和外部RAM的合理分配（即 _data_、_xdata_、_code_等保留字的使用）
- 由于在STM32上写的时候没太在意内存，C51区区 _128字节_ 的RAM着实难到了我，所以特地修改了菜单结构体的成员的定义顺序，即利用【结构体对齐】进行空间的压缩。

C51的运行速度慢的可怜，擦个屏幕都要老半天，我暂时不知道怎么加快这个通讯速度。目前花了一点时间搞了IIC的通信，之后有时间可能会写一个SPI的。

# WIP
考核任务还有一个eeprom的存储功能，明天再搞。

