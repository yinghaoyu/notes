void swap(int arr[], int i, int j)
{
	int tmp = arr[i];
	arr[i] = arr[j];
	arr[j] = tmp;
}

void swap(int arr[], int i, int j)
{
	// 不用额外空间
	// 需要两个数不同内存地址，否则会清0
	if (i == j) return;
	arr[i] = arr[i] ^ arr[j];
	arr[j] = arr[i] ^ arr[j];
	arr[i] = arr[i] ^ arr[j];
}

void swap(int arr[], int i, int j)
{
	arr[i] = arr[i] + arr[j];  // 此处可能有溢出
	arr[j] = arr[i] - arr[j];
	arr[i] = arr[i] - arr[j];
}