/*
 * mod_time.c
 *
 *  Created on: 2012-8-19
 *      Author: greenleaf
 */

#define SetTimeInfo(value,len)	\
	dbcd_to_str(value,(char*)&date_info[date_pos]);	\
	date_pos += len

/* PC输入频率 */
#define TIMER_FREQ     1193182L
/* 输出频率 */
#define HZ             100	// 10ms级

#include "include/module.h"
#include "include/sysapi.h"




char day_of_week_table[7][12] = {
		"  Sunday    ",
		"  Monday    ",
		"  Tuesday   ",
		"  Wednesday ",
		"  Thursday  ",
		"  Friday    ",
		"  Saturday  "};


void mod_time_show()
{
	char date_info[50] = "Date: ";
	int date_pos = 6;

	SetTimeInfo(cmos_info.DateCentury,2);	// 世纪
	SetTimeInfo(cmos_info.Year,2);			// 年
	date_info[date_pos++] = '-';
	SetTimeInfo(cmos_info.Month,2);			// 月
	date_info[date_pos++] = '-';
	SetTimeInfo(cmos_info.DayOfMonth,2);	// 日
	memcpy((void*)&date_info[date_pos],(void*)&day_of_week_table[cmos_info.DayOfWeek][0],11);	// 星期
	date_pos += 11;

	SetTimeInfo(cmos_info.Hours,2);			// 时
	date_info[date_pos++] = ':';

	SetTimeInfo(cmos_info.Minutes,2);		// 分
	date_info[date_pos++] = ':';

	SetTimeInfo(cmos_info.Seconds,2);		// 秒
	date_info[date_pos++] = '\n';

	print_pos((char*)&date_info,24*160 + 90);
}


void mod_time_read_cmos()
{
	for(int i=0;i<10;i++)
	{
		outb(0x70,i);
		*(((u8*)&cmos_info) + i) = inb(0x71);

	}
}



void mod_time_do(MsgHead msg)
{
	if(msg.vector == MSG_INTR_CLOCK)
	{
		mod_time_read_cmos();
		mod_time_show();
	}
	return;
}


id_t mod_time_id;
void mod_time_main()
{
	/* 注册时钟中断 */
	msg_reg.param = MSG_INTR_CLOCK;
	msg_reg.sender = mod_time_id;
	post(msg_reg);

	/* 初始化8253 */
	outb(TIMER_MODE,RATE_GENERATOR);
	outb(TIMER0, (u8)(TIMER_FREQ/HZ));
	outb(TIMER0, (u8)((TIMER_FREQ/HZ) >> 8));

	mod_time_show();
	while(1)
	{
		MsgHead msg = recv();		// 获取消息
		mod_time_do(msg);			// 处理消息
	}
}
