template<typename T>
class Node
{
public:
	T value;
	Node<T>* last;
	Node<T>* next;
	Node(T data)
	{
		value = data;
	}
};

template<typename T>
class DoubleEndsQueue
{
public:
	Node<T>* head;
	Node<T>* tail;

	void addFromHead(T value)
	{
		Node<T>* cur = new Node<T>(value);
		if (head == null)
		{
			head = cur;
			tail = cur;
		}
		else
		{
			cur->next = head;
			head->last = cur;
			head = cur;
		}
	}

	void addFromBottom(T value) {
		Node<T>* cur = new Node<T>(value);
		if (head == null)
		{
			head = cur;
			tail = cur;
		}
		else
		{
			cur->last = tail;
			tail->next = cur;
			tail = cur;
		}
	}

	T popFromHead()
	{
		if (head == null)
		{
			return null;
		}
		Node<T>* cur = head;
		if (head == tail)
		{
			head = null;
			tail = null;
		}
		else
		{
			head = head->next;
			cur->next = null;
			head->last = null;
		}
		return cur->value;
	}

	T popFromBottom()
	{
		if (head == null)
		{
			return null;
		}
		Node<T>* cur = tail;
		if (head == tail)
		{
			head = null;
			tail = null;
		}
		else
		{
			tail = tail->last;
			tail->next = null;
			cur->last = null;
		}
		return cur->value;
	}

	public boolean isEmpty()
	{
		return head == null;
	}

}

template<typename T>
class MyStack
{
public:
	MyStack()
	{
		queue = new DoubleEndsQueue<T>();
	}

	public void push(T value)
	{
		queue.addFromHead(value);
	}

	public T pop()
	{
		return queue.popFromHead();
	}

	public boolean isEmpty()
	{
		return queue->isEmpty();
	}
private:
	DoubleEndsQueue<T>* queue;

}

class MyQueue<T>
{
public:
	MyQueue()
	{
		queue = new DoubleEndsQueue<T>();
	}

	void push(T value)
	{
		queue.addFromHead(value);
	}

	T poll()
	{
		return queue.popFromBottom();
	}

	boolean isEmpty()
	{
		return queue.isEmpty();
	}
private:
	DoubleEndsQueue<T>* queue;
}
