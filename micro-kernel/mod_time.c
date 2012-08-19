/*
 * mod_time.c
 *
 *  Created on: 2012-8-19
 *      Author: greenleaf
 */

#define SetTimeInfo(value,len)	\
	dbcd_to_str(value,(char*)&date_info[date_pos]);	\
	date_pos += len

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

	SetTimeInfo(cmos_info.DateCentury,2);	// ����
	SetTimeInfo(cmos_info.Year,2);			// ��
	date_info[date_pos++] = '-';
	SetTimeInfo(cmos_info.Month,2);			// ��
	date_info[date_pos++] = '-';
	SetTimeInfo(cmos_info.DayOfMonth,2);	// ��
	memcpy((void*)&date_info[date_pos],(void*)&day_of_week_table[cmos_info.DayOfWeek][0],11);	// ����
	date_pos += 11;

	SetTimeInfo(cmos_info.Hours,2);			// ʱ
	date_info[date_pos++] = ':';

	SetTimeInfo(cmos_info.Minutes,2);		// ��
	date_info[date_pos++] = ':';

	SetTimeInfo(cmos_info.Seconds,2);		// ��
	date_info[date_pos++] = '\n';

	print_pos((char*)&date_info,24*160 + 90);
}


void mod_time_read_cmos()
{
	for(int i=0;i<10;i++)
	{
		outb(0x71,i);
		for(int j=0;j<10;j++);
		*(((u8*)&cmos_info) + i) = inb(0x71);
	}
}



void mod_time_do(MsgHead msg)
{
	// if(msg.vector == MSG_INTR_CLOCK)
	{
//		char ch = 't';
//		print(&ch);
		mod_time_read_cmos();
		mod_time_show();
	}
/*	char *ch = "t";
	itoa(msg.vector,ch);
	print(&ch); */
	return;
}


id_t mod_time_id;
void mod_time_main()
{
	msg_reg.param = MSG_INTR_CLOCK;
	msg_reg.sender = mod_time_id;
	post(msg_reg);
	mod_time_show();
	while(1)
	{
		MsgHead msg = recv();		// ��ȡ��Ϣ
		mod_time_do(msg);			// ������Ϣ
		//for(int i=0;i<1000000;i++);
		char *ch = "T";
		print(ch);
	}
}
