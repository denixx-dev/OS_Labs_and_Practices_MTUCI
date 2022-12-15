#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <stdlib.h>
#include <wait.h>

#define NUMS_COUNT 15
#define CHILD_COUNT 3
#define NUMS_PER_CHILD NUMS_COUNT

int from_parent_fd[2];
int to_parent_fd[2];

struct Bucket {
    int size;
    int data[NUMS_PER_CHILD];
};

void bucket_add(struct Bucket* bucket, int value) {
    bucket->data[bucket->size] = value;
    bucket->size = bucket->size + 1;
}

void print_array(int* a, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d; ", a[i]);
    }

    printf("\n");
}



void print_bucket(struct Bucket bucket, char* endl) {
    for (int i = 0; i < bucket.size; i++) {
        printf("%d ", bucket.data[i]);
    }

    printf(endl);
}

int compare( const void* a, const void* b) {
     int int_a = * ( (int*) a );
     int int_b = * ( (int*) b );

     if ( int_a == int_b ) return 0;
     else if ( int_a < int_b ) return -1;
     else return 1;
}

void child_work() {
    printf("Child start working, PID = %d\n", getpid());
    
    struct Bucket bucket;

    read(from_parent_fd[0], &bucket, sizeof(struct Bucket));

    qsort(bucket.data, bucket.size, sizeof(int), compare);

    printf("Sorted nums by child: ");
    print_bucket(bucket, "\n");

    write(to_parent_fd[1], &bucket, sizeof(struct Bucket));
    
    printf("Child end working, PID = %d\n", getpid());
}

void parent_work() {
    printf("Parent start working, PID = %d\n", getpid());

    int sorted_count = 0;
    struct Bucket buckets[CHILD_COUNT];

    while (sorted_count < CHILD_COUNT) {
        read(to_parent_fd[0], &buckets[sorted_count], sizeof(struct Bucket));

        printf("Read sorted nums by parent: ");
        print_bucket(buckets[sorted_count], "\n");

        for (int j = sorted_count; j > 0; j--) {
            if (buckets[j].data[0] < buckets[j - 1].data[buckets[j - 1].size - 1]) {
                struct Bucket temp = buckets[j];
                buckets[j] = buckets[j - 1];
                buckets[j - 1] = temp;
            }
        }

        sorted_count++;
    }
    
    printf("\nResult: ");

    for (int i = 0; i < CHILD_COUNT; i++) {
        if (buckets[i].size > 0) {
            print_bucket(buckets[i], "");
        }
    }

    printf("\n");

    printf("Parent end working, PID = %d\n", getpid());
}

int main(void) {
    int ppid = getpid();
    int cpids[CHILD_COUNT];

    if (pipe(from_parent_fd) == -1) {
        perror ("Ошибка вызова pipe");
        exit (1); 
    }

    if (pipe(to_parent_fd) == -1) {
        perror ("Ошибка вызова pipe");
        exit (1); 
    }

    for (int i = 0; i < CHILD_COUNT; i++) {
        int cpid = fork();
        if (cpid == 0) {
            child_work();
            break;
        } else if (cpid == -1) {
            printf("Child process %d fork failed\n", i);
            return 1;
        } else {
            cpids[i] = cpid;
        }
    }

    if (getpid() == ppid) {
        FILE* file = fopen("numbers.txt", "r");
        int nums[NUMS_COUNT];
    
        for (int i = 0; i < NUMS_COUNT; i++) {
            fscanf(file, "%d", &nums[i]);
        }
        fclose(file);

        struct Bucket buckets[CHILD_COUNT];

        for (int i = 0; i < CHILD_COUNT; i++) {
            buckets[i].size = 0;
        }

        int min = INT_MAX;
        int max = INT_MIN;

        for (int i = 0; i < NUMS_COUNT; i++) {
            if (nums[i] < min) {
                min = nums[i];
            }

            if (nums[i] > max) {
                max = nums[i];
            }
        }

        for (int i = 0; i < NUMS_COUNT; i++) {
            int child_index = ((float)nums[i] - min) / (max - min) * (CHILD_COUNT - 1);
            bucket_add(&buckets[child_index], nums[i]);
        }



        for (int i = 0; i < CHILD_COUNT; i++) {
            write(from_parent_fd[1], &buckets[i], sizeof(struct Bucket));
        }

        parent_work();

        for (int i = 0; i < CHILD_COUNT; i++) {
            waitpid(cpids[i], 0, NULL);
        }
    }

    return 0;
}

