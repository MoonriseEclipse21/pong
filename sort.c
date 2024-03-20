#include <stdio.h>
#define NMAX 10
int input(int *a, int b);
void func(int *a, int b);
void output(int *a, int b);

int main()
{
 int data[NMAX], result;
    result = input(data, NMAX);
    if (result) {
        func(data, NMAX);
        output(data, NMAX);
    } else {
        printf("n/a");
    }
    return 0;
}

int input(int *a, int b) {
    int ret = 0;

    for (int *ptr = a; ptr - a < b; ptr++) {
        char ch;
        if (scanf("%d", ptr) != 1){
            break;
        }
        ch = getchar();
        if (ch != ' ') {
            if (ch == '\n' && ptr - a + 1 == b) {
                ret = 1;
                break;
            }
        }
    }
    return ret;
}

void func(int *a, int b){
    int temporary1,temporary2; 
    for (int i = 0; i < b; i++){
        for (int j = 0; j < b; j++){
            if (a[i] < a[j]){
                temporary1 = a[i];
                temporary2 = a[j];
                a[i] = temporary2;
                a[j] = temporary1;
            }
        }
    }
}

void output(int *a, int b){
    for (int i = 0; i != b; i++){
        int x = a[i];
        printf("%d ",x);
    }    
}