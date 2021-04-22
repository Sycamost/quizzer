#include "Flashcard.h"
#include "FlashcardWriter.h"
#include "FlashcardReader.h"
#include "QuestionReader.h"
#include "Option.h"
#include "QuestionType.h"
#include "globals.h"
#include "util.h"

namespace FlashcardReader
{
	std::wstring front{ L"" };
	std::wstring back{ L"" };

	void clear()
	{
		front = L"";
		back = L"";
	}

	void read(std::wstring line)
	{
		if (front == L"")
			front = line;
		else if (back == L"")
			back = line;
		// if we get any extra inputs after front and back are filled, they are ignored
	}

	Question* construct(easy_list::list<Option> options, easy_list::list<std::wstring> tags)
	{
		if (front == L"")
			return nullptr;
		if (back == L"")
			return nullptr;
		bool caseSensitive = options.contains(toLower(Globals::optionCaseSensitive), &Option::getOption);
		return new Flashcard(front, back, caseSensitive, tags);
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