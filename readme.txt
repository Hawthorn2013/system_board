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

system_board_0301_1932
整理了一下程序，删掉一些注释。
将sd卡提升至10M。20M会出错。

system_board_0301_2202
开始移植fatfs。完成了读/写函数。配置文件中未找到大小端选项。未进行任何测试。
准备用BYTE代替uint8_t这种代码。
我们的代码体系可能有问题，不应该将所有的函数和宏定义都包含在.h中。准备改正。。工程量略大。

system_board_0304_2010
修改数据类型声明，使用宏定义定义BYTE、WORD、DWORD。
将sd.c中的大量内部函数设为静态，防止外部访问。真怕出bug。。。

system_board_0304_2316
经过测试，多扇区读写函数适用于单扇区。
将i/o中单/多扇区分开处理。使用const限定写缓冲区。
成功实现挂载与新建文件。出错原因：扇区大小定义不一致。文件名8.3。

system_board_0309_2139
通过宏与强制类型转换去除警告。
测试写函数。文件通过FA_CREATE_ALWAYS创建后，不能写，要关闭后再打开。

system_board_0317_1706
I2C不正常。怀疑是时钟、和主接收应答的问题。先把进度保存起来，转去调试时钟。

system_board_0318_2215
不能烧写的处理方法。拔掉所有模块，特别是龙丘的串口，包括示波器。然后烧写。还不行换一块单片机烧一下，能烧了再换回来。
I2C通信重大突破，可以读寄存器。第一次正常。之后后面的会出0xff，有待调试。
请勿使用单片机上的5V电源，会使单片机进入异常的中断。

system_board_0327_2303
I2C通信成功，读写正常。之前只能读一次，后面除第一字节全为0xff因为没设置应答位。
写单字节没问题，读单字节未测试清楚，整体思路是读两个，第二个不要。

system_board_0405_1507
将OLED加入，DSPI_2改成DSPI_1，经过测试OLED与TF卡正常工作。

system_board_0405_2349
添加下位机调参，已实现，未重构。

system_board_0406_1245
修改通信协议：AA BB 01 00 04 00 01 00 00 04 使其可以寻址。上位机版本v1.6.舵机调参已实现。
大量修改了管脚，特别是超声，占了8路ch。
