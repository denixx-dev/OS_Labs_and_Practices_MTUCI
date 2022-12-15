#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main(){
	mkfifo("pipe1", 0666);
	
	int fd= open("pipe1", O_RDWR);
	
	int cpids[3];
	
	//Массив для хранения чисел
	int arr[9];
	
	//Считывание чисел из файла и запись их в массив
	printf("Sending array blocks to clients...\n");
	FILE* file = fopen("numbers.txt", "r");   
    for (int i = 0; i < 9; i++) {
    	fscanf(file, "%d ", &arr[i]);
    }
    fclose(file);
    /*for (int i = 0; i < 9; i++) {
    	printf("%d; ", arr[i]);
    }*/
    
   	
    
    int buf;
    
    int count =0;

    /*int x = arr[0];
    int y = arr[1];
    int z = arr[2];*/
    	
    //write(fd, &arr, sizeof(arr)*9);
    //В бесконечном вайле делаем опрос всех клиентов
    while(1){
		read(fd, &buf, sizeof(buf));
		//Пишем, что получили флаг от клиента
		printf("Flag received");
		//Записали одну из частей массива (ну тут одно значением из массива, и будет типа 1-й, 2-й, 3-й в зависимости от переменной count, но пока не суть)
		write(fd, &arr[count], sizeof(arr[count]));
		cout<<"Num written";
		//Прибавляем полученную единицу из канала к переменной count
		count += buf;
	}
    
	//count += 3;    	
    close(fd);
	printf("Blocks sent\n");
	return 0;
}
