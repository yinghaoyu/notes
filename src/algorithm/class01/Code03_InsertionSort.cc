void swap(int arr[], int i, int j)
{
	int tmp = arr[i];
	arr[i] = arr[j];
	arr[j] = tmp;
}

void InsertionSort(int arr[], int len)
{
	//  时间复杂度O(n^2)，空间复杂度O(1)，稳定
	//  比较次数n~(n-1)+(n-2)+...+1，取决于初始元素顺序
    //  交换次数0~(n-1)+(n-2)+...+1，取决于初始元素顺序
	if(arr == NULL || len < 2)
	{
		return;
	}
	// 不只1个数
	for(int i = 1; i < len; ++i)  // 0 ~ i 做到有序
	{
		for(int j = i-1; j >=0 && arr[j] > arr[j+1]; --j)
		{
			swap(arr, j, j+1);
		}
	}
}