#include <stdio.h>

int input(int *a, int n);
void output(int *a, int c);
int func1(int *a, int n, int *b);
int arr_numb();

int main()
{
    int n,c;
    n = arr_numb();
    int data[n];
    int temp_arr[n];

    input(data, n);
    c = func1 (data, n, temp_arr);
    printf("%d\n", c);
    output(temp_arr, c);

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

void output(int *a, int c){
    for (int i = 0; i != c; i++){
        int x = a[i];
        printf("%d ",x);
    }    
}

int func1(int *a, int n, int *b){
    int x,p;
    for (int i = 0; i != n; i++){
        if (a[i] % 2 == 0){
            x += a[i];
            printf("\n%d\n",x);
            p++;
        }
    }
    for (int i = 0; i !=n; i++){
        if (x % a[i] == 0){
            b[i] = a[i];
        }
    }
    return p;
}