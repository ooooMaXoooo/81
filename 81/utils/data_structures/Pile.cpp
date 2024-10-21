#include "Pile.h"
#include <algorithm>

namespace dataS {
	
	template<typename Ty>
	Pile<Ty>::Pile()
		: m_size(1), m_current_element(nullptr)
	{
	}

	template<typename Ty>
	Pile<Ty>::~Pile()
	{
		clear();
	}

	template<typename Ty>
	void Pile<Ty>::remove()
	{
		if (size == 0)
			return;

		Element<Ty>* previous = m_current_element->element;
		delete m_current_element;

		m_current_element = previous;
		m_size--;
	}

	template<typename Ty>
	void Pile<Ty>::add(Ty data)
	{
		// if we point currently to nothing, we create a fresh element
		if (!m_current_element) // size = 0 ?
			m_current_element = new Element<Ty>(data, nullptr);

		else
			m_current_element = new Element<Ty>(data, m_current_element);

		m_size++;
	}

	template<typename Ty>
	Ty Pile<Ty>::get()
	{
		return m_current_element->data;
	}

	template<typename Ty>
	Ty Pile<Ty>::get_remove()
	{
		Ty data = get();
		remove();
		return data;
	}

	template<typename Ty>
	bool Pile<Ty>::contains(const Ty& data) const
	{				  
		Element<Ty>* p_elem = m_current_element;
					  
		while (p_elem && p_elem->data != data)
			p_elem = p_elem->element;
					  
		return p_elem->data == data;
	}

	template<typename Ty>
	void Pile<Ty>::clear()
	{
		while (m_current_element)
		{
			remove();
		}
	}

}
