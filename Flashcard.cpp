#include "Flashcard.h"
#include "globals.h"
#include "util.h"

Flashcard::Flashcard(std::wstring question, std::wstring answer, std::vector<std::wstring> tags)
	: Question(tags)
{
	_question = question;
	_answer = answer;
}

std::wstring Flashcard::getQuestion()
{
	return _question;
}

std::wstring Flashcard::getAnswer()
{
	return _answer;
}

bool Flashcard::isCorrect(std::wstring guess)
{
	return (toUpper(guess) == toUpper(_answer));
}

void Flashcard::write(std::wofstream& stream)
{
	stream << _question << L"\n";
	stream << _answer << L"\n";
	for (unsigned int i = 0; i < _tags.size(); i++)
		stream << _tags[i] << L"\n";
	stream << L"\n";
}

std::vector<Question*> Flashcard::readFlashcardList()
{
	std::vector<Question*> _flashcardList = std::vector<Question*>();

	try
	{
		std::wifstream file(Globals::flashcardsFileAddress);
		if (file.is_open())
		{
			while (!file.eof())
			{
				std::wstring question = getUserInputLine(file);

				if (file.eof()) break;
				std::wstring answer = getUserInputLine(file);

				std::vector<std::wstring> tags = std::vector<std::wstring>();
				while (!file.eof())
				{
					std::wstring tag = getUserInputLine(file);
					if (tag == L"")
						break;
					tags.push_back(tag);
				}

				_flashcardList.push_back(new Flashcard(question, answer, tags));
			}
		}
		file.close();
	}
	catch (std::exception) {}

	std::wcout << "\tRead " << _flashcardList.size() << " flashcards from file...\n";

	return _flashcardList;
}
