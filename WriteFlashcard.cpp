#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <exception>
#include <fstream>
#include "CmdWrite.h"
#include "util.h"
#include "Flashcard.h"
#include "globals.h"
#include "WriteFlashcard.h"
#include "WriteQuestion.h"

WriteFlashcard::InputDataStage WriteFlashcard::_inputDataStage = WriteFlashcard::InputDataStage::FRONT;
std::wstring WriteFlashcard::_currentFront = L"";
std::wstring WriteFlashcard::_currentBack = L"";
std::vector<Flashcard*> WriteFlashcard::_newFlashcards = std::vector<Flashcard*>();

CmdHandler::Returns writeCmdHandler(std::wstring userInput);

void WriteFlashcard::setInputDataStage(InputDataStage inputDataStage) {

	if (inputDataStage == InputDataStage::FRONT)
	{
		_currentFront = L"";
		_currentBack = L"";
		std::wcout << L"Front:\t";
		_inputDataStage = inputDataStage;
		return;
	}

	if (inputDataStage == InputDataStage::BACK)
	{
		std::wcout << L"Back:\t";
		_inputDataStage = inputDataStage;
		return;
	}

	if (inputDataStage == InputDataStage::CASE_SENSITIVE)
	{
		std::wcout << L"Case-sensitive? [Y/N]:\t";
		_inputDataStage = inputDataStage;
		return;
	}
}

void WriteFlashcard::startWriting()
{
	std::wcout << "Enter the values for new flashcards' front, back and any tags.\nOnce you're finished adding tags, leave the next tag blank.\nThe front and back cannot be blank.\nUse \"cancel\" to cancel adding the current card.\n";
	setInputDataStage(InputDataStage::FRONT);
}

std::vector<Question*> WriteFlashcard::writeToFile()
{
	try
	{
		std::wofstream file(Globals::flashcardsFileAddress, std::ios::app);
		if (!file.is_open())
			throw new std::exception("Couldn't open file.");
		for (unsigned int i = 0; i < _newFlashcards.size(); i++)
			_newFlashcards[i]->write(file);
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

	return convv<Flashcard*, Question*>(_newFlashcards);
}

void WriteFlashcard::inputData(std::wstring)
{
	std::wstring userInputUpper = toUpper(userInput);
	using CmdHandler::Returns;

	if (WriteFlashcard::getValue() == WriteFlashcard::InputStage::FRONT)
	{
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
}

void WriteFlashcard::cancel()
{
}

void WriteFlashcard::resetLastInputStep()
{
}

void WriteFlashcard::pushCurrent()
{
}

std::vector<Question*> WriteFlashcard::writeToFile()
{
	return std::vector<Question*>();
}
