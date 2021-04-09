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

WriteFlashcard::InputDataStage WriteFlashcard::_inputDataStage = WriteFlashcard::InputDataStage::SLEEP;
std::wstring WriteFlashcard::_currentFront = L"";
std::wstring WriteFlashcard::_currentBack = L"";
bool WriteFlashcard::_currentCaseSensitive = false;
std::vector<Flashcard*> WriteFlashcard::_newFlashcards = std::vector<Flashcard*>();

CmdHandler::Returns writeCmdHandler(std::wstring userInput);

void WriteFlashcard::setInputDataStage(InputDataStage inputDataStage) {

	if (inputDataStage == InputDataStage::FRONT)
	{
		_currentFront = L"";
		_currentBack = L"";
		_currentCaseSensitive = false;
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

const std::wstring WriteFlashcard::startWritingMessage =
	L"Enter the values for new flashcards' front, back and any tags.\nOnce you're finished adding tags, leave the next tag blank.\nThe front and back cannot be blank.\nUse <"
	+ toLower(Command::getCommandInfo(CommandType::CANCEL)->code)
	+ L"> to cancel adding the current card.\n";

void WriteFlashcard::startInputData()
{
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

void WriteFlashcard::inputData(std::wstring userInput)
{
	bool isEmpty = (userInput == L"");
	auto resetIfEmpty = [isEmpty]() -> bool {
		if (isEmpty)
		{
			resetLastInputStep();
			return true;
		}
		return false;
	};

	if (_inputDataStage == InputDataStage::SLEEP)
	{
		std::wcout << L"Error - I was asked to take user input for a new flashcard, but I was asleep at the time. Exiting write session...\n";
		WriteQuestion::finishWriting();
		return;
	}

	if (_inputDataStage == InputDataStage::FRONT)
	{
		if (resetIfEmpty())
			return;
		_currentFront = userInput;
		setInputDataStage(InputDataStage::BACK);
		return;
	}

	if (_inputDataStage == InputDataStage::BACK)
	{
		if (resetIfEmpty())
			return;
		_currentBack = userInput;
		setInputDataStage(InputDataStage::CASE_SENSITIVE);
		return;
	}

	if (_inputDataStage == InputDataStage::CASE_SENSITIVE)
	{
		_currentCaseSensitive = isYes(userInput);
		WriteQuestion::setStage(WriteQuestion::Stage::INPUT_TAGS);
		setInputDataStage(InputDataStage::SLEEP);
		return;
	}

	std::wcout << L"\nSomething went wrong interpreting that input. Exiting write session...\n";
	WriteQuestion::finishWriting();
}

void WriteFlashcard::cancel()
{
	_currentFront = L"";
	_currentBack = L"";
	_currentCaseSensitive = false;
	_inputDataStage = InputDataStage::SLEEP;
}

void WriteFlashcard::resetLastInputStep()
{
	setInputDataStage(_inputDataStage);
}

void WriteFlashcard::pushCurrent(std::vector<std::wstring> tags)
{
	_newFlashcards.push_back(new Flashcard(_currentFront, _currentBack, _currentCaseSensitive, tags));
	_currentFront = L"";
	_currentBack = L"";
	_currentCaseSensitive = false;
}

std::vector<Question*> WriteFlashcard::writeToFile()
{
	return std::vector<Question*>();
}
