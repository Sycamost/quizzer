#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <fstream>
#include "Write.h"
#include "util.h"
#include "Flashcard.h"
#include "globals.h"

std::wstring front = L"";
std::wstring back = L"";
std::vector<std::wstring> tags = std::vector<std::wstring>();
std::vector<Flashcard> newFlashcards = std::vector<Flashcard>();

CmdHandler::Returns writeCmdHandler(std::wstring userInput);

class WriteStage {
public:
	enum class Stage {
		NEW_CARD,
		FRONT,
		BACK,
		TAGS
	};

	static void startWriting()
	{
		setValue(Stage::FRONT);
	}

	static Stage getValue() {
		return _value;
	}

	static void setValue(Stage stage) {

		if (stage == Stage::NEW_CARD)
		{
			std::wcout << L"Would you like to add another card? [Y/N]\n";
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

private:
	static WriteStage::Stage _value;
};
WriteStage::Stage WriteStage::_value = WriteStage::Stage::NEW_CARD;

void startWriting()
{
	std::wcout << "Writing new flashcards...\n\n";
	std::wcout << "Enter the values for new flashcards' front, back and any tags.\nOnce you're finished adding tags, leave the next tag blank.\nThe front and back cannot be blank.\nUse \"cancel\" to cancel adding the current card.\n";
	CmdHandler::setHandler(&writeCmdHandler);
	WriteStage::startWriting();
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
			newFlashcards[i].write(file);
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

	Flashcard::appendFlashcardsToList(newFlashcards);

	CmdHandler::setHandlerDefault();
}

CmdHandler::Returns writeCmdHandler(std::wstring userInput)
{
	std::wstring userInputUpper = toUpper(userInput);
	using CmdHandler::Returns;
	
	if (WriteStage::getValue() == WriteStage::Stage::NEW_CARD)
	{
		if (isYes(userInputUpper))
		{
			WriteStage::setValue(WriteStage::Stage::FRONT);
			return Returns::SUCCESS;
		}

		finishWriting();
		return Returns::SUCCESS;
	}

	else if (WriteStage::getValue() == WriteStage::Stage::FRONT)
	{
		if (userInputUpper == Globals::cmdCancel)
		{
			std::wcout << L"\nAre you sure you want to cancel adding the current card? [Y/N]\n";
			if (getUserYesNo())
			{
				WriteStage::setValue(WriteStage::Stage::NEW_CARD);
				return Returns::SUCCESS;
			}
			WriteStage::setValue(WriteStage::Stage::FRONT);
			return Returns::SUCCESS;
		}

		if (userInput == L"")
		{
			WriteStage::setValue(WriteStage::Stage::FRONT);
			return Returns::SUCCESS;
		}

		front = userInput;
		WriteStage::setValue(WriteStage::Stage::BACK);
		return Returns::SUCCESS;
	}

	else if (WriteStage::getValue() == WriteStage::Stage::BACK)
	{
		if (userInputUpper == Globals::cmdCancel)
		{
			std::wcout << L"\nAre you sure you want to cancel adding the current card? [Y/N]\n";
			if (getUserYesNo())
			{
				WriteStage::setValue(WriteStage::Stage::NEW_CARD);
				return Returns::SUCCESS;
			}
			WriteStage::setValue(WriteStage::Stage::BACK);
			return Returns::SUCCESS;
		}

		if (userInput == L"")
		{
			WriteStage::setValue(WriteStage::Stage::BACK);
			return Returns::SUCCESS;
		}

		back = userInput;
		WriteStage::setValue(WriteStage::Stage::TAGS);
		return Returns::SUCCESS;
	}

	else if (WriteStage::getValue() == WriteStage::Stage::TAGS)
	{
		if (userInputUpper == Globals::cmdCancel)
		{
			std::wcout << L"\nAre you sure you want to cancel adding the current card? [Y/N]\n";
			if (getUserYesNo())
			{
				WriteStage::setValue(WriteStage::Stage::NEW_CARD);
				return Returns::SUCCESS;
			}
			WriteStage::setValue(WriteStage::Stage::TAGS);
			return Returns::SUCCESS;
		}

		if (userInput == L"")
		{
			newFlashcards.push_back(Flashcard(front, back, tags));
			WriteStage::setValue(WriteStage::Stage::NEW_CARD);
			return Returns::SUCCESS;
		}

		if (userInput.find(L' ') != std::wstring::npos)
		{
			std::wcout << L"Tags must be one word only.\n";
			WriteStage::setValue(WriteStage::Stage::TAGS);
			return Returns::SUCCESS;
		}

		tags.push_back(userInput);
		WriteStage::setValue(WriteStage::Stage::TAGS);
		return Returns::SUCCESS;
	}

	std::wcout << L"\nSomething went wrong interpreting that input. Exiting write session...\n";
	finishWriting();
	return Returns::CMD_NOT_RECOGNISED;
}
