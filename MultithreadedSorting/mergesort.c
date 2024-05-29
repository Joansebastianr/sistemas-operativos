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

void insertion_sort(int *array, int left, int right)
{
	for (int i = left + 1; i <= right; i++)
	{
		int key = array[i];
		int j = i -1;
		while (j >= left && array[j] > key)
		{
			array[j + 1] = array[j];
			j--;
		}
		array[j+ 1] = key;
	}
}

void merge(int *array, int left, int mid, int right)
{
	int n1 = mid - left + 1;
	int n2 = right - mid;
	int *L = (int *)malloc(n1 * sizeof(int));
	int *R = (int *)malloc(n2 * sizeof(int));

	for (int i = 0; i < n1; i++)
		L[i] = array[left + i];
	for (int i = 0; i < n2; i++)
		R[i] = array[mid + 1 + i];

	int i = 0, j = 0, k = left;
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			array[k++] = L[i++];
		}
		else
		{
			array[k++] = R[j++];
		}
	}

	while (i < n1)
	{
		array[k++] = L[i++];
	}

	while (j < n2)
	{
		array[k++] = R[j++];
	}

	free(L);
	free(R);
}

void *mergesort(void *arg)
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
			int mid = left + (right - left) / 2;

			Task leftTask = {array, left, mid};
			Task rightTask = {array, mid + 1, right};

			pthread_t leftThread, rightThread;

			pthread_create(&leftThread, NULL, mergesort, &leftTask);
			pthread_create(&rightThread, NULL, mergesort, &rightTask);

			pthread_join(leftThread, NULL);
			pthread_join(rightThread, NULL);

			merge(array, left, mid, right);
		}
	}

	return NULL;
}

void mergesort_parallel(int *array, int n)
{
	Task initialTask = {array, 0, n - 1};
	mergesort(&initialTask);

}

int main()
{
	int n;
	printf("Enter the size of the array");
	scanf("%d", &n);

	int *array = (int *)malloc(n * sizeof(int));
	printf("Enter the elements of the array separed by a space: ");
	for (int i = 0; i < n; i++)
	{
		scanf("%d", &array[i]);
	}

	mergesort_parallel(array, n);

	printf("Sorted array: ");

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
