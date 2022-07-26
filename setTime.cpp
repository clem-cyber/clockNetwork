#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sys/time.h>

using namespace std;

int main (int argc, char * argv[])
{
	if (argc < 6)
		exit(EXIT_FAILURE);
	struct tm tm;
	time_t timer;
	memset(&tm, 0, sizeof(tm));
	memset(&timer, 0, sizeof(timer));
	tm.tm_year = atoi(argv[1])- 1900;
        tm.tm_mon = atoi(argv[2]) - 1;
        tm.tm_mday = atoi(argv[3]);
        tm.tm_hour = atoi(argv[4]) - 1;
        tm.tm_min = atoi(argv[5]);
        //tm.tm_sec = atoi(argv[6]);
        //tm.tm_wday = atoi(argv[7]);
        
        struct timeval tv;
        timer = mktime(&tm);
        tv.tv_sec = timer;
        tv.tv_usec = 0;
        settimeofday(&tv, NULL);
	
	return 0;
}
