typedef struct Node
{
	int value;
	Node* next;
}Node;

// head = removeValue(head, 2);
Node* removeValue(Node* head, int num)
{
	// head来到第一个不需要删的位置，此处为了减少常数时间
	while (head != null)
	{
		if (head->value != num)
		{
			break;
		}
		head = head->next;
	}
	// 1 ) head == null
	// 2 ) head != null
	Node* pre = head;
	Node* cur = head;
	while (cur != null)
	{
		if (cur->value == num)
		{
			pre->next = cur->next;
		}
		else
		{
			pre = cur;
		}
		cur = cur->next;
	}
	return head;
}
