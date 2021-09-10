void swap(int arr[], int i, int j)
{
	int tmp = arr[i];
	arr[i] = arr[j];
	arr[j] = tmp;
}

void bubbleSort(int arr[], int len)
{
	//  时间复杂度O(n^2)，空间复杂度O(1)，稳定
	//  比较次数(n-1)+(n-2)+...+1
    //  交换次数0~(n-1)+(n-2)+...+1，取决于初始元素顺序
	if(arr == NULL || len < 2)
	{
		return;
	}
	// 0 ~ N-1
	// 0 ~ N-2
	// 0 ~ N-3
	for(int i = len-1; i > 0; --i)
	{
		for(int j = 0; j < i; ++j)
		{
			if(arr[j] > arr[j+1])
			{
				swap(arr, j, j+1);
			}
		}
	}
}