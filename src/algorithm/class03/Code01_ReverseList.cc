typedef struct Node
{
	int value;
	Node* next;
}Node;

typedef struct DoubleNode
{
	int value;
	DoubleNode* last;
	DoubleNode* next;
}DoubleNode;

//  head
//   a    ->   b    ->  c  ->  null
//   c    ->   b    ->  a  ->  null
Node* reverseLinkedList(Node* head) {
	Node* pre = null;
	Node* next = null;
	while (head != null) {
		next = head->next;
		head->next = pre;
		pre = head;
		head = next;
	}
	return pre;
}

DoubleNode* reverseDoubleList(DoubleNode* head) {
	DoubleNode* pre = null;
	DoubleNode* next = null;
	while (head != null) {
		next = head->next;
		head->next = pre;
		head->last = next;
		pre = head;
		head = next;
	}
	return pre;
}