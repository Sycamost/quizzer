#include "Flashcard.h"
#include "globals.h"
#include "util.h"

std::vector<Flashcard> Flashcard::_flashcardList = std::vector<Flashcard>();

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

std::vector<std::wstring> Flashcard::getTags()
{
	return _tags;
}

void Flashcard::write(std::wofstream& stream)
{
	stream << _front << L"\n";
	stream << _back << L"\n";
	for (unsigned int i = 0; i < _tags.size(); i++)
		stream << _tags[i] << L"\n";
	stream << L"\n";
}

std::vector<Flashcard> Flashcard::getFlashcardList()
{
	return _flashcardList;
}

void Flashcard::readFlashcardList()
{
	_flashcardList = std::vector<Flashcard>();

	try
	{
		std::wifstream file(Globals::flashcardsFileAddress);
		if (file.is_open())
		{
			while (!file.eof())
			{
				std::wstring front = getUserInputLine(file);

				if (file.eof()) break;
				std::wstring back = getUserInputLine(file);

				std::vector<std::wstring> tags = std::vector<std::wstring>();
				while (!file.eof())
				{
					std::wstring tag = getUserInputLine(file);
					if (tag == L"")
						break;
					tags.push_back(tag);
				}

				_flashcardList.push_back(Flashcard(front, back, tags));
			}
		}
	}
	catch (std::exception) {}
}

void Flashcard::appendFlashcardsToList(std::vector<Flashcard> flashcards)
{
	_flashcardList.insert(_flashcardList.end(), flashcards.begin(), flashcards.end());
}
