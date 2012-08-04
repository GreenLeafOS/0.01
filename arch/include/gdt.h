#ifndef GDT_H_
#define GDT_H_
/*===============================================================
 *                       gdt.h
 *                	  GDT��ش�����
 ===============================================================*/
#include "desc.h"
#include <Type.h>

/* ��ʼ�� */
void gdt_init();

/* gdt���� */
void gdt_add(Desc item);
void gdt_remove(int index);
void gdt_set(int index,Desc item);
void gdt_get(int index,Desc *item);

/* ������������gdt�Ĳ������ǶԻ�����в����� */
void gdt_save();
void gdt_ref();

extern Desc gdt_table[32][2];
extern u8 gdt_tmp;
extern u8 gdt_ptr[6];

#endif /* GDT_H_ */
