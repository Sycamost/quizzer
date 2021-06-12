#include <iostream>
#include <string>
#include <easy_list.h>
#include <Windows.h>
#include <exception>
#include <fstream>
#include "Command.h"
#include "util.h"
#include "MultipleChoice.h"
#include "MultipleChoiceWriter.h"
#include "globals.h"
#include "QuestionWriter.h"
#include "Write.h"

namespace MultipleChoiceWriter
{
	std::wstring question{ L"" };
	std::wstring correctAnswer{ L"" };
	easy_list::list<std::wstring> wrongAnswers{ {} };

	void askQuestion();
	void askCorrectAnswer();
	void askWrongAnswer();

	DEFINE_INPUT_HANDLER_FUNC(questionInputHandler)
	{
		if (input.empty())
		{
			askQuestion();
			return InputHandlerReturns::SUCCESS;
		}

		question = input;
		askCorrectAnswer();
		return InputHandlerReturns::SUCCESS;
	}

	DEFINE_INPUT_HANDLER_FUNC(correctAnswerInputHandler)
	{
		if (input.empty())
		{
			askCorrectAnswer();
			return InputHandlerReturns::SUCCESS;
		}

		correctAnswer = input;
		askWrongAnswer();
		return InputHandlerReturns::SUCCESS;
	}

	DEFINE_INPUT_HANDLER_FUNC(wrongAnswerInputHandler)
	{
		if (input.empty())
		{
			wrongAnswers.push_back(input);
			QuestionWriter::writeTags();
			return InputHandlerReturns::SUCCESS;
		}

		wrongAnswers.push_back(input);
		askWrongAnswer();
		return InputHandlerReturns::SUCCESS;
	}

	void askQuestion()
	{
		static const std::wstring msg = L"Question:\t";
		setInputHandling(msg, &questionInputHandler);
	}

	void askCorrectAnswer()
	{
		static const std::wstring msg = L"Correct answer:\t";
		setInputHandling(msg, &correctAnswerInputHandler);
	}

	void askWrongAnswer()
	{
		const std::wstring msg = L"Incorrect answer " + std::to_wstring(wrongAnswers.size() + 1) + L":\t";
		setInputHandling(msg, &wrongAnswerInputHandler);
	}

	void writeMultipleChoice()
	{
		question = L"";
		correctAnswer = L"";
		wrongAnswers = easy_list::list<std::wstring>();

		std::wcout <<
			L"Enter the values for new the new multiple choice question, correct answer, incorrect answers and tags.\n"
			<< L"Once you're finished adding tags, leave the next tag blank.\nUse <"
			<< toLower(CommandInfo::getFirstCode(CommandType::CANCEL))
			<< L"> to cancel adding the current multiple choice question.\n\n";

		askQuestion();
	}

	bool resetLastStep()
	{
		if (question.empty())
		{
			askQuestion();
			return true;
		}
		if (correctAnswer.empty())
		{
			askCorrectAnswer();
			return true;
		}
		if (wrongAnswers.empty() || wrongAnswers.back() != L"")
		{
			askWrongAnswer();
			return true;
		}
		return false;
	}

	Question* constructCurrent(easy_list::list<std::wstring> tags)
	{
		if (question.empty())
			return nullptr;
		if (correctAnswer.empty())
			return nullptr;
		if (wrongAnswers.empty() || wrongAnswers.back() != L"")
			return nullptr;
		return new MultipleChoice(question, correctAnswer, wrongAnswers.slice(0, -1), tags);
	}

	QuestionWriter& get()
	{
		static QuestionWriter writer = QuestionWriter(
			QuestionType::MULTIPLE_CHOICE,
			&writeMultipleChoice,
			&resetLastStep,
			&constructCurrent
		);
		return writer;
	}
}