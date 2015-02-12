#ifndef __ENCODED_H__
#define __ENCODED_H__

//#include "gb2312_unicode_table.h"
//#include "unicode_gb2312_table.h"

extern void Utf8ToGb2312(unsigned const char* utf8, int len, unsigned char *temp);

extern void Gb2312ToUft8(unsigned const char* gb, int len, unsigned char* tmp);

#endif //__ENCODED_H__
