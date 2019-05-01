/*
 * =====================================================================================
 *
 *       Filename:  my_heap.h
 *
 *    Description:  First approach to implement a heap.
 *
 *        Version:  1.0
 *        Created:  03/30/19 20:43:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Maciej M
 *        Company:  
 *
 * =====================================================================================
 */
typedef int bool;
#define true 1
#define false 0

typedef long long* heap;

void move_down(heap h, long long n, long long i);
void move_up(heap h, long long n, long long i);

void change_element(heap h, long long n, long long i, long long u)
{
	long long x=h[i];
	h[i]=u;
	if(u<x)
		move_up(h, n, i);
	else
		move_down(h, n, i);
	return;
}

void swap(long long *a, long long *b)
{
	long long temp=*a;
	*a=*b;
	*b=temp;
	return;
}

void move_down(heap h, long long n, long long i)
{
	long long k=i;
	long long j=k;
	do
	{
		j=k;
		if(2*j<=n && h[2*j]>h[k]) k=2*j;
		if(2*j<n && h[2*j+1]>h[k]) k=2*j+1;
		swap(&h[j], &h[k]);
	}while(j!=k);
	return;
}


void move_up(heap h, long long n, long long i)
{
	long long k=i;
	long long j=k;
	do
	{
		j=k;
		if(j>1 && h[j/2]<h[k])
			k=j/2;
		swap(&h[j], &h[k]);
	}while(j!=k);
	return;
}

long long find_max(heap h)
{
	return h[1];
}

void remove_max(heap h, long long *n)
{
	h[1]=h[(*n)];
	// 
	move_down(h, (*n)--, 1);
	return;
}

void insert_value(heap h, long long *n, long long value)
{
	(*n)++;
	h[*n]=value;
	move_up(h, *n, *n);
	return;
}

bool empty(long long n)
{
	return n==0;
}

#include <stdlib.h>
#include <stdio.h>

heap h;
long long M, count; //input
long long n=0;
long long last=-12345;
// long long products_printed=0;


void print_heap_max()
{
    long long max=find_max(h);
    printf("%lld\n", max);
    // products_printed++;
    count--;
    if(count==0)
        exit(0);
    last=max;
    remove_max(h, &n);
    return;
}

bool print_distinct_greater_than(long long greater_than)
{
    while(last==find_max(h))
    {
        remove_max(h, &n);
    }
    long long max=find_max(h);
    if(max>greater_than)
    {
        print_heap_max();
        return true;
    }
    else
    {
        return false;
    }
}

void print_all_distinct_greater_than(long long greater_than)
{
    while(!empty(n) && print_distinct_greater_than(greater_than))
        {}
    return;
}

#define HEAP_ELEMENTS (int)(1400000+1)

void tricky_insert_value(long long number)
{
    
    insert_value(h, &n, number);
        // for(int i=0; i<1000; i++)
        //     print_heap_max();
    
    if(n==HEAP_ELEMENTS)
        n--;
}


// void print_heap_array(heap h, long long n)
// {
//     for(unsigned long long i=0; i<n; i++)
//     {
//         printf("%lld ", h[i]);
//     }
//     printf("\n");
//     return;
// }

int main(){// rozmiar kwadratu tabliczki, liczba największych liczb, które mają zostać wypisane
    h=(long long*)malloc(sizeof(long long)*HEAP_ELEMENTS);
    scanf("%lld %lld", &M, &count);

    for(long long collumn=M; collumn!=0; collumn--)
    {
        for(long long row=M; row>=collumn; row--)
        {
            long long product=collumn*row;
            if(row==M)
            {
                print_all_distinct_greater_than(product);
            }
            // printf("collumn%lld row%lld %lld; ", collumn, row, collumn*row);
            // printf("%lld ", product);
            tricky_insert_value(product);
        }
        // printf("\n");
        // print_heap_array(h, n);
    }

    print_all_distinct_greater_than(0);

    // while(n>0)
    // {
    //     printf("%lld \n", find_max(h));
    //     remove_max(h, &n);
    // }
    // print_all_distinct_greater_than(10);
}
