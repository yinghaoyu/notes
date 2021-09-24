#include <climits>
#include <iostream>
#include <string>
class Node
{
  public:
    int value;
    Node* left;
    Node* right;
    Node(int data)
    {
      value = data;
      left = NULL;
      right = NULL;
    }
};

void process(Node* root)
{
  if (root == NULL)
  {
    return;
  }
  // 1
  process(root->left);
  // 2
  process(root->right);
  // 3
}

void morris(Node* head)
{
  if (head == NULL)
  {
    return;
  }
  Node* cur = head;
  Node* mostRight = NULL;
  while (cur != NULL)
  {
    mostRight = cur->left;
    if (mostRight != NULL)
    {
      while (mostRight->right != NULL && mostRight->right != cur)
      {
        mostRight = mostRight->right;
      }
      if (mostRight->right == NULL)
      {
        mostRight->right = cur;
        cur = cur->left;
        continue;
      }
      else
      {
        mostRight->right = NULL;
      }
    }
    cur = cur->right;
  }
}

void morrisPre(Node* head)
{
  if (head == NULL)
  {
    return;
  }
  Node* cur = head;
  Node* mostRight = NULL;
  while (cur != NULL)
  {
    mostRight = cur->left;
    if (mostRight != NULL)
    {
      while (mostRight->right != NULL && mostRight->right != cur)
      {
	mostRight = mostRight->right;
      }
      if (mostRight->right == NULL) 
      {
        std::cout<<cur->value<<" ";
	mostRight->right = cur;
	cur = cur->left;
	continue;
      }
      else
      {
	mostRight->right = NULL;
      }
      }
    else
    {
      std::cout<<cur->value<<" ";
    }
    cur = cur->right;
  }
  std::cout<<std::endl;
}

void morrisIn(Node* head)
{
  if (head == NULL)
  {
    return;
  }
  Node* cur = head;
  Node* mostRight = NULL;
  while (cur != NULL)
  {
    mostRight = cur->left;
    if (mostRight != NULL)
    {
      while (mostRight->right != NULL && mostRight->right != cur)
      {
        mostRight = mostRight->right;
      }
      if (mostRight->right == NULL) 
      {
        mostRight->right = cur;
	cur = cur->left;
	continue;
      }
      else
      {
	mostRight->right = NULL;
      }
    }
    std::cout<<cur->value<<" ";
    cur = cur->right;
  }
  std::cout<<std::endl;
}

Node* reverseEdge(Node* from)
{
  Node* pre = NULL;
  Node* next = NULL;
  while (from != NULL)
  {
    next = from->right;
    from->right = pre;
    pre = from;
    from = next;
  }
  return pre;
}

void printEdge(Node* head)
{
  Node* tail = reverseEdge(head);
  Node* cur = tail;
  while (cur != NULL)
  {
    std::cout<<cur->value<<" ";
    cur = cur->right;
  }
  reverseEdge(tail);
}

void morrisPos(Node* head)
{
  if (head == NULL)
  {
    return;
  }
  Node* cur = head;
  Node* mostRight = NULL;
  while (cur != NULL)
  {
    mostRight = cur->left;
    if (mostRight != NULL)
    {
      while (mostRight->right != NULL && mostRight->right != cur)
      {
	mostRight = mostRight->right;
      }
      if (mostRight->right == NULL)
      {
	mostRight->right = cur;
	cur = cur->left;
	continue;
      }
      else
      {
        mostRight->right = NULL;
	printEdge(cur->left);
      }
    }
    cur = cur->right;
  }
  printEdge(head);
  std::cout<<std::endl;
}

std::string getSpace(int num)
{
  std::string space = " ";
  std::string buf;
  for (int i = 0; i < num; i++)
  {
    buf.append(space);
  }
  return buf;
}

void printInOrder(Node* head, int height, std::string to, int len)
{
  if (head == NULL)
  {
    return;
  }
  printInOrder(head->right, height + 1, "v", len);
  std::string val = to + std::to_string(head->value) + to;
  int lenM = val.length();
  int lenL = (len - lenM) / 2;
  int lenR = len - lenM - lenL;
  val = getSpace(lenL) + val + getSpace(lenR);
  std::cout<<getSpace(height * len) + val << std::endl;
  printInOrder(head->left, height + 1, "^", len);
}

// for test -- print tree
void printTree(Node* head)
{
  std::cout<<"Binary Tree:"<<std::endl;
  printInOrder(head, 0, "H", 5);
  std::cout<<std::endl;
}

bool isBST(Node* head)
{
  if (head == NULL)
  {
    return true;
  }
  Node* cur = head;
  Node* mostRight = NULL;
  int pre = INT_MIN;
  bool ans = true;
  while (cur != NULL)
  {
    mostRight = cur->left;
    if (mostRight != NULL)
    {
      while (mostRight->right != NULL && mostRight->right != cur) 
      {
        mostRight = mostRight->right;
      }
      if (mostRight->right == NULL)
      {
        mostRight->right = cur;
	cur = cur->left;
	continue;
      }
      else
      {
        mostRight->right = NULL;
      }
    }
    if (pre != INT_MIN && pre >= cur->value)
    {
      ans = false;
    }
    pre = cur->value;
    cur = cur->right;
  }
  return ans;
}

int main(int argc, char* argv[])
{
  Node* head = new Node(4);
  head->left = new Node(2);
  head->right = new Node(6);
  head->left->left = new Node(1);
  head->left->right = new Node(3);
  head->right->left = new Node(5);
  head->right->right = new Node(7);
  printTree(head);
  morrisIn(head);
  morrisPre(head);
  morrisPos(head);
  printTree(head);
  return 0;
}
