// 在有序sortedArr上
bool exist(int sortedArr[], int length, int num)
{
	if (sortedArr == NULL || length == 0)
	{
		return false;
	}
	int L = 0;
	int R = length - 1;
	int mid = 0;
	// L..R
	while (L < R)
	{ // L..R 至少两个数的时候
		mid = L + ((R - L) >> 1);
		if (sortedArr[mid] == num)
		{
			return true;
		}
		else if (sortedArr[mid] > num)
		{
			R = mid - 1;
		}
		else
		{
			L = mid + 1;
		}
	}
	return sortedArr[L] == num;
}

// 在有序arr上，找满足>=value的最左位置，对应lower_bound
int nearestIndex(int arr[], int length, int value)
{
	int L = 0;
	int R = length - 1;
	int index = -1; // 记录最左的对号
	while (L <= R)
	{ // 至少一个数的时候
		int mid = L + ((R - L) >> 1);
		if (arr[mid] >= value)
		{
			index = mid;
			R = mid - 1;
		}
		else
		{
			L = mid + 1;
		}
	}
	return index;
}

// 在有序arr上，找满足<=value的最右位置，对应upper_bound
int nearestIndex(int arr[], int length, int value)
{
	int L = 0;
	int R = length - 1;
	int index = -1; // 记录最右的对号
	while (L <= R)
	{
		int mid = L + ((R - L) >> 1);
		if (arr[mid] <= value)
		{
			index = mid;
			L = mid + 1;
		}
		else
		{
			R = mid - 1;
		}
	}
	return index;
}
// 找出任意一个序列的导数为0的点
int getLessIndex(int arr[], int legnth)
{
	if (arr == null || length == 0)
	{
		return -1; // no exist
	}
	if (length == 1 || arr[0] < arr[1])
	{
		return 0;
	}
	if (arr[length - 1] < arr[length - 2])
	{
		return length - 1;
	}
	int left = 1;
	int right = length - 2;
	int mid = 0;
	while (left < right)
	{
		mid = (left + right) / 2;
		if (arr[mid] > arr[mid - 1])
		{
			right = mid - 1;
		}
		else if (arr[mid] > arr[mid + 1])
		{
			left = mid + 1;
		}
		else
		{
			return mid;
		}
	}
	return left;
}