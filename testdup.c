#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{
	int fd = open("jj", O_RDWR | O_TRUNC);
	int tmp = dup(1);
	int tst = 10;
	dup2(fd, 1);
	close(fd);
	printf("write in file jj %d\n", tst);
	
	write(1, "before\n", 7);
	sleep(5);
	dup2(tmp, 1);
	tst = 20;
	printf("write in stdout %d\n", tst);
	write(1, "after\n", 6);
	return 0;
}