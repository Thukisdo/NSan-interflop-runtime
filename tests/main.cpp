#include <stdlib.h>
#include <stdio.h>


void f(float* tab, size_t size)
{
    for (int i = 0; i < size; i++)
        tab[i] += 4;
}

void g(float ft)
{
    printf("%f\n", ft);
}

int main() {
    float* tab = (float*) malloc(sizeof(float) * 2);

    tab[1] = 1;
    for (int i = 1; i < 2; i++)
        tab[i] = tab[i - 1] + 2;

    f(tab, 2);

    float a = 5;
    float b = 10;
    float c = a + b;
    g(5);

    printf("Ptr : %p\n", tab);
    free(tab);
    return 0;
}