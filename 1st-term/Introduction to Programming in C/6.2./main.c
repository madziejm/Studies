#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sprawnoscsortowania.h"

int main()
{
    srand (time(NULL));
    double t[100];
    assignRand(t, 100);
    insertSort(t, 100, less);
    print(t, 100);
    printf("sortowanie poprawne: %d\n", sortPerformsCorrectly(t, 100, insertSort));
    printf("%d\n", countRandomSortTime(10000, insertSort));
    printf("\ninsert sort:\n");
    printRandomSortStats(100, 100, insertSort);
    printf("\nquick sort:\n");
    printRandomSortStats(100, 100, quicksort);

    printf("\ninsert sort:\n");
    printRandomSortStats(1000, 100, insertSort);
    printf("\nquick sort:\n");
    printRandomSortStats(1000, 100, quicksort);

    printf("\ninsert sort:\n");
    printRandomSortStats(10000, 100, insertSort);
    printf("\nquick sort:\n");
    printRandomSortStats(10000, 100, quicksort);
    return 0;
}
