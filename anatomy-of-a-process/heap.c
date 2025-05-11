#include <stdio.h>
#include <stdlib.h>

// void flow1(int* ptr1) {
//     *ptr1 = 50;
//     printf("%s %p %d\n", "flow1", (void*)ptr1, *ptr1);
//     free(ptr1);
// }

// void flow2(int* ptr2) {
//     *ptr2 = 100;
//     printf("%s %p %d\n", "flow2", (void*)ptr2, *ptr2);
// }

struct data_struct {
    char* name; // 8
    int age; // 4
};


void pass_cp(int ptr2) {
    ptr2 = 100;
    // printf("%s %p %d\n", "flow2", (void*)ptr2, *ptr2);
    printf("%s\n", "pass_cp");
    printf("%d\n", ptr2);
}

int main() {
    // int *foo = malloc(sizeof(int));
    // int *arr = (int *)malloc(10*sizeof(int));
    // for (int i=0; i < 10; i++) {
    //     arr[i] = (rand() % 100) + 1;
    //     printf("%p %d\n", &arr[i], arr[i]);
    // }
    // free(arr);
    // for (int i=0; i < 10; i++) {
    //     printf("%p %d\n", &arr[i], arr[i]);
    // }

    // pass_cp(*arr);
    // int* ptr1 = foo;
    // int* ptr2 = foo;

    // flow1(ptr1);
    // flow2(ptr2);

    // *foo = 200;
    // printf("%s %p %d\n", "main", (void*)foo, *foo);


    // struct
    struct data_struct *ds1 = (struct data_struct *)malloc(sizeof(struct data_struct));
    printf("%d\n", sizeof(struct data_struct));
    free(ds1);
    free(ds1);
}