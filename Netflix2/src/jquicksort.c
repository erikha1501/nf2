#include "../inc/jquicksort.h"

typedef struct
{
    int i, j;
} partition_ret;

void swap(Jval a[], int firstIndex, int secondIndex);
void qsort_internal(Jval a[], int low, int high, Jval_Comparer comparer);
partition_ret partition(Jval a[], int low, int high, Jval_Comparer comparer);

void swap(Jval a[], int firstIndex, int secondIndex)
{
    Jval temp = a[firstIndex];
    a[firstIndex] = a[secondIndex];
    a[secondIndex] = temp;
}

void qsort_jval(Jval arr[], int low, int high, Jval_Comparer comparer)
{
    qsort_internal(arr, low, high, comparer);
}

void qsort_internal(Jval a[], int low, int high, Jval_Comparer comparer)
{
    if (low < high)
    {
        partition_ret p = partition(a, low, high, comparer);
        qsort_internal(a, low, p.i, comparer);
        qsort_internal(a, p.j, high, comparer);
    }
}

partition_ret partition(Jval a[], int low, int high, Jval_Comparer comparer)
{
    int i = low - 1;
    int j = high;
    int p = low - 1;
    int q = high;
    Jval pivot = a[high];

    while (1)
    {
        do
        {
            i++;
        } while (comparer(&a[i], &pivot) < 0);

        do
        {
            j--;
        } while (comparer(&a[j], &pivot) > 0 && j > low);

        if (i >= j)
        {
            break;
        }

        swap(a, i, j);

        if (comparer(&a[i], &pivot) == 0)
        {
            p++;
            swap(a, i, p);
        }

        if (comparer(&a[j], &pivot) == 0)
        {
            q--;
            swap(a, j, q);
        }
    }

    swap(a, i, high);

    int t1 = i - 1;
    for (int x = low; x <= p; x++, t1--)
    {
        swap(a, x, t1);
    }

    int t2 = i + 1;
    for (int y = high - 1; y >= q; y--, t2++)
    {
        swap(a, y, t2);
    }

    return (partition_ret){t1, t2};
}