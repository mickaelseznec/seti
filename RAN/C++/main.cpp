#include <iostream>

#include "FigGeom.h"

using namespace std;

int **alloc_2d (int x, int y)
{
    int **tab = new int*[x];
    for (int i = 0; i < x; i++)
        tab[i] = new int[y];
    return tab;
}

void free_2d (int **tab, int x)
{
    for (int i = 0; i < x; i++)
        delete[] tab[i];
    delete[] tab;
}

int SearchMin (const float * arr, int size)
{
    int pos = 0;
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[pos])
            pos = i;
    }
    return pos;
}

int main ()
{
    float float_arr[] = {-1.5, 299.12, 3.14, -982738., 3487.};
    cout << "Min is at pos " << SearchMin(float_arr, sizeof(float_arr) / sizeof(float)) << endl;
    return 0;
}

