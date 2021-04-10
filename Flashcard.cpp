#include <algorithm>
#include "Flashcard.h"
#include "globals.h"
#include "util.h"
#include "Option.h"

const std::wstring Flashcard::_optCaseSensitive = L"case_sensitive";

Flashcard::Flashcard(std::wstring question, std::wstring answer, bool caseSensitive, std::vector<std::wstring> tags) :
	Question(QuestionType::FLASHCARD, tags),
	_question(question),
	_answer(answer),
	_caseSensitive(caseSensitive)
{
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
	return _caseSensitive ? (guess == _answer) : (toUpper(guess) == toUpper(_answer));
}

void Flashcard::writeChildData(std::wofstream& stream)
{
	if (_caseSensitive)
		Option(_optCaseSensitive).write(stream);
	stream << _question << L"\n";
	stream << _answer << L"\n";
}

bool Flashcard::isCaseSensitive()
{
	return _caseSensitive;
}

void Flashcard::setCaseSensitive()
{
	_caseSensitive = true;
}

void Flashcard::setCaseInsensitive()
{
	_caseSensitive = false;
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
				std::vector<Option> options = Option::readOptions(file);
				bool caseSensitive = std::find_if(options.begin(), options.end(), [](Option opt) -> bool {return opt.getOption() == _optCaseSensitive; }) != options.end();

				std::wstring question = getInputLine(file);

				if (file.eof()) break;
				std::wstring answer = getInputLine(file);

				std::vector<std::wstring> tags = std::vector<std::wstring>();
				while (!file.eof())
				{
					std::wstring tag = getInputLine(file);
					if (tag == L"")
						break;
					tags.push_back(tag);
				}

				_flashcardList.push_back(new Flashcard(question, answer, caseSensitive, tags));
			}
		}
		file.close();
	}
	catch (std::exception) {}

	std::wcout << "\tRead " << _flashcardList.size() << " flashcards from file...\n";

	return _flashcardList;
}
