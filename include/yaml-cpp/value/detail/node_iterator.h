#ifndef VALUE_DETAIL_NODE_ITERATOR_H_62B23520_7C8E_11DE_8A39_0800200C9A66
#define VALUE_DETAIL_NODE_ITERATOR_H_62B23520_7C8E_11DE_8A39_0800200C9A66

#if defined(_MSC_VER) || (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || (__GNUC__ >= 4)) // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif


#include "yaml-cpp/dll.h"
#include "yaml-cpp/value/ptr.h"
#include <boost/iterator/iterator_facade.hpp>
#include <boost/utility/enable_if.hpp>
#include <list>
#include <utility>
#include <vector>

namespace YAML
{
	namespace detail
	{
		struct iterator_type { enum value { None, Sequence, Map }; };
		
		template<typename V>
		struct node_iterator_value {
			node_iterator_value(): pNode(0), pKey(0), pValue(0) {}
			explicit node_iterator_value(V& rhs): pNode(&rhs), pKey(0), pValue(0) {}
			explicit node_iterator_value(V& key, V& value): pNode(0), pKey(&key), pValue(&value) {}
			
			V *pNode;
			V *pKey, *pValue;
		};
		
		typedef std::vector<node *> node_seq;
		typedef std::pair<node *, node *> kv_pair;
		typedef std::list<kv_pair> node_map;
		
		template<typename V>
		struct node_iterator_type {
			typedef node_seq::iterator seq;
			typedef node_map::iterator map;
		};
		
		template<typename V>
		struct node_iterator_type<const V> {
			typedef node_seq::const_iterator seq;
			typedef node_map::const_iterator map;
		};
		

		template<typename V>
		class node_iterator_base: public boost::iterator_facade<
		node_iterator_base<V>,
		node_iterator_value<V>,
		std::bidirectional_iterator_tag,
		node_iterator_value<V> >
		{
		private:
			struct enabler {};
			
		public:
			typedef typename node_iterator_type<V>::seq SeqIter;
			typedef typename node_iterator_type<V>::map MapIter;
			typedef node_iterator_value<V> value_type;
			
			node_iterator_base(): m_type(iterator_type::None) {}
			explicit node_iterator_base(SeqIter seqIt): m_type(iterator_type::Sequence), m_seqIt(seqIt) {}
			explicit node_iterator_base(MapIter mapIt): m_type(iterator_type::Map), m_mapIt(mapIt) {}
			
			template<typename W>
			node_iterator_base(const node_iterator_base<W>& rhs, typename boost::enable_if<boost::is_convertible<W*, V*>, enabler>::type = enabler())
			: m_type(rhs.m_type), m_seqIt(rhs.m_seqIt), m_mapIt(rhs.m_mapIt) {}
			
		private:
			friend class boost::iterator_core_access;
			template<typename> friend class node_iterator_base;
			
			template<typename W>
			bool equal(const node_iterator_base<W>& rhs) const {
				if(m_type != rhs.m_type)
					return false;
				
				switch(m_type) {
					case iterator_type::None: return true;
					case iterator_type::Sequence: return m_seqIt == rhs.m_seqIt;
					case iterator_type::Map: return m_mapIt == rhs.m_mapIt;
				}
				return true;
			}
			
			void increment() {
				switch(m_type) {
					case iterator_type::None: break;
					case iterator_type::Sequence: ++m_seqIt; break;
					case iterator_type::Map: ++m_mapIt; break;
				}
			}
			
			void decrement() {
				switch(m_type) {
					case iterator_type::None: break;
					case iterator_type::Sequence: --m_seqIt; break;
					case iterator_type::Map: --m_mapIt; break;
				}
			}
			
			value_type dereference() const {
				switch(m_type) {
					case iterator_type::None: return value_type();
					case iterator_type::Sequence: return value_type(**m_seqIt);
					case iterator_type::Map: return value_type(*m_mapIt->first, *m_mapIt->second);
				}
				return V();
			}
			
		private:
			typename iterator_type::value m_type;

			SeqIter m_seqIt;
			MapIter m_mapIt;
		};

		typedef node_iterator_base<node> node_iterator;
		typedef node_iterator_base<const node> const_node_iterator;
	}
}

#endif // VALUE_DETAIL_NODE_ITERATOR_H_62B23520_7C8E_11DE_8A39_0800200C9A66
