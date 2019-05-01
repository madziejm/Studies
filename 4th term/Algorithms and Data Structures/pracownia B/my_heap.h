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
