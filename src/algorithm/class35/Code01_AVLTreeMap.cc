#include <iostream>
#include <string>
template<typename K, typename V>
class AVLNode
{
  public:
    K k;
    V v;
    AVLNode<K, V>* l;
    AVLNode<K, V>* r;
    int h;

    AVLNode(K key, V value)
    {
      k = key;
      v = value;
      h = 1;
    }
};

template<typename K, typename V>
class AVLTreeMap
{
  public:
    AVLNode<K, V>* root;
    int size;

    AVLTreeMap()
    {
      root = NULL;
      size = 0;
    }

  private:
    AVLNode<K, V>* rightRotate(AVLNode<K, V>* cur)
    {
      AVLNode<K, V>* left = cur->l;
      cur->l = left->r;
      left->r = cur;
      cur->h = std::max((cur->l != NULL ? cur->l->h : 0), (cur->r != NULL ? cur->r->h : 0)) + 1;
      left->h = std::max((left->l != NULL ? left->l->h : 0), (left->r != NULL ? left->r->h : 0)) + 1;
      return left;
    }

    AVLNode<K, V>* leftRotate(AVLNode<K, V>* cur)
    {
      AVLNode<K, V>* right = cur->r;
      cur->r = right->l;
      right->l = cur;
      cur->h = std::max((cur->l != NULL ? cur->l->h : 0), (cur->r != NULL ? cur->r->h : 0)) + 1;
      right->h = std::max((right->l != NULL ? right->l->h : 0), (right->r != NULL ? right->r->h : 0)) + 1;
      return right;
    }

    AVLNode<K, V>* maintain(AVLNode<K, V>* cur) {
      if (cur == NULL) {
        return NULL;
      }
      int leftHeight = cur->l != NULL ? cur->l->h : 0;
      int rightHeight = cur->r != NULL ? cur->r->h : 0;
      if (abs(leftHeight - rightHeight) > 1) {
        if (leftHeight > rightHeight) {
          int leftLeftHeight = cur->l != NULL && cur->l->l != NULL ? cur->l->l->h : 0;
          int leftRightHeight = cur->l != NULL && cur->l->r != NULL ? cur->l->r->h : 0;
          if (leftLeftHeight >= leftRightHeight) {
            cur = rightRotate(cur);
          } else {
            cur->l = leftRotate(cur->l);
            cur = rightRotate(cur);
          }
        } else {
          int rightLeftHeight = cur->r != NULL && cur->r->l != NULL ? cur->r->l->h : 0;
          int rightRightHeight = cur->r != NULL && cur->r->r != NULL ? cur->r->r->h : 0;
          if (rightRightHeight >= rightLeftHeight) {
            cur = leftRotate(cur);
          } else {
            cur->r = rightRotate(cur->r);
            cur = leftRotate(cur);
          }
        }
      }
      return cur;
    }

    AVLNode<K, V>* findLastIndex(K key) {
      AVLNode<K, V>* pre = root;
      AVLNode<K, V>* cur = root;
      while (cur != NULL) {
        pre = cur;
        if (key == cur->k) {
          break;
        } else if (key < cur->k) {
          cur = cur->l;
        } else {
          cur = cur->r;
        }
      }
      return pre;
    }

    AVLNode<K, V>* findLastNoSmallIndex(K key) {
      AVLNode<K, V>* ans = NULL;
      AVLNode<K, V>* cur = root;
      while (cur != NULL) {
        if (key == cur->k) {
          ans = cur;
          break;
        } else if (key < cur->k) {
          ans = cur;
          cur = cur->l;
        } else {
          cur = cur->r;
        }
      }
      return ans;
    }

    AVLNode<K, V>* findLastNoBigIndex(K key) {
      AVLNode<K, V>* ans = NULL;
      AVLNode<K, V>* cur = root;
      while (cur != NULL) {
        if (key == cur.k) {
          ans = cur;
          break;
        } else if (key < cur.k) {
          cur = cur->l;
        } else {
          ans = cur;
          cur = cur->r;
        }
      }
      return ans;
    }

    AVLNode<K, V>* add(AVLNode<K, V>* cur, K key, V value) {
      if (cur == NULL) {
        return new AVLNode<K, V>(key, value);
      } else {
        if (key < cur->k) {
          cur->l = add(cur->l, key, value);
        } else {
          cur->r = add(cur->r, key, value);
        }
        cur->h = std::max(cur->l != NULL ? cur->l->h : 0, cur->r != NULL ? cur->r->h : 0) + 1;
        return maintain(cur);
      }
    }

    // 在cur这棵树上，删掉key所代表的节点
    // 返回cur这棵树的新头部
    AVLNode<K, V>* del(AVLNode<K, V>* cur, K key) {
      if (key > cur.k) {
        cur->r = del(cur->r, key);
      } else if (key < cur->k) {
        cur->l = del(cur->l, key);
      } else {
        if (cur->l == NULL && cur->r == NULL) {
          cur = NULL;
        } else if (cur->l == NULL && cur->r != NULL) {
          cur = cur->r;
        } else if (cur->l != NULL && cur->r == NULL) {
          cur = cur->l;
        } else {
          AVLNode<K, V>* des = cur->r;
          while (des->l != NULL) {
            des = des->l;
          }
          cur->r = del(cur->r, des->k);
          des->l = cur->l;
          des->r = cur->r;
          cur = des;
        }
      }
      if (cur != NULL) {
        cur->h = std::max(cur->l != NULL ? cur->l->h : 0, cur->r != NULL ? cur->r->h : 0) + 1;
      }
      return maintain(cur);
    }

    int size() {
      return size;
    }

    bool containsKey(K key) {
      if (key == "") {
        return false;
      }
      AVLNode<K, V>* lastNode = findLastIndex(key);
      return lastNode != NULL && key == lastNode->k ? true : false;
    }

    void put(K key, V value) {
      if (key == "") {
        return;
      }
      AVLNode<K, V>* lastNode = findLastIndex(key);
      if (lastNode != NULL && key == lastNode->k) {
        lastNode->v = value;
      } else {
        size++;
        root = add(root, key, value);
      }
    }

    void remove(K key) {
      if (key == "") {
        return;
      }
      if (containsKey(key)) {
        size--;
        root = del(root, key);
      }
    }

    V get(K key) {
      if (key == "") {
        return "";
      }
      AVLNode<K, V>* lastNode = findLastIndex(key);
      if (lastNode != NULL && key == lastNode->k) {
        return lastNode->v;
      }
      return "";
    }

    K firstKey() {
      if (root == "") {
        return "";
      }
      AVLNode<K, V>* cur = root;
      while (cur->l != NULL) {
        cur = cur->l;
      }
      return cur->k;
    }

    K lastKey() {
      if (root == NULL) {
        return "";
      }
      AVLNode<K, V>* cur = root;
      while (cur->r != NULL) {
        cur = cur->r;
      }
      return cur->k;
    }

    K floorKey(K key) {
      if (key == "") {
        return "";
      }
      AVLNode<K, V>* lastNoBigNode = findLastNoBigIndex(key);
      return lastNoBigNode == NULL ? "" : lastNoBigNode->k;
    }

    K ceilingKey(K key) {
      if (key == "") {
        return "";
      }
      AVLNode<K, V>* lastNoSmallNode = findLastNoSmallIndex(key);
      return lastNoSmallNode == NULL ? "" : lastNoSmallNode->k;
    }

};

int main(int argc, char* argv[])
{
  return 0;
}
