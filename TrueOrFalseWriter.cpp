#include <iostream>
#include <string>
#include <easy_list.h>
#include <Windows.h>
#include <exception>
#include <fstream>
#include "Command.h"
#include "util.h"
#include "TrueOrFalse.h"
#include "TrueOrFalseWriter.h"
#include "globals.h"
#include "QuestionWriter.h"
#include "Write.h"

namespace TrueOrFalseWriter
{
	
	std::wstring prompt{ L"" };
	bool isTrue{ false };

	enum class Stage {
		PROMPT,
		IS_TRUE,
		TAGS
	} stage;

	
	void askPrompt();
	void askIsTrue();

	
	bool isValidPrompt(std::wstring input)
	{
		return !input.empty();
	}
	
	bool isValidIsTrue(std::wstring input)
	{
		auto yesNo = getYesNo(input);
		return yesNo || !yesNo;
	}
	

	
	DEFINE_INPUT_HANDLER_FUNC(promptInputHandler)
	{
		if (!isValidPrompt(input))
		{
			askPrompt();
			return InputHandlerReturns::SUCCESS;
		}

		prompt = input;
		askIsTrue();
		return InputHandlerReturns::SUCCESS;
	}
	
	DEFINE_INPUT_HANDLER_FUNC(isTrueInputHandler)
	{
		if (!isValidIsTrue(input))
		{
			askIsTrue();
			return InputHandlerReturns::SUCCESS;
		}

		isTrue = getYesNo(input);
		QuestionWriter::writeTags();
		stage = Stage::TAGS;
		return InputHandlerReturns::SUCCESS;
	}
	

	
	void askPrompt()
	{
		static const std::wstring msg = L"Prompt:\t";
		stage = Stage::PROMPT;
		setInputHandling(msg, &promptInputHandler);
	}
	
	void askIsTrue()
	{
		static const std::wstring msg = L"True or false?\t";
		stage = Stage::IS_TRUE;
		setInputHandling(msg, &isTrueInputHandler);
	}
	

	void writeTrueOrFalse()
	{
		prompt = L"";
		isTrue = false;

		std::wcout <<
			L"Enter the values for the new true/false question's data and tags.\n"
			<< L"Make sure your prompt is a statement, such as \"the cat sat on the mat.\" Don't include anything like \"true or false?\" - we'll add that on ourselves!\n"
			<< L"Once you're finished adding tags, leave the next tag blank.\n"
			<< L"\nUse <"
			<< toLower(CommandInfo::getFirstCode(CommandType::CANCEL))
			<< L"> to cancel adding the current true/false question.\n\n";

		askPrompt();

		return;
	}

	bool resetLastStep()
	{
		if (stage == Stage::PROMPT)
		{
			askPrompt();
			return true;
		}
		if (stage == Stage::IS_TRUE)
		{
			askIsTrue();
			return true;
		}
		return false;
	}

	Question* constructCurrent(easy_list::list<std::wstring> tags)
	{
		if (stage != Stage::TAGS)
			return nullptr;
		return new TrueOrFalse(prompt, isTrue, tags);
	}

	QuestionWriter& get()
	{
		static QuestionWriter writer = QuestionWriter(
			QuestionType::TRUE_OR_FALSE,
			&writeTrueOrFalse,
			&resetLastStep,
			&constructCurrent
		);
		return writer;
	}
}