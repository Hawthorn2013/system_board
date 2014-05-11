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
��bug��RAM��д��ʽPIT�жϲ��ܴ�����FLASH��д��ʽPIT������

20140118
��Ӵ���0�����ڿ���android��λ����������Ĵ����쳣�ӵ�����������ձ귴�ˡ�

20140220
���spi

system_board_0221_1446
spiͨ������ʹ�ã����ֽڷ��ͣ���û�����С�
�˳���Ϊ�������ӻ�����system_board_0221_1446_slave

system_board_0222_1400
spi���Ե��ֽڷ��ͣ���һ�汾δʵ�ֵ��ֽڷ��ͣ���˫�ֽڷ��͡�
���sd����д��δ���ԡ�
�˳���Ϊ�������ӻ�����system_board_0222_1400_slave

system_board_0225_1529
����Ƭѡ�����⡣��ʼ��ʱ��ҪƬѡ���߸�����74��ʱ�ӣ���spi���ڷ��͵�ʱ���Զ�����Ƭѡ��ʹ��io�ڴ���ͺ��ˡ��������������spi��Ƭѡ���ܡ��޸�ing������
�Ļ���spi�Դ���Ƭѡ���ܡ�sd����/д�������ˡ�

system_board_0228_2252
��Ӷ���/д���ܣ��ܾ�����ϸ�ڣ����緵��ֵ��δ�����ԡ�

system_board_0301_1825
������������д��
�Ƴ�MBRд������
д�������ɹ���
�������δʵ�֣�����CMD18ʧ�ܣ�ԭ������
����д�������ܳɹ�������顣
��������д�����ˡ�������д�����CMD12.����Ӱ��֮��Ĳ�����

system_board_0301_1932
������һ�³���ɾ��һЩע�͡�
��sd��������10M��20M�����

system_board_0301_2202
��ʼ��ֲfatfs������˶�/д�����������ļ���δ�ҵ���С��ѡ�δ�����κβ��ԡ�
׼����BYTE����uint8_t���ִ��롣
���ǵĴ�����ϵ���������⣬��Ӧ�ý����еĺ����ͺ궨�嶼������.h�С�׼�����������������Դ�

system_board_0304_2010
�޸���������������ʹ�ú궨�嶨��BYTE��WORD��DWORD��
��sd.c�еĴ����ڲ�������Ϊ��̬����ֹ�ⲿ���ʡ����³�bug������

system_board_0304_2316
�������ԣ���������д���������ڵ�������
��i/o�е�/�������ֿ�����ʹ��const�޶�д��������
�ɹ�ʵ�ֹ������½��ļ�������ԭ��������С���岻һ�¡��ļ���8.3��

system_board_0309_2139
ͨ������ǿ������ת��ȥ�����档
����д�������ļ�ͨ��FA_CREATE_ALWAYS�����󣬲���д��Ҫ�رպ��ٴ򿪡�

system_board_0317_1706
I2C��������������ʱ�ӡ���������Ӧ������⡣�Ȱѽ��ȱ���������תȥ����ʱ�ӡ�

system_board_0318_2215
������д�Ĵ��������ε�����ģ�飬�ر�������Ĵ��ڣ�����ʾ������Ȼ����д�������л�һ�鵥Ƭ����һ�£��������ٻ�������
I2Cͨ���ش�ͻ�ƣ����Զ��Ĵ�������һ��������֮�����Ļ��0xff���д����ԡ�
����ʹ�õ�Ƭ���ϵ�5V��Դ����ʹ��Ƭ�������쳣���жϡ�

system_board_0327_2303
I2Cͨ�ųɹ�����д������֮ǰֻ�ܶ�һ�Σ��������һ�ֽ�ȫΪ0xff��Ϊû����Ӧ��λ��
д���ֽ�û���⣬�����ֽ�δ�������������˼·�Ƕ��������ڶ�����Ҫ��

system_board_0405_1507
��OLED���룬DSPI_2�ĳ�DSPI_1����������OLED��TF������������

system_board_0405_2349
�����λ�����Σ���ʵ�֣�δ�ع���
