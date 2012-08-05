/*===============================================================
 *                      kernel.c
 *                		ÄÚºËÖ÷º¯Êý
 ===============================================================*/
#include "print.h"

int disp_pos = 0;

void KernelMain()
{
	char *str = "\nIn Kernel now!";
	print(str);
	while(1);
}
