#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THRESHOLD 100

typedef struct
{
	int *array;
	int left;
	int right;
} Task;

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

int partition(int *array, int left, int right)
{
	int pivot = array[right];
	int i = left - 1;
	for (int j = left; j < right; j++)
	{
		if (array[j] <= pivot)
		{
			i++;
			swap(&array[i], &array[j]);
		}
	}
	swap(&array[i + 1], &array[right]);
	return i + 1;
}

void insertion_sort(int *array, int left, int right)
{
	for (int i = left + 1; i <= right; i++)
	{
		int key = array[i];
		int j = i - 1;
		while (j >= left && array[j] > key)
		{
			array[j + 1] = array[j];
			j--;
		}
		array[j + 1] = key;
	}
}

void *quicksort(void *arg)
{
	Task *task = (Task *)arg;
	int *array = task->array;
	int left = task->left;
	int right = task->right;

	if (left < right)
	{
		if (right - left + 1 < THRESHOLD)
		{
			insertion_sort(array, left, right);
		}
		else
		{
			int pi = partition(array, left, right);

			Task leftTask = {array, left, pi - 1};
			Task rightTask = {array, pi + 1, right};

			pthread_t leftThread, rightThread;

			pthread_create(&leftThread, NULL, quicksort, &leftTask);
			pthread_create(&rightThread, NULL, quicksort, &rightTask);

			pthread_join(leftThread, NULL);
			pthread_join(rightThread, NULL);
		}
	}

	return NULL;

}

void quicksort_parallel(int *array, int n)
{
	Task initialTask = {array, 0, n - 1};
	quicksort(&initialTask);
}

int main()
{
	int n;
	printf("Enter the size of the array: ");
	scanf("%d", &n);

	int *array = (int *)malloc(n * sizeof(int));
	printf("Enter the elements of the array separated by a space: ");
	for (int i = 0; i < n; i++)
	{
		scanf("%d", &array[i]);
	}

	quicksort_parallel(array, n);

	printf("sorted array: ");
	for (int i = 0; i < n; i++)
	{
		if (i != n - 1)
		{
			printf("%d, ", array[i]);
		}
		else
		{
			printf("%d", array[i]);
		}
	}
	printf("\n");

	free(array);

	return 0;
}
