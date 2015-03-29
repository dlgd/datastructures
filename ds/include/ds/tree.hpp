#ifndef DATASTRUCTURES_TREE_HPP
#define DATASTRUCTURES_TREE_HPP

#include <cstdlib>
#include <memory>

namespace ds
{

namespace detail
{

template <typename NodeType>
struct node_trait_t
{
   using ptr_t = std::unique_ptr<NodeType>;
   using key_t = decltype(NodeType::m_key);
   using value_t = decltype(NodeType::m_value);
};

template<typename KeyType, typename ValueType, typename NodeType>
struct node_base_t
{
   node_base_t(NodeType* parent, const KeyType &key, const ValueType& value):
      m_key(key),
      m_value(value),
      m_parent(parent)
   {}

   KeyType m_key;
   ValueType m_value;
   NodeType* m_parent;
   typename node_trait_t<NodeType>::ptr_t m_left;
   typename node_trait_t<NodeType>::ptr_t m_right;
};

template<typename NodeType, typename LessType, typename ImplType>
class tree_t
{
public:
   using node_ptr_t = typename node_trait_t<NodeType>::ptr_t;
   using key_t = typename node_trait_t<NodeType>::key_t;
   using value_t = typename node_trait_t<NodeType>::value_t;

   tree_t(const LessType& less):
      m_less(less),
      m_impl(m_less)
   {}

   tree_t():
      m_impl(m_less)
   {}

   void put(const key_t& key, const value_t& value)
   {
      m_impl.put(m_root, key, value);
   }

   value_t* get(const key_t& key) const
   {
      const auto& n = _get(m_root, key);
      if (!n)
         return nullptr;
      return &n->m_value;
   }

   void remove(const key_t& key)
   {
      m_impl.remove(m_root, key);
   }

   std::size_t size() const
   {
      return _size(m_root);
   }

private:
   node_ptr_t m_root;
   LessType m_less;
   ImplType m_impl;

   const node_ptr_t& _get(const node_ptr_t& node, const key_t& key) const
   {
      if (!node)
         return node;

      if (m_less(key, node->m_key))
         return _get(node->m_left, key);

      if (m_less(node->m_key, key))
         return _get(node->m_right, key);

      return node;
   }

   std::size_t _size(const node_ptr_t& node) const
   {
      if (!node)
         return 0;
      return 1 + _size(node->m_left) + _size(node->m_right);
   }
};

}

}

#endif
