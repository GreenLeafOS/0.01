#ifndef GDT_H_
#define GDT_H_
/*===============================================================
 *                       gdt.h
 *                	  GDT相关处理函数
 ===============================================================*/
#include "desc.h"
#include <Type.h>

/* 初始化 */
void gdt_init();

/* gdt操作 */
void gdt_add(Desc item);
void gdt_remove(int index);
void gdt_set(int index,Desc item);
void gdt_get(int index,Desc *item);

/* 缓冲操作（针对gdt的操作都是对缓冲进行操作） */
void gdt_save();
void gdt_ref();

extern Desc gdt_table[32][2];
extern u8 gdt_tmp;
extern u8 gdt_ptr[6];

#endif /* GDT_H_ */
