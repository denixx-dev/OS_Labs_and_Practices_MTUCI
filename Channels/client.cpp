#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(){

	int fd = open("pipe1", O_RDWR);
	
	printf("Client started working\n");
	
	
	//close(fd);
	//int fd = open("pipe1", O_RDONLY);
	
	int buf[9];
	
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
	//Считываем массив данных и выводим об этом запись
	read(fd, &arr, sizeof(arr)*9);	
	printf("Received array\n");
	//Вывод массива клиентом
	/*for (int i =0; i<9; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");*/
	
	//Создаем флаг plus со значением 1 для того, чтобы передать его через канал серверу
	//А тот в свою очередь прибавит этот флаг в свою переменную и отошлет соответствующий массив конкретному клиенту
	int plus = 1;
	int res;
	write(fd, &plus, sizeof(plus));
	cout<<"Flag sent";
	read(fd, &res, sizeof(res));
	
	cout<<"Num from server: "<<res;
	
	
	close(fd);
	
	
	
	return 0;
}
