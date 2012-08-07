/*===============================================================
 *                      kernel.c
 *                		ÄÚºËÖ÷º¯Êý
 ===============================================================*/
#include "print.h"

int disp_pos = 0;

void KernelMain()
{
	char *str = "In Kernel now!\n";
	print(str);
	while(1);
}
