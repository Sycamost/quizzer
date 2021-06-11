#include "SweetieJar.h"
#include "SweetieJarReader.h"
#include "QuestionReader.h"
#include "Option.h"
#include "QuestionType.h"
#include "globals.h"
#include "util.h"

namespace SweetieJarReader
{
	std::wstring question{ L"" };
	long double number{ 0.l };
	long double accuracy{ 0.l };
	size_t sigFigsOrDecimalPoints{ 0ull };
	size_t leadingZeroes{ 0ull };
	bool displayAsExp{ false };
	bool decimalPoints{ false };

	enum class Stage {
		QUESTION,
		NUMBER,
		ACCURACY,
		SIG_FIGS_OR_DECIMAL_POINTS,
		ZEROES,
		IS_EXP,
		IS_DECIMAL_POINTS,
		NONE
	} stage;

	void clear()
	{
		question = L"";
		number = 0.l;
		accuracy = 0.l;
		sigFigsOrDecimalPoints = 0ull;
		leadingZeroes = 0ull;
		displayAsExp = false;
		decimalPoints = false;
		stage = Stage::QUESTION;
	}

	void read(std::wstring line)
	{
		if (stage == Stage::QUESTION)
		{
			if (line != L"")
			{
				question = line;
				stage = Stage::NUMBER;
			}
			return;
		}
		else if (stage == Stage::NUMBER)
		{
			if (interpretLongDouble(line, &number))
				stage = Stage::ACCURACY;
			return;
		}
		else if (stage == Stage::ACCURACY)
		{
			if (interpretLongDouble(line, &accuracy))
				stage = Stage::SIG_FIGS_OR_DECIMAL_POINTS;
			return;
		}
		else if (stage == Stage::SIG_FIGS_OR_DECIMAL_POINTS)
		{
			if (interpretSize(line, &sigFigsOrDecimalPoints))
				stage = Stage::ZEROES;
			return;
		}
		else if (stage == Stage::ZEROES)
		{
			if (interpretSize(line, &leadingZeroes))
				stage = Stage::IS_EXP;
			return;
		}
		else if (stage == Stage::IS_EXP)
		{
			auto yesNo = getYesNo(line);
			if (yesNo)
			{
				displayAsExp = true;
				stage = Stage::IS_DECIMAL_POINTS;
			}
			else if (!yesNo)
			{
				displayAsExp = false;
				stage = Stage::IS_DECIMAL_POINTS;
			}
			return;
		}
		else if (stage == Stage::IS_DECIMAL_POINTS)
		{
			auto yesNo = getYesNo(line);
			if (yesNo)
			{
				decimalPoints = true;
				stage = Stage::NONE;
			}
			else if (!yesNo)
			{
				decimalPoints = false;
				stage = Stage::NONE;
			}
			return;
		}
	}

	Question* construct(easy_list::list<Option> options, easy_list::list<std::wstring> tags)
	{
		if (stage != Stage::NONE)
			return nullptr;
		return new SweetieJar(question, number, accuracy, sigFigsOrDecimalPoints, leadingZeroes, displayAsExp, decimalPoints, tags);
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