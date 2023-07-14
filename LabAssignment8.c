#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

// swap funtion
void swap (int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

// creates a heap through array
void heapify(int arr[], int n, int i) {
  // setting variables and children
  int minVal = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;

  // if left val is larger than maxVal, set new maxVal
  if (left < n && arr[left] > arr[minVal])
    minVal = left;
  // if right val is larger than maxVal, set new maxVal
  if (right < n && arr[right] > arr[minVal])
    minVal = right;

  // update new maxVal, continue down sub tree
  if (minVal != i) {
    swap(&arr[i], &arr[minVal]);
    heapify(arr, n, minVal);
  }
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n) {
  // transforming array into a heap
  for (int i = n / 2 - 1; i >= 0; i--)
    heapify(arr, n, i);
  
  // sorting array through heapifying
  for (int i = n - 1; i > 0; i--) {
    // setting root to end
    swap(&arr[0], &arr[i]);

    // heapifying sub trees, top to bottom
    // after each iteration the lowest value should be in its place
    heapify(arr, i, 0);
  }
}

// merge function to sort and merge subarrays after recursion
void merge(int pData[], int l, int mid, int r) {
	// vars
  int i, j;
  
  // lengths of two sub arrays
	int len1 = mid - l + 1;
	int len2 = r - mid;

	// allocating memory for two unsorted temp arrays
	int *arr1 = malloc(sizeof(int) * len1);
	int *arr2 = malloc(sizeof(int) * len2);

  // updating extraMemoryAllocated
  extraMemoryAllocated += (sizeof(int) * len1);
  extraMemoryAllocated += (sizeof(int) * len2);

	// fill up temp arrays with vals
	for (i = 0; i < len1; i++)
		arr1[i] = pData[l + i];
	for (i = 0; i < len2; i++)
		arr2[i] = pData[mid + 1 + i];
	
	// indexes of first temp sub array, second temp sub array, and final sorted array
	i = 0;
	j = 0;
	int k = l;

	// sorting into pData array
	while (i < len1 && j < len2) {
		if (arr1[i] <= arr2[j]) {
			pData[k] = arr1[i];
			i++;
		}
		else if (arr2[j] < arr1[i]) {
			pData[k] = arr2[j];
			j++;
		}
		k++;
	}

	// loop that resolves any unemptied arrays
	while (i < len1) {
		pData[k] = arr1[i];
		i++;
		k++;
	}

	while (j < len2) {
		pData[k] = arr2[j];
		j++;
		k++;
	}

	// free temp arrays as they have served their purpose and are now emptied :)
	free(arr1);
	free(arr2);
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r) {
	if (l < r) {
		int mid = (l + r) / 2;

		mergeSort(pData, l, mid);
		mergeSort(pData, mid + 1, r);
		merge(pData, l, mid, r);
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
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
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
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
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.3lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.3lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}
