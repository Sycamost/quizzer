#include "TrueOrFalse.h"
#include "TrueOrFalseReader.h"
#include "QuestionReader.h"
#include "Option.h"
#include "QuestionType.h"
#include "globals.h"
#include "util.h"

namespace TrueOrFalseReader
{
	std::wstring prompt{ L"" };
	bool isTrue{ false };

	enum class Stage {
		PROMPT,
		IS_TRUE,
		TAGS
	} stage;

	void clear()
	{
		prompt = L"";
		isTrue = false;
	}

	void read(std::wstring line)
	{
		if (stage == Stage::PROMPT)
		{
			if (!line.empty())
			{
				prompt = line;
				stage = Stage::IS_TRUE;
			}
			return;
		}
		if (stage == Stage::IS_TRUE)
		{
			auto yesNo = getYesNo(line);
			if (yesNo || !yesNo)
			{
				isTrue = yesNo;
				stage = Stage::TAGS;
			}
			return;
		}
	}

	Question* construct(easy_list::list<Option> options, easy_list::list<std::wstring> tags)
	{
		if (stage != Stage::TAGS)
			return nullptr;
		return new TrueOrFalse(prompt, isTrue, tags);
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