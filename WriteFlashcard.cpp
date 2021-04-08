#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <fstream>
#include "CmdWrite.h"
#include "util.h"
#include "Flashcard.h"
#include "globals.h"
#include "WriteFlashcard.h"

std::wstring front = L"";
std::wstring back = L"";
std::vector<std::wstring> tags = std::vector<std::wstring>();
std::vector<Flashcard*> newFlashcards = std::vector<Flashcard*>();

CmdHandler::Returns writeCmdHandler(std::wstring userInput);

void WriteFlashcard::startWriting()
{
	setStage(InputStage::FRONT);
}

WriteFlashcard::InputStage WriteFlashcard::getValue() {
	return _value;
}

void WriteFlashcard::setStage(WriteFlashcard::InputStage stage) {

	if (stage == InputStage::NEW_CARD)
	{
		if (newFlashcards[newFlashcards.size() - 1]->isCaseSensitive())
		{
			std::wcout << L"W";
		}
		else
		{
			std::wcout << L"If the previous card should be checked as case-sensitive, enter <"
				<< toLower(Globals::cmdCase) <<
				L">. Otherwise, w";
		}
		std::wcout << "ould you like to add another card ? [Y/N]\n";
		_value = stage;
		return;
	}

	if (stage == InputStage::FRONT)
	{
		front = L"";
		back = L"";
		tags = std::vector<std::wstring>();
		std::wcout << L"Front:\t";
		_value = stage;
		return;
	}

	if (stage == InputStage::BACK)
	{
		std::wcout << L"Back:\t";
		_value = stage;
		return;
	}

	if (stage == InputStage::TAGS)
	{
		std::wcout << L"Tag " << std::to_wstring(tags.size() + 1) << L":\t";
		_value = stage;
		return;
	}
}
WriteFlashcard::InputStage WriteFlashcard::_value = WriteFlashcard::InputStage::NEW_CARD;

void startWritingFlashcards()
{
	std::wcout << "Writing new flashcards...\n\n";
	std::wcout << "Enter the values for new flashcards' front, back and any tags.\nOnce you're finished adding tags, leave the next tag blank.\nThe front and back cannot be blank.\nUse \"cancel\" to cancel adding the current card.\n";
	CmdHandler::setHandler(&writeCmdHandler);
	WriteFlashcard::startWriting();
}

void finishWriting()
{
	std::wcout << "\nFinished writing new flashcards. Writing to file...\n";

	try
	{
		std::wofstream file(Globals::flashcardsFileAddress, std::ios::app);
		if (!file.is_open())
			throw new std::exception("Couldn't open file.");
		for (unsigned int i = 0; i < newFlashcards.size(); i++)
			newFlashcards[i]->write(file);
		file.close();
	}
	catch (std::exception e)
	{
		std::wcout << L"\nWrite failed. Sorry! :p\n";
		std::wcout << L"We got the following error message: ";
		std::cout << e.what();
		std::wcout << L"\n";
	}
	std::wcout << L"Finished writing.\n\n";

	std::vector<Question*> newQuestions = std::vector<Question*>();
	for (unsigned int i = 0; i < newFlashcards.size(); i++)
		newQuestions.push_back(newFlashcards[i]);
	Question::appendQuestionsToList(newQuestions);

	CmdHandler::setHandlerDefault();
}

CmdHandler::Returns writeCmdHandler(std::wstring userInput)
{
	std::wstring userInputUpper = toUpper(userInput);
	using CmdHandler::Returns;
	
	if (WriteFlashcard::getValue() == WriteFlashcard::InputStage::NEW_CARD)
	{
		if (userInputUpper == Globals::cmdCase)
		{
			newFlashcards[newFlashcards.size() - 1]->setCaseSensitive();
			WriteFlashcard::setStage(WriteFlashcard::InputStage::NEW_CARD);
			return Returns::SUCCESS;
		}

		if (isYes(userInputUpper))
		{
			WriteFlashcard::setStage(WriteFlashcard::InputStage::FRONT);
			return Returns::SUCCESS;
		}

		finishWriting();
		return Returns::SUCCESS;
	}

	else if (WriteFlashcard::getValue() == WriteFlashcard::InputStage::FRONT)
	{
		if (userInputUpper == Globals::cmdCancel)
		{
			std::wcout << L"\nAre you sure you want to cancel adding the current card? [Y/N]\n";
			if (getUserYesNo())
			{
				WriteFlashcard::setStage(WriteFlashcard::InputStage::NEW_CARD);
				return Returns::SUCCESS;
			}
			WriteFlashcard::setStage(WriteFlashcard::InputStage::FRONT);
			return Returns::SUCCESS;
		}

		if (userInput == L"")
		{
			WriteFlashcard::setStage(WriteFlashcard::InputStage::FRONT);
			return Returns::SUCCESS;
		}

		front = userInput;
		WriteFlashcard::setStage(WriteFlashcard::InputStage::BACK);
		return Returns::SUCCESS;
	}

	else if (WriteFlashcard::getValue() == WriteFlashcard::InputStage::BACK)
	{
		if (userInputUpper == Globals::cmdCancel)
		{
			std::wcout << L"\nAre you sure you want to cancel adding the current card? [Y/N]\n";
			if (getUserYesNo())
			{
				WriteFlashcard::setStage(WriteFlashcard::InputStage::NEW_CARD);
				return Returns::SUCCESS;
			}
			WriteFlashcard::setStage(WriteFlashcard::InputStage::BACK);
			return Returns::SUCCESS;
		}

		if (userInput == L"")
		{
			WriteFlashcard::setStage(WriteFlashcard::InputStage::BACK);
			return Returns::SUCCESS;
		}

		back = userInput;
		WriteFlashcard::setStage(WriteFlashcard::InputStage::TAGS);
		return Returns::SUCCESS;
	}

	else if (WriteFlashcard::getValue() == WriteFlashcard::InputStage::TAGS)
	{
		if (userInputUpper == Globals::cmdCancel)
		{
			std::wcout << L"\nAre you sure you want to cancel adding the current card? [Y/N]\n";
			if (getUserYesNo())
			{
				WriteFlashcard::setStage(WriteFlashcard::InputStage::NEW_CARD);
				return Returns::SUCCESS;
			}
			WriteFlashcard::setStage(WriteFlashcard::InputStage::TAGS);
			return Returns::SUCCESS;
		}

		if (userInput == L"")
		{
			newFlashcards.push_back(new Flashcard(front, back, false, tags));
			WriteFlashcard::setStage(WriteFlashcard::InputStage::NEW_CARD);
			return Returns::SUCCESS;
		}

		if (userInput.find(L' ') != std::wstring::npos)
		{
			std::wcout << L"Tags must be one word only.\n";
			WriteFlashcard::setStage(WriteFlashcard::InputStage::TAGS);
			return Returns::SUCCESS;
		}

		tags.push_back(userInput);
		WriteFlashcard::setStage(WriteFlashcard::InputStage::TAGS);
		return Returns::SUCCESS;
	}

	std::wcout << L"\nSomething went wrong interpreting that input. Exiting write session...\n";
	finishWriting();
	return Returns::CMD_NOT_RECOGNISED;
}
