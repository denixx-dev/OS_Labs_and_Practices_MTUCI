#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
	int fd = open("pipe1", O_RDONLY);
	
	int arr[9];
	/*while(1){
		int len = read(fd, buf, sizeof(buf)-1);
		if (len > 0){
			buf[len] =0;
			printf("array values: %d\n", buf);
		}
		else if (len == 0){
			printf("server quit\n");
			break;
		}
		else{
			perror("read");
			return 1;
		}
	
	}*/
	
	read(fd, &arr, sizeof(arr)*9);	
	printf("Received array\n");
	for (int i =0; i<9; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");
	close(fd);
	return 0;
}
