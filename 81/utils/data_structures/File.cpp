#include "File.h"
#include <algorithm>

namespace dataS {

	template<typename Ty>
	File<Ty>::File()
		: m_size(1), m_current_element(nullptr), m_first(nullptr)
	{
	}


	template<typename Ty>
	File<Ty>::~File()
	{
		clear();
		// both pointers will point to nullptr
	}

	template<typename Ty>
	void File<Ty>::remove()
	{
		if (size == 0)
			return;

		Element<Ty>* next = m_first->element;
		delete m_first;

		if (m_size == 1) // when m_size = 1,       next = nullptr  so  m_first will equal nullptr too
		{
			m_current_element = nullptr;
		}

		m_first = next;
		m_size--;
	}

	template<typename Ty>
	void File<Ty>::add(Ty data)
	{
		// if we point currently to nothing, we create a fresh element
		if (!m_current_element) // size = 0 ?
		{
			m_current_element = new Element<Ty>(data, nullptr);
			m_first = m_current_element;
		}
		/*else if (!m_first)
		{
			// better never be in this case !
		}*/
		else
		{
			// the current element point to a fresh element
			// we keep the younger element pointing to nullptr
			m_current_element->element = new Element<Ty>(data, nullptr);

			//we update the pointer to our younger element to update the data structure
			m_current_element = m_current_element->element;
		}

		m_size++;
	}

	template<typename Ty>
	Ty File<Ty>::get()
	{
		return m_first->data;
	}

	template<typename Ty>
	Ty File<Ty>::get_remove()
	{
		Ty data = get();
		remove();
		return data;
	}

	template<typename Ty>
	bool File<Ty>::contains(const Ty& data) const
	{
		Element<Ty>* p_elem = m_first;

		while (p_elem && p_elem->data != data)
			p_elem = p_elem->element;

		return p_elem->data == data;
	}

	template<typename Ty>
	void File<Ty>::clear()
	{
		while (m_first)
		{
			remove();
		}
	}
}
