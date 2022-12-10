#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

void Child(int n){

	pid_t pidf, pidp;
	
	if ((pidf = fork()) == 0){
		pidf = getpid();
		pidp = getppid();
		printf ("%d.Это Дочерний процесс его pid=%d\n", n, pidf);
		printf ("%d.А pid его Родительского процесса=%d\n", n, pidp);
		
		int fd_c = open("pipe1", O_RDONLY);
		/*int buf[1024];
		while(1){
			int len = read(fd_c, buf, sizeof(buf)-1);
			if (len>0){
				buf[len] = 0;
				printf("result %c", buf);
			}
		}*/
		int res[3];
		read(fd_c, res, sizeof(res));
		printf("%d ", res);
		close(fd_c);
	}
}

int main(){
	mkfifo("pipe1", 0666);
	//Массив для хранения чисел
	int arr[9];
	
	//Считывание чисел из файла и запись их в массив
	FILE* file = fopen("numbers.txt", "r");   
    for (int i = 0; i < 9; i++) {
    	fscanf(file, "%d, ", &arr[i]);
    }
    /*for (int i = 0; i < 9; i++) {
    	printf("%d; ", arr[i]);
    }*/
    
    for (int i =1; i <= 3; i++){
    	int fd_p = open("pipe1", O_WRONLY);
    	switch (i){
    		case 1:
    			//int buf[3];
    			for (int j = 0; j<3; j++){
    				 write(fd_p, &arr[j], 4);
    			}
    		case 2:
    			for (int j = 3; j<6; j++){
    				 write(fd_p, &arr[j], 4);
    			}
    		case 3:
    			for (int j = 6; j<8; j++){
    				 write(fd_p, &arr[j], 4);
    			}
    	}
		Child(i);
	}
	close(fd_p);

}
