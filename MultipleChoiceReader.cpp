#include "MultipleChoice.h"
#include "MultipleChoiceReader.h"
#include "QuestionReader.h"
#include "Option.h"
#include "QuestionType.h"
#include "globals.h"
#include "util.h"

namespace MultipleChoiceReader
{
	std::wstring question{ L"" };
	std::wstring correctAnswer{ L"" };
	easy_list::list<std::wstring> wrongAnswers{ {} };

	void clear()
	{
		question = L"";
		correctAnswer = L"";
		wrongAnswers = easy_list::list<std::wstring>();
	}

	void read(std::wstring line)
	{
		if (question == L"")
			question = line;
		else if (correctAnswer == L"")
			correctAnswer = line;
		else if (line != L"")
			wrongAnswers.push_back(line);
	}

	Question* construct(easy_list::list<Option> options, easy_list::list<std::wstring> tags)
	{
		if (question == L"")
			return nullptr;
		else if (correctAnswer == L"")
			return nullptr;
		return new MultipleChoice(question, correctAnswer, wrongAnswers, tags);
	}

	QuestionReader& get()
	{
		static QuestionReader reader = QuestionReader(
			&clear,
			&read,
			&construct
		);
		return reader;
	}
}