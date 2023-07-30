#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if (l < r)
	{
		int m = (l + r) / 2;
		extraMemoryAllocated += sizeof(int);

		mergeSort(pData, l, m);
		mergeSort(pData, m + 1, r);

		int leftLength = m - l + 1;
		int rightLength = r - m;
		extraMemoryAllocated += sizeof(int) * 2;

		// malloc used to create subarrays on heap rather than stack to avoid overflow
		int* leftSub = malloc(sizeof(int) * leftLength);
		int* rightSub = malloc(sizeof(int) * rightLength);
		extraMemoryAllocated += sizeof(int) * (leftLength + rightLength);

		int i, j;
		extraMemoryAllocated += sizeof(int) * 2;

		// Copy values to subarrays
		for (i = 0; i < leftLength; i++)
		{
			leftSub[i] = pData[l + i];
		}
		for (j = 0; j < rightLength; j++)
		{
			rightSub[j] = pData[m + 1 + j];
		}

		i = 0, j = 0;
		int k = l;
		extraMemoryAllocated += sizeof(int);

		// Add smallest from each array
		while (i < leftLength && j < rightLength)
		{
			if (leftSub[i] <= rightSub[j])
			{
				pData[k] = leftSub[i];
				i++;
			}
			else
			{
				pData[k] = rightSub[j];
				j++;
			}

			k++;
		}

		// Fill rest of array with remaining values
		while (i < leftLength)
		{
			pData[k] = leftSub[i];
			i++;
			k++;
		}

		while (j < rightLength)
		{
			pData[k] = rightSub[j];
			j++;
			k++;
		}
	}
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	int i, j, swap;

	extraMemoryAllocated += sizeof(int) * 3;

	for (i = 1; i < n; i++)
	{
		for (j = 0; j < i; j++)
		{
			if (pData[j] > pData[i])
			{
				swap = pData[j];
				pData[j] = pData[i];
				pData[i] = swap;
			}
		}
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	int i, j, swap;

	extraMemoryAllocated += sizeof(int) * 3;

	for (i = 0; i < n - 1; i++)
	{
		for (j = 0; j < n - 1 - i; j++)
		{
			if (pData[j] > pData[j + 1])
			{
				swap = pData[j];
				pData[j] = pData[j + 1];
				pData[j + 1] = swap;
			}
		}
	}
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	int i, j, minIndex, swap;

	extraMemoryAllocated += sizeof(int) * 4;

	for (i = 0; i < n - 1; i++)
	{
		minIndex = i;

		for (j = i + 1; j < n; j++)
		{
			if (pData[j] < pData[minIndex])
			{
				minIndex = j;
			}
		}

		if (minIndex != i)
		{
			swap = pData[i];
			pData[i] = pData[minIndex];
			pData[minIndex] = swap;
		}
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}

		for (i = 0; i < dataSz; ++i)
		{
			fscanf(inFile,"%d",(*ppData)+i);
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}