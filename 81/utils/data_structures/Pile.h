#pragma once

#include "data_structures/Element.h"

namespace dataS
{
	template <typename Ty>
	class Pile
	{
	private :
		size_t m_size = 0;

		Element<Ty>* m_current_element;


	public :
		Pile();

		~Pile();

		size_t size() const { return m_size; }

		void add(Ty data);
		Ty get();
		void remove();
		Ty get_remove();

		bool contains(const Ty& data) const;
		void clear();
	};
}