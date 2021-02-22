/*
 * =====================================================================================
 *
 *       Filename:  browary2nd.c
 *
 *    Description:  Zadango na pracownie z AIDS-u
 *
 *        Version:  1.0
 *        Created:  03/20/19 18:45:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Maciej M
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>
long long n;
long long sum_of_distances=0;
long long max_distance=0;
// long long distance_from_zero_vertex_clockwise[1000000];


long long min(long long a, long long b)
{
    return a < b ? a : b;
}

long long max(long long a, long long b)
{
    return a > b ? a : b;
}

long long abs_ll(long long a)
{
    return a<0?-a:a;
}

long long j_to_i_distance(long long distance_from_zero_vertex_clockwise[], long long i, long long j) {
    return (i<j) ? sum_of_distances +
					distance_from_zero_vertex_clockwise[i] -
					distance_from_zero_vertex_clockwise[j] 
					:
					distance_from_zero_vertex_clockwise[i] - distance_from_zero_vertex_clockwise[j];
}

int main()
{
	scanf("%lld", &n);
	long long distance[n];
	long long distance_from_zero_vertex_clockwise[n];
	for(long long i=0; i<n; i++)
	{
		scanf("%lld", &(distance[i]));
		sum_of_distances+=distance[i];
		if(i==0)
			distance_from_zero_vertex_clockwise[i]=0;
		else
			distance_from_zero_vertex_clockwise[i]=distance_from_zero_vertex_clockwise[i-1]+distance[i];
	}
	// sum_of_distances = distance_from_zero_vertex_clockwise[n - 1] + distance[n - 1];


	for(long long i=0; i<n; i++)
		printf("%lld\n", distance_from_zero_vertex_clockwise[i]);

	long long second_vertex=1;
	for(long long first_vertex=0; first_vertex<n/2+1; first_vertex++)
	{
		while(j_to_i_distance(distance_from_zero_vertex_clockwise, first_vertex, second_vertex)<0.5*sum_of_distances && second_vertex<n)
			second_vertex++;
		printf("first:%lld second:%lld\n", first_vertex, second_vertex);
		long long second_vertex_clockwise_distance=j_to_i_distance(distance_from_zero_vertex_clockwise, (second_vertex) % n, first_vertex);
		long long second_vertex_but_one_clockwise_distance=j_to_i_distance(distance_from_zero_vertex_clockwise, (second_vertex+1) % n, first_vertex);
		long long second_vertex_anticlockwise_distance=sum_of_distances-second_vertex_clockwise_distance;
		long long second_vertex_but_one_anticlockwise_distance=sum_of_distances-second_vertex_but_one_clockwise_distance;
		long long longest_path_from_first_vertex=max(
			min(second_vertex_clockwise_distance, second_vertex_anticlockwise_distance), 
			min(second_vertex_but_one_clockwise_distance, second_vertex_but_one_anticlockwise_distance));
		printf("%lld %lld %lld %lld %lld \n",
		 second_vertex_clockwise_distance,
		 second_vertex_but_one_clockwise_distance, 
		 second_vertex_anticlockwise_distance, 
		 second_vertex_but_one_anticlockwise_distance, 
		 longest_path_from_first_vertex);
		if(longest_path_from_first_vertex>max_distance)
			max_distance=longest_path_from_first_vertex;
	}
	
	printf("%lld\n", max_distance);
	
	return 0;
}

