#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 11

typedef struct 
{
	int row;
	int column;
}parameters;

// sudoku grid to validate

int sudoku[9][9];

// Array to store the results of each thread

int results[NUM_THREADS] = {0};

void *check_row(void *param)
{
	parameters *p = (parameters *)param;
	int row = p->row;
	int valid[9] = {0};
	
	for (int col = 0; col < 9; col++)
	{
		int num = sudoku[row][col];
		if (num < 1 || num > 9 || valid[num - 1] == 1 )
		{
			results[0] = 0;
			pthread_exit(0);
		}
		else
		{
			valid[num - 1] = 1;
		}

	}
	results[0] = 1;
	pthread_exit(0);
}

void *check_column(void *param)
{
	parameters *p = (parameters *)param;
	int column = p->column;
	int valid[9] = {0};
	
	for (int row = 0; row < 9; row++)
	{
		int num = sudoku[row][column];
		if (num < 1 || num > 9 || valid[num - 1] == 1)
		{
			results[1] = 0;
			pthread_exit(0);
		}
		else
		{
			valid[num - 1] = 1;
		}
	}
	results[1] = 1;
	pthread_exit(0);
}

void *check_subgrid(void *param)
{
	parameters *p = (parameters *)param;
	int startRow = p->row;
	int startCol = p->column;
	int valid[9] = {0};

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int num = sudoku[startRow + i][startCol + j];
			if (num < 1 || num > 9 || valid[num - 1] == 1)
			{
				results[2 + (startRow / 3) * 3 + startCol / 3] = 0;
				pthread_exit(0);
			}
			else
			{
				valid[num - 1] = 1;
			}
		}
	}
	results[2 + (startRow / 3) * 3 + startCol / 3] = 1;
	pthread_exit(0);
}

void print_sudoku()
{
	printf("Sudoku grid:\n");
	for (int i = 0; i < 9; i++)
	{
		if (i % 3 == 0 && i != 0)
		{
			printf("---------------------\n");
		}
		for (int j = 0; j < 9; j++)
		{
			if (j % 3 == 0 && j != 0)
			{
				printf("| ");
			}
			printf("%d", sudoku[i][j]);
		}
		printf("\n");
	}
}


int main()
{
	pthread_t threads[NUM_THREADS];
	int thread_index = 0;
	parameters *data;

	// Read the sudoku grid from the user
	printf("Please enter the sudoku grid (9x9) row, with each number separated by space: \n");
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			scanf("%d", &sudoku[i][j]);
		}
	}
	
	//print the entered sudoku grid
	print_sudoku();

	//Create thread to check rows
	data = (parameters *) malloc(sizeof(parameters));
	data->row = 0;
	data->column = 0;
	pthread_create(&threads[thread_index++], NULL, check_row, data);

	//Create thread to check columns 
	data = (parameters *) malloc(sizeof(parameters));
	data->row = 0;
	data->column = 0;
	pthread_create(&threads[thread_index++], NULL, check_column, data);


	//create threads to check subgrids
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			data = (parameters *) malloc(sizeof(parameters));
			data->row = i * 3;
			data->column = j * 3;
			pthread_create(&threads[thread_index++], NULL, check_subgrid, data);
		}
	}

	//wait for all threads to finish
	for (int i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
	//Check results
	int valid = 1;
	for (int i = 0; i < NUM_THREADS; i++)
	{
		if (results[i] == 0)
		{
			valid = 0;
			break;
		}
	}
	
	if (valid)
	{
		printf("Sudoku solution is valid. \n");
	}
	else
	{
		printf("Sudoku solution is invalid. \n");
	}

	return 0;

}

