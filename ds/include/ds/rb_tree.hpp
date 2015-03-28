#ifndef DATASTRUCTURES_RB_TREE_HPP
#define DATASTRUCTURES_RB_TREE_HPP

#include <cassert>
#include <functional>
#include <utility>

#include "ds/tree.hpp"

namespace ds
{

namespace detail
{

template<typename NodeType, typename LessType>
struct rbt_impl_t
{
   using node_ptr_t = typename node_trait_t<NodeType>::ptr_t;
   using key_t = typename node_trait_t<NodeType>::key_t;
   using value_t = typename node_trait_t<NodeType>::value_t;

   rbt_impl_t(const LessType& less):
      m_less(less)
   {}

   void put(node_ptr_t& root, const key_t& key, const value_t& value) const
   {
      assert(is_sound(root));
      _put(root, key, value);
      root->m_color = NodeType::color_t::black;
      assert(is_sound(root));
   }

   void remove(node_ptr_t& root, const key_t& key) const
   {
      assert(is_sound(root));
      if (!is_red(root->m_left) && !is_red(root->m_right))
         root->m_color = NodeType::color_t::red;

      _remove(root, key);

      if (root)
         root->m_color = NodeType::color_t::black;
      assert(is_sound(root));
   }

private:
   LessType m_less;


   bool is_sound(const node_ptr_t& root) const
   {
      return is_node_sound(root) && is_balanced(root);
   }

   static bool is_node_sound(const node_ptr_t& h)
   {
      if (!h)
         return true;
      if (is_red(h->m_right))
         return false;
      if (is_red(h) && is_red(h->m_left))
         return false;
      return is_node_sound(h->m_left) && is_node_sound(h->m_right);
   }

   static bool is_balanced(const node_ptr_t& root)
   {
      return _is_balanced(root, get_nb_black_links(root, 0), 0);
   }

   static size_t get_nb_black_links(const node_ptr_t& h, size_t nb_black_links)
   {
      if (!h)
         return nb_black_links;
      return get_nb_black_links(h->m_left, is_red(h) ? 
                                nb_black_links + 1 : nb_black_links);
   }

   static bool _is_balanced(const node_ptr_t& h,
                            size_t expected_nb_black_links,
                            size_t nb_black_links)
   {
      if (!h)
         return nb_black_links == expected_nb_black_links;

      if (!is_red(h))
         ++nb_black_links;

      return _is_balanced(h->m_left, expected_nb_black_links, nb_black_links) &&
         _is_balanced(h->m_right, expected_nb_black_links, nb_black_links);
   }

   void _put(node_ptr_t& node, const key_t& key, const value_t& value) const
   {
      if (!node)
      {
         node = node_ptr_t(new NodeType);
         node->m_key = key;
         node->m_value = value;
         node->m_color = NodeType::color_t::red;
         return;
      }
	   
      if (m_less(key, node->m_key))
         _put(node->m_left, key, value);
      else if (m_less(node->m_key, key))
         _put(node->m_right, key, value);
      else
         node->m_value = value;

      if (is_red(node->m_right) && !is_red(node->m_left))
         rotate_left(node);

      if (is_red(node->m_left) && is_red(node->m_left->m_left))
         rotate_right(node);

      if (is_red(node->m_left) && is_red(node->m_right))
         flip_colors(node);
   }


   void _remove(node_ptr_t& h, const key_t& key) const
   {
      if (m_less(key, h->m_key))
      {
         assert(h->m_left);
         if (!is_red(h->m_left) && !is_red(h->m_left->m_left))
            move_red_left(h);

         _remove(h->m_left, key);
      }
      else
      {
         if (is_red(h->m_left))
            rotate_right(h);

         if (key_equal(key, h->m_key) && !h->m_right)
         {
            h.reset();
            return;
         }

         assert(h->m_right);
         if (!is_red(h->m_right) && !is_red(h->m_right->m_left))
            move_red_right(h);

         if (key_equal(key, h->m_key))
         {
            const auto& node_min = find_min(h->m_right);
            h->m_value = node_min->m_value;
            h->m_key = node_min->m_key;
            remove_min(h->m_right);
         }
         else
            _remove(h->m_right, key);
      }

      balance(h);
   }


   static void remove_min(node_ptr_t& h)
   {
      assert(h);
      if (!h->m_left)
      {
         h.reset();
         return;
      }

      assert(h->m_left);
      if (!is_red(h->m_left) && !is_red(h->m_left->m_left))
         move_red_left(h);

      remove_min(h->m_left);

      balance(h);
   }

   bool key_equal(const key_t& lhs, const key_t& rhs) const
   {
      return !m_less(lhs, rhs) && !m_less(rhs, lhs);
   }

   static void balance(node_ptr_t& h)
   {
      if (is_red(h->m_right))
         rotate_left(h);
      if (is_red(h->m_left) && is_red(h->m_left->m_left))
         rotate_right(h);
      if (is_red(h->m_left) && is_red(h->m_right))
         flip_colors(h);
   }

   static node_ptr_t& find_min(node_ptr_t& node)
   {
      if (!node || !node->m_left)
         return node;
      return find_min(node->m_left);
   }

   static void move_red_right(node_ptr_t& h)
   {
      flip_colors(h);
      assert(h->m_left);
      if (is_red(h->m_left->m_left))
      {
         rotate_right(h);
         flip_colors(h);
      }
   }

   static void move_red_left(node_ptr_t& h)
   {
      flip_colors(h);
      assert(h->m_right);
      if (is_red(h->m_right->m_left))
      {
         rotate_right(h->m_right);
         rotate_left(h);
         flip_colors(h);
      }
   }

   static bool is_red(const node_ptr_t& node)
   {
      if (!node)
         return false;
      return node->m_color == NodeType::color_t::red;
   }

   static void rotate_right(node_ptr_t& h)
   {
      node_ptr_t x = std::move(h->m_left);
      h->m_left = std::move(x->m_right);
      x->m_color = h->m_color;
      h->m_color = NodeType::color_t::red;
      x->m_right = std::move(h);
      h = std::move(x);
   }

   static void rotate_left(node_ptr_t& h)
   {
      node_ptr_t x = std::move(h->m_right);
      h->m_right = std::move(x->m_left);
      x->m_color = h->m_color;
      h->m_color = NodeType::color_t::red;
      x->m_left = std::move(h);
      h = std::move(x);
   }

   static typename NodeType::color_t 
   flip_color(typename NodeType::color_t color)
   {
      return color == NodeType::color_t::red ? 
         NodeType::color_t::black : NodeType::color_t::red;
   }

   static void flip_colors(node_ptr_t& h)
   {
      h->m_color = flip_color(h->m_color);
      assert(h->m_left);
      assert(h->m_right);
      h->m_left->m_color = flip_color(h->m_left->m_color);
      h->m_right->m_color = flip_color(h->m_right->m_color);
   }

};


template <typename KeyType, typename ValueType>
struct rbt_node_t: public node_base_t<KeyType, ValueType,
                                      rbt_node_t<KeyType, ValueType>>
{
   enum class color_t { red, black };
   color_t m_color = color_t::red;
};

}

template<typename KeyType, typename ValueType,
         typename LessType = std::less<KeyType>>
   using rb_tree_t =
   detail::tree_t<detail::rbt_node_t<KeyType, ValueType>, LessType,
                  detail::rbt_impl_t<detail::rbt_node_t<KeyType, ValueType>,
                                     LessType>>;

}

#endif
