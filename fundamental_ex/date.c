
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

/* Write a version of date command. */
/*
 * date: Sun Aug 30 01:07:59 +07 2020
 *       "+%a %b %e %H:%M:%S %Z %Y"
 *   -u: Convert time to UTC0
 *
 *
 * */
char * get_year(struct tm * t, char * result, int n) {
    snprintf(result, n, "%d", t->tm_year + 1900);
    return 0;
}

int get_tz(struct tm * t, char * result, int n) {
    snprintf(result, n, "%+03li", t->tm_gmtoff / 3600);

    /* Below is the difficult way of doing this. */
/*    memmove(result + 1, result, strlen(result) + 1);
  *  
  * if (t->tm_gmtoff >= 0) {
  *      memcpy(result, "+", 1);
  *  }else {
  *      memcpy(result, "-", 1);
  *  }
*/
    return 0;
}

int main() {
    time_t rawtime;
    struct tm * timeinfo;
    char year[5]; 
    char tz[4];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    get_year(timeinfo, year, 5);
    get_tz(timeinfo, tz, 4);

    printf("%s %s \n", tz, year);

    return 0;
}


