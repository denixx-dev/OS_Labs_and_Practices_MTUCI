#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
	char read_arr[9];
	
	int fd = open("pipe1", O_RDONLY);
	
	char buf[1024];
	for (int i=0; i++; i<9){
		read_arr[i] = read(fd, buf, 36); 
	}
	
	for (int i=0; i++; i<9){
		printf("%d\n", read_arr[i]);
	}
	printf("%d\n", read_arr[8]);
	close(fd);
	return 0;
}
