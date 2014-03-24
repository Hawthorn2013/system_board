//------------------------------------------------------------------------
//  Readme.txt
//------------------------------------------------------------------------
This project stationery is designed to get you up and running
quickly with CodeWarrior on your MPC5604B_M27V target.

The wizard has created the following CodeWarrior target(s):


The targets are set up for the selected CPU and options,
but can be easily modified.

          *************
          * IMPORTANT *
          *************

If your application will run from flash memory, you should
initialize some additional registers to improve system
performance. For MPC551x devices, initialize the PFCRP0 and 
PFCRP1 registers; for other devices, initialize the BIUCR.

Please review the MPC5604B_M27V reference manual to determine 
appropriate values.

Sample code for the following language(s) is at your disposal:


Other options may be available:

//------------------------------------------------------------------------
//  Getting Started
//------------------------------------------------------------------------
To build  your project, use the menu Project > Make or press F7.
This will build the selected target.

//------------------------------------------------------------------------
// Project structure
//------------------------------------------------------------------------
The project generated contains various files/groups:
- readme.txt: this file
- Sources: folder with the application source code
- Header Files: all application and runtime header files
- Startup Code: user customizable startup code 
- Lcf Files: the linker command files for the different build targets.
- Runtime: the libraries

//------------------------------------------------------------------------
//  Adding your own code
//------------------------------------------------------------------------
Once everything is working as expected, you can begin adding your own code
to the project. Keep in mind that we provide this as an example of how to
get up and running quickly with CodeWarrior. There are certainly other
ways to handle interrupts and set up your linker command file. Feel free
to modify any of the source files provided.

//------------------------------------------------------------------------
//  Additional documentation
//------------------------------------------------------------------------
Read the online documentation provided. From within the CodeWarrior IDE,
select Help > CodeWarrior Help.

//------------------------------------------------------------------------
//  Contacting Freescale
//------------------------------------------------------------------------
For bug reports, technical questions, and suggestions, please use the
forms installed in the Release_Notes folder and send them to:

cw_support@freescale.com


20131104
大bug：RAM烧写方式PIT中断不能触发，FLASH烧写方式PIT正常。

20140118
添加串口0，用于开发android上位机。龙丘买的串口异常坑爹，发送与接收标反了。

20140220
添加spi

system_board_0221_1446
spi通信已能使用，单字节发送，还没做队列。
此程序为主机，从机程序：system_board_0221_1446_slave

system_board_0222_1400
spi调试单字节发送，上一版本未实现单字节发送，是双字节发送。
添加sd卡读写，未测试。
此程序为主机，从机程序：system_board_0222_1400_slave

system_board_0225_1529
真是片选的问题。初始化时需要片选拉高给至少74个时钟，但spi会在发送的时候自动拉低片选。使用io口代替就好了。不过我想继续用spi的片选功能。修改ing。。。
改回了spi自带的片选功能。sd卡读/写都可以了。

system_board_0228_2252
添加多块读/写功能，很纠结于细节，比如返回值。未经测试。

system_board_0301_1825
修正多扇区读写。
移出MBR写保护。
写多扇区成功。
读多个块未实现，发送CMD18失败，原因不明。
不先写多个块就能成功读多个块。
多扇区读写调成了。不能在写中添加CMD12.否则影响之后的操作。
