#pragma once

namespace dataS
{
	typedef unsigned short uint16;

	template<typename Ty>
	struct Element
	{
		Ty data;

		Element<Ty>* element;

		Element(Ty _data, Element<Ty>* _element)
		{
			element = _element;
			data = _data;
		}
	};
}