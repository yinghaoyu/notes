class MyQueue
{
public:
	MyQueue(int limit)
	{
		arr = new int[limit];
		pushi = 0;
		polli = 0;
		size = 0;
		this.limit = limit;
	}

	void push(int value)
	{
		if (size == limit)
		{
			printf("队列满了，不能再加了\n");
			return;
		}
		size++;
		arr[pushi] = value;
		pushi = nextIndex(pushi);
	}

	public int pop()
	{
		if (size == 0)
		{
			printf("队列空了，不能再拿了\n");
			return;
		}
		size--;
		int ans = arr[polli];
		polli = nextIndex(polli);
		return ans;
	}

	bool isEmpty()
	{
		return size == 0;
	}

	// 如果现在的下标是i，返回下一个位置
	int nextIndex(int i)
	{
		return i < limit - 1 ? i + 1 : 0;
	}
private:
	int* arr;
	int pushi;// end
	int polli;// begin
	int size;
	const static int limit;

};
