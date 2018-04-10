#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "sprawnoscsortowania.h"

void print(const double *t, const int n)
{
    for(int i=0; i<n-1; i++)
        printf("%lf ", t[i]);
    printf("%lf\n", t[n-1]);
    return;
}

void swap(double *a, double *b)
{
    double t=*a;
    *a=*b;
    *b=t;
    return;
}

void insertSort(double t[], int n, int(*comparator)(const void*, const void*))
{
    if(n<2)
        return;
    double *minimum=t;
    for(int i=1; i<n; i++)
        if(comparator(t+i, minimum)==-1)
        {
            swap(t+i, minimum);
        }
    swap(minimum, t);
    insertSort(t+1, n-1, comparator);
    return;
}

void quicksort(double t[], int n, int(*comparator)(const void*, const void*))
{
    qsort(t, n, sizeof(double), comparator);
    return;
}

double randDouble()
{
    return ((rand()&1)?1.0:-1.0)*(double)rand()/(double)rand();
}

void assignRand(double t[], int n)
{
    for(int i=0; i<n; i++)
        t[i]=randDouble();
    return;
}

int less(const void* a, const void*b)
{
    return *(double*)a==*(double*)b ? 0:(*(double*)a<*(double*)b?-1:1);
}

_Bool sortPerformsCorrectly(double *t, int n, void(*sort)(double t[], int n, int(*comparator)(const void*, const void*)))
{
    print(t, n);
    _Bool *presentInSortedArray;
    presentInSortedArray=malloc(n*sizeof(_Bool));
    double *tcopy;
    tcopy=malloc(n*sizeof(double));
    for(int i=0; i<n; i++)
    {
        tcopy[i]=t[i];
        presentInSortedArray[i]=0;
    }
    sort(tcopy, n, less);
    printf("\n");
    print(tcopy, n);
    for(int i=1; i<n; i++)
        if(tcopy[i-1]>=tcopy[i])
            goto returnfalse;
    for(int i=0; i<n; i++) //poszukujemy kazdego elementu tcopy w t
    {
        int j=0;
        for(;j<n; j++)
        {
            if(tcopy[i]==t[j] && presentInSortedArray[j]==0)
                break; //czyli znalezlismy taki element ciagu
        }
        if((j==n-1 && tcopy[i]!=t[j]) || presentInSortedArray[j]==1) //nie znaleziono albo znaleziono taki ktory juz byl w wyjsciowej tablicy
            goto returnfalse;
        presentInSortedArray[j]=1;
    }
    for(int i=0; i<n; i++)
        if(presentInSortedArray[i]==0)
            goto returnfalse;
    free(presentInSortedArray);
    free(tcopy);
    return 1;
    returnfalse:
    free(presentInSortedArray);
    free(tcopy);
    return 0;
}

int countRandomSortTime(int n, void(*sort)(double t[], int n, int(*comparator)(const void*, const void*)))
{
    double *t;
    t=malloc(n*sizeof(double));
    assignRand(t, n);
    int time=clock();
    sort(t, n, less);
    time=clock()-time;
    free(t);
    return time;
}

void printRandomSortStats(int length, int iterations, void(*sort)(double t[], int n, int(*comparator)(const void*, const void*)))
{
    printf("iterations: %d, length: %d\n", iterations, length);
    int min=1<<30;
    int max=0;
    double avg=0;
    for(int i=0; i<iterations; i++)
    {
        int t=countRandomSortTime(length, sort);
        if(t<min)
            min=t;
        if(max<t)
            max=t;
        avg+=t;
    }
    printf("minimum: %d\nmaximum: %d\navg time: %lf\n", min, max, avg/(double)iterations);
}

