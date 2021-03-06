#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/utsname.h>
#include <linux/kernel.h>
#include <sys/sysinfo.h>
#define _POSIX_C_SOURCE 200809L
#define chunk 1024

char *substring(char *string, int position, int length) {
	char *pointer;
	int c;

	pointer = malloc(length+1);

	if (pointer == NULL) {
		printf("Unable to allocate memory.\n");
		exit(1);
	}

	for (c = 0 ; c < length ; c++) {
		*(pointer+c) = *(string+position-1);
		string++;
	}

	*(pointer+c) = '\0';

	return pointer;
}

/*
char *nlnrem(char *str) {
	if (str == NULL) {
		exit(1);
	}
	int length = strlen(str);
	int pos[length+1];
	if (str[pos-1] == '\n') {
		str[pos-1]  = '\0';
		pos = pos-1;
	}
	return str;
}
*/
char *fileopen(char *file) {
        FILE *filePointer ;
        char dataToBeRead[5002];
        filePointer = fopen( file, "r") ;
        fseek(filePointer, 0L, SEEK_END);
        long int res = ftell(filePointer);
        char *result = calloc(5000, 5000);

        if ( filePointer == NULL ) {
                printf( "%s file failed to open.", file ) ;
        }
        else {
                fseek(filePointer, 0, SEEK_SET);
                while( fgets ( dataToBeRead, 50, filePointer ) != NULL ) {
                strcat(result, dataToBeRead );
                }

                fclose(filePointer);
        }
	return result;
}

int os() {
        struct utsname utbuffer;
        errno = 0;


        if (uname(&utbuffer) != 0) {
                perror("uname");
                exit(EXIT_FAILURE);
        }

        char *result =  fileopen("/usr/lib/os-release");
        char *out = (result+ strlen(result)- 251);
        char *os[50];
        strncpy((void *) os, out, 11);
        char *architecture = (utbuffer.machine);
        printf("\e[36;1m OS\e[m:  %.20s\n", strcat((strcat((void *)os, " ")), architecture));
}

int model() {
        char *nameold = fileopen("/sys/devices/virtual/dmi/id/product_name");
	char *name[strlen(nameold)];
	strncpy((void *) name, nameold, 14);
	char *versionold = fileopen("/sys/devices/virtual/dmi/id/product_version");
        char *version[strlen(versionold)];
	strncpy((void *) version, versionold, 45);
	printf("\e[36;1m Host\e[m: %s\n", strcat(strcat((void *) name, " "), (void *) version));
}

int Kernel() {
        struct utsname utbuffer;
        errno = 0;


        if (uname(&utbuffer) != 0) {
                perror("uname");
                exit(EXIT_FAILURE);
        }

	printf("\e[36;1m Kernel\e[m: %s\n", utbuffer.release);
}

int uptime() {
	struct sysinfo s_info;
	int error = sysinfo(&s_info);
	if(error != 0) {
		printf("code error = %d\n", error);
	}
	printf("\e[36;1m Uptime\e[m: %d %s\n", (s_info.uptime / 60), "mins");
}

int packages() {
	DIR *d;
	struct dirent *dir;
	d = opendir(".");

	if (d) {
		while ((dir = readdir(d)) != NULL) {
			printf("%s\n", dir->d_name);
		}
		closedir(d);
	}
	return(0);
}

int main(void) {
/*	printf("system name = %s\n", utbuffer.sysname);
	printf("node name= %s\n", utbuffer.nodename);
	printf("release = %s\n", utbuffer.release);
	printf("version = %s\n", utbuffer.version);
	printf("\n\n\n\n\n\n\n");
*/
	os();
	model();
	Kernel();
	uptime();
	return EXIT_SUCCESS;

}

