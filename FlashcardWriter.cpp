#include <iostream>
#include <string>
#include <vector>
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
	enum class Stage {
		FRONT,
		BACK,
		CASE_SENSITIVE,
		SLEEP
	};

	Stage stage{ Stage::SLEEP };
	std::wstring front{ L"" };
	std::wstring back{ L"" };
	bool caseSensitive{ false };
	const std::wstring startWritingMessage{
		L"Enter the values for new flashcards' front, back and any tags.\nOnce you're finished adding tags, leave the next tag blank.\nThe front and back cannot be blank.\nUse <"
		+ toLower(Command::getCommandInfo(CommandType::CANCEL)->code)
		+ L"> to cancel adding the current card.\n" };

	void setFlashcardWriteStage(Stage stage)
	{

		if (stage == Stage::FRONT)
		{
			front = L"";
			back = L"";
			caseSensitive = false;
			std::wcout << L"Front:\t";
			FlashcardWriter::stage = stage;
			return;
		}

		else if (stage == Stage::BACK)
		{
			std::wcout << L"Back:\t";
			FlashcardWriter::stage = stage;
			return;
		}

		else if (stage == Stage::CASE_SENSITIVE)
		{
			std::wcout << L"Case-sensitive? [Y/N]:\t";
			FlashcardWriter::stage = stage;
			return;
		}

		else if (stage == Stage::SLEEP)
		{
			FlashcardWriter::stage = stage;
			return;
		}
	}

	void startInput()
	{
		setFlashcardWriteStage(Stage::FRONT);
	}

	void resetLastStep()
	{
		setFlashcardWriteStage(stage);
	}

	bool input(std::wstring userInput)
	{
		if (userInput == L"")
		{
			resetLastStep();
			return true;
		}

		if (stage == Stage::FRONT)
		{
			front = userInput;
			setFlashcardWriteStage(Stage::BACK);
			return true;
		}

		if (stage == Stage::BACK)
		{
			back = userInput;
			setFlashcardWriteStage(Stage::CASE_SENSITIVE);
			return true;
		}

		if (stage == Stage::CASE_SENSITIVE)
		{
			caseSensitive = isYes(userInput);
			setFlashcardWriteStage(Stage::SLEEP);
			return true;
		}

		return false;
	}

	Question* constructCurrent(std::vector<std::wstring> tags)
	{
		if (front == L"")
			return nullptr;
		if (back == L"")
			return nullptr;
		return new Flashcard(front, back, caseSensitive, tags);
	}

	QuestionWriter& get()
	{
		static QuestionWriter writer = QuestionWriter(
			QuestionType::FLASHCARD,
			startWritingMessage,
			&startInput,
			&input,
			&resetLastStep,
			&constructCurrent
		);
		return writer;
	}
}