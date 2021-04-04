#include "Flashcard.h"

Flashcard::Flashcard(std::wstring front, std::wstring back, std::vector<std::wstring> tags = std::vector<std::wstring>())
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