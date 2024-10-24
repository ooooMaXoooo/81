#pragma once

#include "data_structures/Element.h"

namespace dataS
{
	template <typename Ty>
	class File
	{
	private :
		size_t m_size;

		Element<Ty>* m_current_element;
		Element<Ty>* m_first;

	public :
		File();

		~File();

		size_t size() const { return m_size; }

		void add(Ty data);
		Ty get();
		void remove();
		Ty get_remove();
		
		bool contains(const Ty& data) const;
		void clear();
	};
}