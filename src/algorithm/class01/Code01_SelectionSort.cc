void swap(int arr[], int i, int j)
{
	int tmp = arr[i];
	arr[i] = arr[j];
	arr[j] = tmp;
}

void selectionSort(int arr[], int len)
{
	//  时间复杂度O(n^2)，空间复杂度O(1)，不稳定
	//  比较次数(n-1)+(n-2)+...+1
    //  交换次数n-1	
	if(arr == NULL || len < 2)
	{
		return;
	}
	// 0 ~ N-1  找到最小值，在哪，放到0位置上
	// 1 ~ N-1  找到最小值，在哪，放到1位置上
	// 2 ~ N-1  找到最小值，在哪，放到2位置上
	for(int i = 0; i < len-1; ++i)
	{
		int minIndex = i;
		for(int j = i+1 ; j < len; ++j)  // i ~ N-1 上找最小值的下标
		{
			minIndex  = arr[j] < arr[minIndex] ? j : minIndex;
		}
		swap(arr, i, minIndex);
	}
}