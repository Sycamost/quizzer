#include <iostream>
#include <string>
#include <easy_list.h>
#include <Windows.h>
#include <exception>
#include <fstream>
#include "Command.h"
#include "util.h"
#include "Flashcard.h"
#include "globals.h"
#include "QuestionWriter.h"

namespace FlashcardWriter
{
	std::wstring front{ L"" };
	std::wstring back{ L"" };
	bool caseSensitive{ false };

	void askFront();
	void askBack();
	void askCaseSensitive();

	DEFINE_INPUT_HANDLER_FUNC(frontInputHandlerFunc)
	{
		if (input.empty())
		{
			askFront();
			return InputHandlerReturns::SUCCESS;
		}

		front = input;
		askBack();
		return InputHandlerReturns::SUCCESS;
	}

	DEFINE_INPUT_HANDLER_FUNC(backInputHandlerFunc)
	{
		if (input.empty())
		{
			askBack();
			return InputHandlerReturns::SUCCESS;
		}

		back = input;
		QuestionWriter::writeTags();
		return InputHandlerReturns::SUCCESS;
	}

	DEFINE_INPUT_HANDLER_FUNC(caseSensitiveInputHandlerFunc)
	{
		auto yesNo = getYesNo(input);

		if (yesNo || !yesNo)
		{
			caseSensitive = yesNo;
			QuestionWriter::writeTags();
			return InputHandlerReturns::SUCCESS;
		}

		std::wcout << L"Please enter either \"yes\" or \"no\".";
		askCaseSensitive();
		return InputHandlerReturns::SUCCESS;
	}

	void askFront()
	{
		std::wcout << L"Front:\t";
		setHandling(&frontInputHandlerFunc, CommandType::CANCEL);
	}

	void askBack()
	{
		std::wcout << L"Back:\t";
		setHandling(&backInputHandlerFunc, CommandType::CANCEL);
	}

	void askCaseSensitive()
	{
		std::wcout << L"Case-sensitive? [Y/N]:\t";
		setHandling(&caseSensitiveInputHandlerFunc, CommandType::CANCEL);
	}

	void writeFlashcard()
	{
		front = L"";
		back = L"";
		caseSensitive = false;

		std::wcout <<
			L"Enter the values for new flashcards' front, back and any tags.\n"
			<< L"Once you're finished adding tags, leave the next tag blank.\n"
			<< L"The front and back cannot be blank.\nUse <"
			<< toLower(CommandInfo::get(CommandType::CANCEL)->getCode())
			<< L"> to cancel adding the current card.\n\n";

		askFront();
		setHandling(&frontInputHandlerFunc, CommandType::CANCEL);
		return;
	}

	bool resetLastStep()
	{
		if (front.empty())
		{
			askFront();
			return true;
		}
		else if (back.empty())
		{
			askBack();
			return true;
		}
		return false;
	}

	Question* constructCurrent(easy_list::list<std::wstring> tags)
	{
		if (front.empty())
			return nullptr;
		if (back.empty())
			return nullptr;
		return new Flashcard(front, back, caseSensitive, tags);
	}

	QuestionWriter& get()
	{
		static QuestionWriter writer = QuestionWriter(
			QuestionType::FLASHCARD,
			&writeFlashcard,
			&resetLastStep,
			&constructCurrent
		);
		return writer;
	}
}