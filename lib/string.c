#include "include/string.h"

void *memcpy(void *dst,const void *src,size_t count)
{
	void *ret = dst;
	while (count--)
	{
		*(char*)dst++ = *(char*)src++;
	}
	return ret;
}


char *strcpy(char *dst,const char *src)
{
	void *ret = dst;
	while (*src != '\0')
	{
		*dst++ = *src++;
	}
	*(dst + 1) = '\0';
	return ret;
}



char *strcat(char *dst,const char *src)
{
	void *ret = dst;
	while (*dst != '\0') dst++;
	while ((*dst++ = *src++) != '\0');
	*((char*)dst + 1) = '\0';
}
