#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(){
	mkfifo("pipe1", 0666);
	//Массив для хранения чисел
	int arr[9];
	
	//Считывание чисел из файла и запись их в массив
	printf("Sending array blocks to clients...\n");
	FILE* file = fopen("numbers.txt", "r");   
    for (int i = 0; i < 9; i++) {
    	fscanf(file, "%d, ", &arr[i]);
    }
    /*for (int i = 0; i < 9; i++) {
    	printf("%d; ", arr[i]);
    }*/
    
   	int fd= open("pipe1", O_WRONLY);
    
    int buf[3];
    
    int count =2;

    
    	
    write(fd, &arr, sizeof(arr)*9);
    
	count += 3;    	
    close(fd);
	printf("Blocks sent\n");
	return 0;
}
