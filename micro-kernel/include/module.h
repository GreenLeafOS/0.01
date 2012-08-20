/*
 * module.h
 *
 *  Created on: 2012-8-19
 *      Author: greenleaf
 */

#ifndef MODULE_H_
#define MODULE_H_

#include "lib.h"


/* mod_pubmsg.c */
void mod_public_msg_main();
id_t mod_public_msg_id;



/* mod_time.c */
typedef struct time
{
	u16	year;
	u8	month;
	u8	day_of_month;
	u8	day_of_week;
	u8	hours;
	u8	minutes;
	u8	seconds;
}Time;

void mod_time_main();
id_t mod_time_id;
Time mod_time_sysclock;



/* mod_handle.c */
void mod_handle_main();
id_t mod_time_id;
#endif /* MODULE_H_ */
