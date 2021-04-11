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

const std::string flashcardsFileAddress = "flashcards.txt";
const std::wstring startWritingFlashcardsMessage =
	L"Enter the values for new flashcards' front, back and any tags.\nOnce you're finished adding tags, leave the next tag blank.\nThe front and back cannot be blank.\nUse <"
	+ toLower(Command::getCommandInfo(CommandType::CANCEL)->code)
	+ L"> to cancel adding the current card.\n";

void startInputFlashcard();
bool inputFlashcard(std::wstring userInput);
void resetLastFlashcardStep();
Question* constructCurrentFlashcard(std::vector<std::wstring> tags);

extern const QuestionWriter* flashcardWriter = new QuestionWriter(
	QuestionType::FLASHCARD,
	flashcardsFileAddress,
	startWritingFlashcardsMessage,
	&startInputFlashcard,
	&inputFlashcard,
	&resetLastFlashcardStep,
	&constructCurrentFlashcard
);

enum FlashcardWriteStage {
	FRONT,
	BACK,
	CASE_SENSITIVE,
	SLEEP
};

FlashcardWriteStage flashcardWriteStage{ SLEEP };
std::wstring front{ L"" };
std::wstring back{ L"" };
bool caseSensitive{ false };

void setFlashcardWriteStage(FlashcardWriteStage stage) {

	if (stage == FlashcardWriteStage::FRONT)
	{
		front = L"";
		back = L"";
		caseSensitive = false;
		std::wcout << L"Front:\t";
		flashcardWriteStage = stage;
		return;
	}

	else if (stage == FlashcardWriteStage::BACK)
	{
		std::wcout << L"Back:\t";
		flashcardWriteStage = stage;
		return;
	}

	else if (stage == FlashcardWriteStage::CASE_SENSITIVE)
	{
		std::wcout << L"Case-sensitive? [Y/N]:\t";
		flashcardWriteStage = stage;
		return;
	}

	else if (stage == FlashcardWriteStage::SLEEP)
	{
		flashcardWriteStage = stage;
		return;
	}
}

void startInputFlashcard()
{
	setFlashcardWriteStage(FlashcardWriteStage::FRONT);
}

bool inputFlashcard(std::wstring userInput)
{
	bool isEmpty = (userInput == L"");
	auto resetIfEmpty = [isEmpty]() -> bool {
		if (isEmpty)
		{
			resetLastFlashcardStep();
			return true;
		}
		return false;
	};

	if (flashcardWriteStage == FlashcardWriteStage::FRONT)
	{
		if (resetIfEmpty())
			return true;
		front = userInput;
		setFlashcardWriteStage(FlashcardWriteStage::BACK);
		return true;
	}

	if (flashcardWriteStage == FlashcardWriteStage::BACK)
	{
		if (resetIfEmpty())
			return true;
		back = userInput;
		setFlashcardWriteStage(FlashcardWriteStage::CASE_SENSITIVE);
		return true;
	}

	if (flashcardWriteStage == FlashcardWriteStage::CASE_SENSITIVE)
	{
		caseSensitive = isYes(userInput);
		setFlashcardWriteStage(FlashcardWriteStage::SLEEP);
		return true;
	}

	return false;
}

void resetLastFlashcardStep()
{
	setFlashcardWriteStage(flashcardWriteStage);
}

Question* constructCurrentFlashcard(std::vector<std::wstring> tags)
{
	if (front == L"")
		return nullptr;
	if (back == L"")
		return nullptr;
	return new Flashcard(front, back, caseSensitive, tags);
}