#ifndef DATASTRUCTURES_BS_TREE_HPP
#define DATASTRUCTURES_BS_TREE_HPP

#include <functional>
#include <utility>

#include "ds/tree.hpp"

namespace ds
{

namespace detail
{

template<typename NodeType, typename LessType>
struct bst_impl_t
{
   using node_ptr_t = typename node_trait_t<NodeType>::ptr_t;
   using key_t = typename node_trait_t<NodeType>::key_t;
   using value_t = typename node_trait_t<NodeType>::value_t;

   bst_impl_t(const LessType& less):
      m_less(less)
   {}

   void put(node_ptr_t& node, const key_t& key, const value_t& value) const
   {
      if (!node)
      {
         node = node_ptr_t(new NodeType);
         node->m_key = key;
         node->m_value = value;
      }
      else if (m_less(key, node->m_key))
      {
         put(node->m_left, key, value);
      }
      else if (m_less(node->m_key, key))
      {
         put(node->m_right, key, value);
      }
      else
      {
         node->m_value = value;
      }
   }

   void remove(node_ptr_t& node, const key_t& key) const
   {
      if (!node)
         return;

      if (m_less(key, node->m_key))
         remove(node->m_left, key);
      else if (m_less(node->m_key, key))
         remove(node->m_right, key);
      else
         remove_node(node);
   }

private:
   LessType m_less;

   static void remove_node(node_ptr_t& node)
   {
      if (!node->m_right)
      {
         auto tmp = std::move(node);
         node = std::move(tmp->m_left);
      }
      else if (!node->m_left)
      {
         auto tmp = std::move(node);
         node = std::move(tmp->m_right);
      }
      else
      {
         auto node_min = std::move(find_min(node->m_right));
         node_min->m_left = std::move(node->m_left);
         node_min->m_right = std::move(node->m_right);
         node = std::move(node_min);
      }
   }

   static node_ptr_t& find_min(node_ptr_t& node)
   {
      if (!node || !node->m_left)
         return node;
      return find_min(node->m_left);
   }
};


template <typename KeyType, typename ValueType>
struct bst_node_t: public node_base_t<KeyType, ValueType,
                                      bst_node_t<KeyType, ValueType>>
{
};

}

template<typename KeyType, typename ValueType,
          typename LessType = std::less<KeyType>>
using bs_tree_t = 
   detail::tree_t<detail::bst_node_t<KeyType, ValueType>, LessType,
                  detail::bst_impl_t<detail::bst_node_t<KeyType, ValueType>,
                                     LessType>>;

}

#endif
