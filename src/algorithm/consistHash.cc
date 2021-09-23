#include <string>
#include <map>
#include <cstdint>
#include <sstream>
#include <iostream>

template<typename T>
std::string toString(const T & t) 
{
    std::ostringstream os;
    os<<t;
    return os.str();
}

template<typename Node, typename Data, typename Hash>
class ConsistentHash
{
public:
    ConsistentHash(uint32_t _virtual_nodes = 100) 
        : hash_(Hash()),
          virtual_nodes_(_virtual_nodes)
    {}

    void addNode(const Node & node)
    {
        std::string str_node = toString(node);
        for(uint32_t i = 0; i < virtual_nodes_; ++i)
        {
            std::string key = str_node + "_" + toString(i);
            uint64_t hash_value = hash_(key);
            unit_circle_.insert(std::make_pair(hash_value, node));
        }
    }

    void delNode(const Node & node)
    {
        std::string str_node = toString(node);
        for(uint32_t i = 0; i < virtual_nodes_; ++i)
        {
            std::string key = str_node + "_" + toString(i);
            uint64_t hash_value = hash_(key);
            unit_circle_.erase(hash_value);
        }
    }

    const Node * getNode(const Data & data) const
    {
        if(unit_circle_.empty())
            return nullptr;

        uint64_t hash_value = hash_(toString(data));
        auto itr = unit_circle_.lower_bound(hash_value);
        if(itr == unit_circle_.end())
            return &unit_circle_.begin()->second;
        return &itr->second;
    }
    void printBalance() const
    {
      std::map<uint64_t, uint64_t> distribute_map;
      uint64_t last_val = 0;
      for(auto & val : unit_circle_)
      {
        auto itr = unit_circle_.upper_bound(val.first);
        if(itr == unit_circle_.end())
        {
          distribute_map[val.second] += uint64_t(-1) - val.first + unit_circle_.begin()->first;
        }
        distribute_map[val.second] += itr->first - val.first;
      }
      for(auto & val : distribute_map)
      {
        std::cout<<val.first<<":"<<val.second<<":"<<val.second*1.0/uint64_t(-1)<<std::endl;
      }
    }

private:
    std::map<uint64_t, Node> unit_circle_;
    uint32_t virtual_nodes_;
    Hash hash_;
};

int main()
{
  uint32_t m, n;
  std::cout<<"input server number:";
  std::cin>>m;
  std::cout<<"input virtual number:";
  std::cin>>n;
  ConsistentHash<uint32_t, uint32_t, std::hash<std::string>> consistent_hash(n);
  for(uint32_t i = 1; i <= m; ++i)
  {
    consistent_hash.addNode(i);
  }
  std::cout<<"before delete node"<<std::endl;
  consistent_hash.printBalance();
  consistent_hash.delNode(3);
  std::cout<<"after delete node"<<std::endl;
  consistent_hash.printBalance();
  return 0;
}
