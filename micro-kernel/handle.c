/*
 * handle.c
 *
 *  Created on: 2012-8-12
 *      Author: greenleaf
 */


#include "include/kernel.h"



/*
 * 初始化中断异常
 * */
void idt_kernel_init()
{

}



/*
 * 中断处理函数
 * 参数：
 * 		irq_num	中断号
 * 返回值：对应restart的地址
 * */
FunAddr intr_handle(int irq_num)
{
	return restart;
}


/*
 * 异常处理函数
 * 参数：
 * 		vec_num 异常向量号
 * 		err_code 错误码
 * 返回值：对应restart的地址
 * */
FunAddr exception_handler(int vec_num, int err_code)
{
	return restart;
}
