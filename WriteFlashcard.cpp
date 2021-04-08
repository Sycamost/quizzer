#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <fstream>
#include "Write.h"
#include "util.h"
#include "Flashcard.h"
#include "globals.h"
#include "WriteFlashcard.h"

std::wstring front = L"";
std::wstring back = L"";
std::vector<std::wstring> tags = std::vector<std::wstring>();
std::vector<Flashcard*> newFlashcards = std::vector<Flashcard*>();

CmdHandler::Returns writeCmdHandler(std::wstring userInput);

void WriteFlashcardStage::startWriting()
{
	setValue(Stage::FRONT);
}

WriteFlashcardStage::Stage WriteFlashcardStage::getValue() {
	return _value;
}

void WriteFlashcardStage::setValue(WriteFlashcardStage::Stage stage) {

	if (stage == Stage::NEW_CARD)
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

	if (stage == Stage::FRONT)
	{
		front = L"";
		back = L"";
		tags = std::vector<std::wstring>();
		std::wcout << L"Front:\t";
		_value = stage;
		return;
	}

	if (stage == Stage::BACK)
	{
		std::wcout << L"Back:\t";
		_value = stage;
		return;
	}

	if (stage == Stage::TAGS)
	{
		std::wcout << L"Tag " << std::to_wstring(tags.size() + 1) << L":\t";
		_value = stage;
		return;
	}
}
WriteFlashcardStage::Stage WriteFlashcardStage::_value = WriteFlashcardStage::Stage::NEW_CARD;

void startWritingFlashcards()
{
	std::wcout << "Writing new flashcards...\n\n";
	std::wcout << "Enter the values for new flashcards' front, back and any tags.\nOnce you're finished adding tags, leave the next tag blank.\nThe front and back cannot be blank.\nUse \"cancel\" to cancel adding the current card.\n";
	CmdHandler::setHandler(&writeCmdHandler);
	WriteFlashcardStage::startWriting();
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
	
	if (WriteFlashcardStage::getValue() == WriteFlashcardStage::Stage::NEW_CARD)
	{
		if (userInputUpper == Globals::cmdCase)
		{
			newFlashcards[newFlashcards.size() - 1]->setCaseSensitive();
			WriteFlashcardStage::setValue(WriteFlashcardStage::Stage::NEW_CARD);
			return Returns::SUCCESS;
		}

		if (isYes(userInputUpper))
		{
			WriteFlashcardStage::setValue(WriteFlashcardStage::Stage::FRONT);
			return Returns::SUCCESS;
		}

		finishWriting();
		return Returns::SUCCESS;
	}

	else if (WriteFlashcardStage::getValue() == WriteFlashcardStage::Stage::FRONT)
	{
		if (userInputUpper == Globals::cmdCancel)
		{
			std::wcout << L"\nAre you sure you want to cancel adding the current card? [Y/N]\n";
			if (getUserYesNo())
			{
				WriteFlashcardStage::setValue(WriteFlashcardStage::Stage::NEW_CARD);
				return Returns::SUCCESS;
			}
			WriteFlashcardStage::setValue(WriteFlashcardStage::Stage::FRONT);
			return Returns::SUCCESS;
		}

		if (userInput == L"")
		{
			WriteFlashcardStage::setValue(WriteFlashcardStage::Stage::FRONT);
			return Returns::SUCCESS;
		}

		front = userInput;
		WriteFlashcardStage::setValue(WriteFlashcardStage::Stage::BACK);
		return Returns::SUCCESS;
	}

	else if (WriteFlashcardStage::getValue() == WriteFlashcardStage::Stage::BACK)
	{
		if (userInputUpper == Globals::cmdCancel)
		{
			std::wcout << L"\nAre you sure you want to cancel adding the current card? [Y/N]\n";
			if (getUserYesNo())
			{
				WriteFlashcardStage::setValue(WriteFlashcardStage::Stage::NEW_CARD);
				return Returns::SUCCESS;
			}
			WriteFlashcardStage::setValue(WriteFlashcardStage::Stage::BACK);
			return Returns::SUCCESS;
		}

		if (userInput == L"")
		{
			WriteFlashcardStage::setValue(WriteFlashcardStage::Stage::BACK);
			return Returns::SUCCESS;
		}

		back = userInput;
		WriteFlashcardStage::setValue(WriteFlashcardStage::Stage::TAGS);
		return Returns::SUCCESS;
	}

	else if (WriteFlashcardStage::getValue() == WriteFlashcardStage::Stage::TAGS)
	{
		if (userInputUpper == Globals::cmdCancel)
		{
			std::wcout << L"\nAre you sure you want to cancel adding the current card? [Y/N]\n";
			if (getUserYesNo())
			{
				WriteFlashcardStage::setValue(WriteFlashcardStage::Stage::NEW_CARD);
				return Returns::SUCCESS;
			}
			WriteFlashcardStage::setValue(WriteFlashcardStage::Stage::TAGS);
			return Returns::SUCCESS;
		}

		if (userInput == L"")
		{
			newFlashcards.push_back(new Flashcard(front, back, false, tags));
			WriteFlashcardStage::setValue(WriteFlashcardStage::Stage::NEW_CARD);
			return Returns::SUCCESS;
		}

		if (userInput.find(L' ') != std::wstring::npos)
		{
			std::wcout << L"Tags must be one word only.\n";
			WriteFlashcardStage::setValue(WriteFlashcardStage::Stage::TAGS);
			return Returns::SUCCESS;
		}

		tags.push_back(userInput);
		WriteFlashcardStage::setValue(WriteFlashcardStage::Stage::TAGS);
		return Returns::SUCCESS;
	}

	std::wcout << L"\nSomething went wrong interpreting that input. Exiting write session...\n";
	finishWriting();
	return Returns::CMD_NOT_RECOGNISED;
}
