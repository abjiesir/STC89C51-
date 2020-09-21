#include<reg51.h>

/*交通灯的使用描述：
首先是东西方向的交通灯闪烁，表示车辆可以正常通行，闪烁30秒后开始常亮，表示通行时间已经过去，然后黄灯闪烁5秒，表示一个警告时间，之后东西方向的红灯依然是常亮，表示禁止通行
黄灯闪烁5秒后，南北方向的红灯开始闪烁，表示南北方向正常通行，然后闪烁30秒后，开始常亮，黄灯开始闪烁，闪烁5秒，之后南北方向红灯常亮，表示南北方向禁止通行。如此反复

//数码管显示的数字0-9
char table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
char table1[]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef};

//用来控制流水灯的各个参数
int red1_sec,red1_sec1,red2_sec,red2_sec1,yello_sec1,yello_sec2;
int red1_flag=0,red2_flag=0,yello1_flag=0,yello2_flag=0;

//控制交通方向的红灯和黄灯
#define led0(x) P1=(x==0?P1&(~(0x1<<0)):P1|0x1<<0)//东西方向红灯
#define led1(x) P1=(x==0?P1&(~(0x1<<1)):P1|0x1<<1)//警告用的黄灯
#define led4(x) P1=(x==0?P1&(~(0x1<<4)):P1|0x1<<4)//南北方向红灯

//初始化定时器的参数和定时器中断配置
void init(){
	//EX0=1;IT0=1;
	ET0=1;
	ET1=1;
	TMOD=0x11;
	TH1=0x4b;
	TL1=0xfd;
	TH0=0x4b;
	TL0=0xfd;
	//TR1=1;
	TR0=1;
	EA=1;
	
}

//串口中断配置，本程序没有用到串口中断
void uart_init(){
	TR1=1;
	TMOD=0x20;
	TH1=0xfd;
	TL1=0xfd;
	SM0=0;SM1=1;
	ES=1;
	REN=1;
	EA=1;
}


//主函数，通过while循环延时，等待定时中断的发声
void main(){
	init();
	led4(0);
	while(1);
}

//定时器0的中断函数
void TIME0()  interrupt 1
{
	TH0=0x4b;
	TL0=0xfd;
	red1_sec++;
	if(red1_sec==20){
		led0(red1_flag);
		red1_flag=!red1_flag;
		red1_sec=0;
		yello_sec1++;
		if(yello_sec1>30){
			red1_flag=0;
			led1(yello1_flag);
			yello1_flag=!yello1_flag;
		}
		if(yello_sec1>35){
			red1_flag=0;
			led1(1);
			TR0=0;
			TR1=1;
			yello_sec1=0;
		}
	}
}


//定时器1的中断函数
void TIME1()  interrupt 3
{
	TH1=0x4b;
	TL1=0xfd;
	red2_sec++;
	if(red2_sec==20){
		red2_sec=0;
		led4(red2_flag);
		red2_flag=!red2_flag;
		yello_sec2++;
		if(yello_sec2>30){
			red2_flag=0;
			led1(yello2_flag);
			yello2_flag=!yello2_flag;
		}
		if(yello_sec2>35){
			red2_flag=0;
			led1(1);
			TR0=1;
			TR1=0;
			yello_sec2=0;
		}
	}
}
