#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>


#define MYLOG_MSG __LINE__, __FILE__, __FUNCTION__


void mylog(int line, const char* file, const char* fun, char* format, ...)
{
	time_t tm;
	va_list vaptr;

	char out_timer[32];
	char fmtString[256];
	char out_str[1024];

	memset(out_timer, 0, sizeof(out_timer));
	memset(fmtString, 0, sizeof(fmtString));
	memset(out_str, 0, sizeof(out_str));

	tm = time(NULL);
	strftime(out_timer, 32, "%Y-%m-%d %I:%M:%S", localtime(&tm));

	va_start(vaptr, format);
	vsprintf(fmtString, format, vaptr);

	snprintf(out_str, sizeof(out_str), "%s [%s=>%s:%d]:%s", out_timer, 
			file, 
			fun, 
			line, 
			fmtString);
	
	va_end(vaptr);
	printf("%s\n", out_str);

}


int main(void)
{
	int a = 10;
	mylog(MYLOG_MSG, "this is %d", a);
	return 0;
}
