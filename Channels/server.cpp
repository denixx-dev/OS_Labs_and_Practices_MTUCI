#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
	mkfifo("pipe1", 0777);
	int arr [9] = {97, 96, 95, 94, 93, 92, 91, 90, 89};
	//char arr [9] = {"97", "96", "95", "94", "93", "92", "91", "90", "89"};
	//int res = sizeof(arr);
	//printf("%d\n", res);
	char buf[1024];
	
	int a = 97;
	int fd = open("pipe1", O_RDWR);
	write(fd, &arr, sizeof(arr));
	
	close(fd);
	return 0;
}
