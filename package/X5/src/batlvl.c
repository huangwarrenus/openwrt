/*
 *  Poray X5 and X6 battery level monitor
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 * -- Luis Soltero <lsoltero@globalmarinenet.com> --
 */



#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>

#define BLINK 250000
#define LEDBASE "/sys/class/leds/x5:green:"
#define LEDTAIL "/brightness"
#define BATLEV  "/proc/driver/cs1168"

char *leds[] = {
	LEDBASE "20" LEDTAIL,
	LEDBASE "50" LEDTAIL,
	LEDBASE "80" LEDTAIL,
	NULL,
};

//#define BATFULL  0xcf
#define BATFULL  0xc5
#define BATEMPTY 0xaa

void ledwrite(int on, int n)
{
    char *val = on ? "1" : "0";
    int i;
    for (i = 0; i < n && leds[i]; i++ ) {

        int fd = open(leds[i], O_WRONLY);
        write(fd, val, strlen(val));
        close(fd);
//printf("%d - %s - %c\n", fd, leds[i], *val);

    }
}

void ledzero()
{
    int i;
    for ( i = 0; i < 10; i++ ) {
       char *val = "1";
       int fd = open(leds[0], O_WRONLY);
       write(fd, val, strlen(val));
       close(fd);
//printf("%d - %s - %c\n", fd, leds[0], *val);
       usleep(BLINK);
 
       val = "0";
       fd = open(leds[0], O_WRONLY);
       write(fd, val, strlen(val));
       close(fd);
//printf("%d - %s - %c\n", fd, leds[0], *val);
       usleep(BLINK);
    }
}


int main(int argc, char **argv)
{
    int fd = open(BATLEV, O_RDONLY);
    if ( fd == -1 ) return 1;


    char level[3];
    bzero(level, sizeof(level));
    read(fd, &level, 2);
    close (fd);
    
    int volt;
    sscanf(level, "%x", &volt);

    int range = (BATFULL - BATEMPTY) / 4;
    int val = volt - BATEMPTY;
//val = 8;
    int n = (int)(val / range);
//printf("Batt level = %d-%d-%d, range = %d, n = %d\n", volt, val, BATFULL-BATEMPTY, range, n);

    if ( n ) {
        ledwrite(1, n);
        sleep(5);
        ledwrite(0, n);
    } else {
        ledzero();
    }

    return 0;
}
