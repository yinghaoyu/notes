#include <iostream>
#include <vector>
#include <string>
using namespace std;

template<typename K, typename V>
class SkipListNode
{
public:
  K key;
  V val;
  std::vector<SkipListNode<K, V>*> nextNodes;

  SkipListNode(K k, V v)
  {
    key = k;
    val = v;
  }
  // 遍历的时候，如果是往右遍历到的null(next == null), 遍历结束
  // 头(null), 头节点的null，认为最小
  // node  -> 头，node(null, "")  node.isKeyLess(!null)  true
  // node里面的key是否比otherKey小，true，不是false
  bool isKeyLess(K otherKey)
  {
    //  otherKey == null -> false 
    return otherKey != "" && (key == "" || key < otherKey);
  }
  
  bool isKeyEqual(K otherKey)
  {
    return (key == "" && otherKey == "")
	    || (key != "" && otherKey != "" && key == otherKey);
  }

};

template<typename K, typename V>
class SkipListMap
{
public:
  static const double PROBABILITY; // < 0.5 继续做，>=0.5 停
  SkipListNode<K, V>* head;
  int size;
  int maxLevel;
  SkipListMap()
  {
    head = new SkipListNode<K, V>("", "");
    head->nextNodes.push_back(NULL); // 0
    size = 0;
    maxLevel = 0;
  }
  // 从最高层开始，一路找下去，
  // // 最终，找到第0层的<key的最右的节点
  SkipListNode<K, V>* mostRightLessNodeInTree(K key)
  {
    if (key == "")
    {
      return NULL;
    }
    int level = maxLevel;
    SkipListNode<K, V>* cur = head;
    while (level >= 0)
    {
      // 从上层跳下层
      //  cur  level  -> level-1
      cur = mostRightLessNodeInLevel(key, cur, level--);
    }
    return cur;
  }

  // 在level层里，如何往右移动
  // 现在来到的节点是cur，来到了cur的level层，在level层上，找到<key最后一个节点并返回
  SkipListNode<K, V>* mostRightLessNodeInLevel(K key, 
				SkipListNode<K, V>* cur, 
				int level)
  {
    SkipListNode<K, V>* next = cur->nextNodes.at(level);
    while (next != NULL && next->isKeyLess(key))
    {
      cur = next;
      next = cur->nextNodes.at(level);
    }
    return cur;
  }

  bool containsKey(K key)
  {
    if (key == "")
    {
      return false;
    }
    SkipListNode<K, V>* less = mostRightLessNodeInTree(key);
    SkipListNode<K, V>* next = less->nextNodes.at(0);
    return next != NULL && next->isKeyEqual(key);
  }

  // 新增、改value
  void put(K key, V value)
  {
    if (key == "")
    {
      return;
    }
    // 0层上，最右一个，< key 的Node -> >key
    SkipListNode<K, V>* less = mostRightLessNodeInTree(key);
    SkipListNode<K, V>* find = less->nextNodes.at(0);
    if (find != NULL && find->isKeyEqual(key))
    {
      find->val = value;
    }
    else
    {
      // find == null   8   7   9
      size++;
      int newNodeLevel = 0;
      while (random()&&0xFFFF < PROBABILITY*0xFFFF)  //from redis
      {
	newNodeLevel++;
      }
      // newNodeLevel
      while (newNodeLevel > maxLevel)
      {
        head->nextNodes.push_back(NULL);
	maxLevel++;
      }
      SkipListNode<K, V>* newNode = new SkipListNode<K, V>(key, value);
      for (int i = 0; i <= newNodeLevel; i++)
      {
        newNode->nextNodes.push_back(NULL);
      }
      int level = maxLevel;
      SkipListNode<K, V>* pre = head;
      while (level >= 0)
      {
	// level 层中，找到最右的 < key 的节点
	pre = mostRightLessNodeInLevel(key, pre, level);
	if (level <= newNodeLevel)
        {
	  newNode->nextNodes[level] = pre->nextNodes.at(level);
	  pre->nextNodes[level] =  newNode;
	}
	level--;
      }
    }
  }

  V get(K key)
  {
    if (key == "")
    {
      return "";
    }
    SkipListNode<K, V>* less = mostRightLessNodeInTree(key);
    SkipListNode<K, V>* next = less->nextNodes.get(0);
    return next != NULL && next->isKeyEqual(key) ? next->val : "";
  }

  void remove(K key)
  {
    if (containsKey(key))
    {
      size--;
      int level = maxLevel;
      SkipListNode<K, V>* pre = head;
      while (level >= 0)
      {
	pre = mostRightLessNodeInLevel(key, pre, level);
	SkipListNode<K, V>* next = pre->nextNodes.at(level);
	// 1）在这一层中，pre下一个就是key
        // 2）在这一层中，pre的下一个key是>要删除key
        if (next != NULL && next->isKeyEqual(key))
        {
	  // free delete node memory -> C++
	  // level : pre -> next(key) -> ...
	  pre->nextNodes[level] = next->nextNodes.at(level);
	}
	// 在level层只有一个节点了，就是默认节点head
	if (level != 0 && pre == head && pre->nextNodes.at(level) == NULL)
        {
          head->nextNodes.erase(head->nextNodes.begin() + level - 1);
	  maxLevel--;
	}
	level--;
      }
    }
  }
  
  K firstKey()
  {
    return head->nextNodes.at(0) != NULL ? head->nextNodes.at(0)->key : "";
  }
  
  K lastKey()
  {
    int level = maxLevel;
    SkipListNode<K, V>* cur = head;
    while (level >= 0)
    {
      SkipListNode<K, V>* next = cur->nextNodes.at(level);
      while (next != NULL)
      {
        cur = next;
	next = cur->nextNodes.at(level);
      }
      level--;
    }
    return cur->key;
  }
  
  K ceilingKey(K key)
  {
    if (key == "")
    {
      return "";
    }
    SkipListNode<K, V>* less = mostRightLessNodeInTree(key);
    SkipListNode<K, V>* next = less->nextNodes.at(0);
    return next != NULL ? next->key : "";
  }
  
  K floorKey(K key)
  {
    if (key == "")
    {
      return "";
    }
    SkipListNode<K, V>* less = mostRightLessNodeInTree(key);
    SkipListNode<K, V>* next = less->nextNodes.at(0);
    return next != NULL && next->isKeyEqual(key) ? next->key : less->key;
  }
  
 // int size()
 //{
 //  return size;
 //}
};

// for test
void printAll(SkipListMap<std::string, std::string> obj)
{
  for (int i = obj.maxLevel; i >= 0; i--)
  {
    std::cout<< "Level " << i<< " : ";
    SkipListNode<std::string, std::string>* cur = obj.head;
    while (cur->nextNodes.at(i) != NULL)
    {
      SkipListNode<std::string, std::string>* next = cur->nextNodes.at(i);
      std::cout<< "(" << next->key << " , " << next->val << ") ";
      cur = next;
    }
    std::cout<<std::endl;
  }
}

template<typename K, typename V>
const double SkipListMap<K, V>::PROBABILITY = 0.5; // < 0.5 继续做，>=0.5 停

int main(int argc, char* argv[])
{
  SkipListMap<std::string, std::string> test;
  printAll(test);
  std::cout<<"======================"<<std::endl;
  test.put("A", "10");
  printAll(test);
  std::cout<<"======================"<<std::endl;
  test.remove("A");
  printAll(test);
  std::cout<<"======================"<<std::endl;
  test.put("E", "E");
  test.put("B", "B");
  test.put("A", "A");
  test.put("F", "F");
  test.put("C", "C");
  test.put("D", "D");
  printAll(test);
  std::cout<<"======================"<<std::endl;
  std::cout<<test.containsKey("B")<<std::endl;
  std::cout<<test.containsKey("Z")<<std::endl;
  std::cout<<test.firstKey()<<std::endl;
  std::cout<<test.lastKey()<<std::endl;
  std::cout<<test.floorKey("D")<<std::endl;
  std::cout<<test.ceilingKey("D")<<std::endl;
  std::cout<<"======================"<<std::endl;
  test.remove("D");
  printAll(test);
  std::cout<<"======================"<<std::endl;
  std::cout<<test.floorKey("D")<<std::endl;
  std::cout<<test.ceilingKey("D")<<std::endl;
  return 0;
}
