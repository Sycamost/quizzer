#include "Flashcard.h"

Flashcard::Flashcard(std::wstring front, std::wstring back, std::vector<std::wstring> tags)
{
	_front = front;
	_back = back;
	_tags = tags;
}

std::wstring Flashcard::getFront()
{
	return _front;
}

std::wstring Flashcard::getBack()
{
	return _back;
}

std::vector<std::wstring>::iterator Flashcard::getTags()
{
	return _tags.begin();;
}

void Flashcard::write(std::wofstream& stream)
{
	stream << _front << L"\n";
	stream << _back << L"\n";
	for (int i = 0; i < _tags.size(); i++)
		stream << _tags[i] << L"\n";
	stream << L"\n";
}
