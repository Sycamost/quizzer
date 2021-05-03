﻿#include <iostream>
#include <string>
#include <easy_list.h>
#include <Windows.h>
#include <exception>
#include <fstream>
#include "Command.h"
#include "util.h"
#include "SweetieJar.h"
#include "SweetieJarWriter.h"
#include "globals.h"
#include "QuestionWriter.h"
#include "Write.h"

namespace SweetieJarWriter
{
	std::wstring question{ L"" };
	long double number{ 0.l };
	long double accuracy{ 0.l };
	size_t sigFigs{ 0ull };
	size_t leadingZeroes{ 0ull };
	bool displayAsExp{ false };

	enum class Stage {
		QUESTION,
		NUMBER,
		ACCURACY,
		TAGS
	} stage;

	void askQuestion();
	void askNumber();
	void askAccuracy();

	DEFINE_INPUT_HANDLER_FUNC(questionInputHandler)
	{
		if (input.empty())
		{
			askNumber();
			return InputHandlerReturns::SUCCESS;
		}

		question = input;
		askNumber();
		return InputHandlerReturns::SUCCESS;
	}

	DEFINE_INPUT_HANDLER_FUNC(numberInputHandler)
	{
		if (!interpretLongDouble(input, &number))
		{
			askNumber();
			return InputHandlerReturns::SUCCESS;
		}

		sigFigs = countSignificantFigures(input);
		leadingZeroes = countLeadingZeroes(input);

		// Display with an exponent?
		easy_list::list<wchar_t> inputList = easy_list::list<wchar_t>(input.begin(), input.end());
		easy_list::list<std::wstring> inputListSubstrs = inputList.substrings().transform<std::wstring>(&easy_list::list<wchar_t>::toString);
		displayAsExp = inputListSubstrs.contains(&isExponentString);

		askAccuracy();
		return InputHandlerReturns::SUCCESS;
	}


	DEFINE_INPUT_HANDLER_FUNC(accuracyInputHandler)
	{
		if (!interpretLongDouble(input, &accuracy))
		{
			askAccuracy();
			return InputHandlerReturns::SUCCESS;
		}

		stage = Stage::TAGS;
		QuestionWriter::writeTags();
		return InputHandlerReturns::SUCCESS;
	}

	void askQuestion()
	{
		static const std::wstring msg = L"Question:\t";
		stage = Stage::QUESTION;
		setInputHandling(msg, &questionInputHandler);
	}

	void askNumber()
	{
		static const std::wstring msg = L"Answer (as a plain number):\t";
		stage = Stage::NUMBER;
		setInputHandling(msg, &numberInputHandler);
	}

	void askAccuracy()
	{
		static const std::wstring msg = L"Required accuracy (as a plain number): ±\t";
		stage = Stage::ACCURACY;
		setInputHandling(msg, &accuracyInputHandler);
	}

	void writeSweetieJar()
	{
		question = L"";
		number = 0.l;
		accuracy = 0.l;
		sigFigs = 0ull;
		leadingZeroes = 0ull;
		displayAsExp = false;
		stage = Stage::QUESTION;

		std::wcout <<
			L"Enter the values for new SweetieJar's question, the numerical answer, and the required accuracy for an answer to count as correct.\n"
			<< L"Once you're finished adding tags, leave the next tag blank.\n"
			<< L"Make sure the answer and accuracy are both numbers.\nUse <"
			<< toLower(CommandInfo::getFirstCode(CommandType::CANCEL))
			<< L"> to cancel adding the current SweetieJar.\n\n";

		askQuestion();

		return;
	}

	bool resetLastStep()
	{
		if (stage == Stage::QUESTION)
		{
			askQuestion();
			return true;
		}

		if (stage == Stage::NUMBER)
		{
			askNumber();
			return true;
		}

		if (stage == Stage::ACCURACY)
		{
			askAccuracy();
			return true;
		}

		return false;
	}

	Question* constructCurrent(easy_list::list<std::wstring> tags)
	{
		if (stage != Stage::TAGS)
			return nullptr;
		return new SweetieJar(question, number, accuracy, sigFigs, leadingZeroes, displayAsExp, tags);
	}

	QuestionWriter& get()
	{
		static QuestionWriter writer = QuestionWriter(
			QuestionType::SWEETIE_JAR,
			&writeSweetieJar,
			&resetLastStep,
			&constructCurrent
		);
		return writer;
	}
}