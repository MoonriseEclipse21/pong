#include <stdio.h>

int input(int *a, int n);
void output(int *a, int n);
void squaring(int *a, int n);
int arr_numb();

int main()
{
    int n;
    n = arr_numb();
    int data[n];
    input(data, n);
    squaring(data, n);
    output(data, n);

    return 0;
}

int arr_numb(){
    int b;
    scanf("%d", &b);
    return b;
}

int input(int *a, int n) {
    int ret = 0;

    for (int *ptr = a; ptr - a < n; ptr++) {
        char ch;
        if (scanf("%d", ptr) != 1){
            break;
        }
        ch = getchar();
        if (ch != ' ') {
            if (ch == '\n' && ptr - a + 1 == n) {
                ret = 1;
                break;
            }
        }
    }
    return ret;
}

void output(int *a, int n){
    for (int i = 0; i != n; i++){
        int x = a[i];
        printf("%d ",x);
    }    
}

void squaring(int *a, int n){
    int temporary1,temporary2; 
    for (int i = 0; i < n; i++){
        a[i] = a[i] * a[i];
    }
}