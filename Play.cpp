#include <iostream>
#include <algorithm>
#include <time.h>
#include <random>
#include "Play.h"
#include "CmdHandler.h"
#include "Flashcard.h"
#include "util.h"
#include "globals.h"

CmdHandler::Returns playHandler(std::wstring userInput);

class PlayStage
{
public:
	enum class Stage {
		QUESTION,
		ANSWER
	};

	static Stage getValue() {
		return _value;
	}

	static void startPlay(std::vector<Flashcard> flashcards)
	{
		_flashcards = flashcards;
		std::shuffle(_flashcards.begin(), _flashcards.end(), std::default_random_engine((unsigned int)time(NULL)));
		_index = 0;
		_correct = 0;
		_wrong = 0;
		setValue(Stage::QUESTION);
	}

	static void setValue(Stage stage) {

		if (stage == Stage::QUESTION)
		{
			_value = stage;

			_hasAnswered = false;

			if (_index >= _flashcards.size())
			{
				finishPlay();
				return;
			}

			std::wcout << L"\n" << _flashcards[_index].getFront() << L"\n";
			return;
		}

		if (stage == Stage::ANSWER)
		{
			_value = stage;
			_index++;
		}
	}

	static bool updateAnswer(std::wstring answer)
	{
		if (!_hasAnswered)
		{
			_isLastCorrect = (answer == _flashcards[_index].getBack());
			if (_isLastCorrect)
				_correct++;
			else
				_wrong++;
			_hasAnswered = true;
		}
		return _isLastCorrect;
	}

	static void boost()
	{
		if (_hasAnswered && !_isLastCorrect)
		{
			_isLastCorrect = true;
			_correct++;
			_wrong--;
			return;
		}
		throw new std::exception("Invalid boost.");
	}

	static int getNumCorrect()
	{
		return _correct;
	}

	static int getNumWrong()
	{
		return _wrong;
	}

	static int getNumSkipped()
	{
		return _flashcards.size() - (_correct + _wrong);
	}

	static std::wstring getCurrentCorrectAnswer()
	{
		return _flashcards[_index].getBack();
	}

	static void finishPlay()
	{
		std::wcout << L"\n" << Globals::horizontalRule << L"\n";
		std::wcout << L"\nEnd of play.\n"
			<< L"Final scores: "
			<< _correct << L" correct, "
			<< _wrong << L" incorrect";
		int numSkipped = getNumSkipped();
		if (numSkipped > 0)
			std::wcout << L" and " << numSkipped << L" skipped";
		std::wcout << L".\n" << Globals::horizontalDoubleRule << L"\n\n";

		CmdHandler::setHandlerDefault();
	}

private:
	static PlayStage::Stage _value;
	static std::vector<Flashcard> _flashcards;
	static unsigned int _index;
	static int _correct;
	static int _wrong;
	static bool _hasAnswered;
	static bool _isLastCorrect;
};
PlayStage::Stage PlayStage::_value = PlayStage::Stage::QUESTION;
std::vector<Flashcard> PlayStage::_flashcards = std::vector<Flashcard>();
unsigned int PlayStage::_index = 0;
int PlayStage::_correct = 0;
int PlayStage::_wrong = 0;
bool PlayStage::_hasAnswered = false;
bool PlayStage::_isLastCorrect = true;

void play(std::vector<Flashcard> flashcards, std::vector<std::wstring> tags)
{
	std::wcout << L"Starting play with ";
	std::vector<Flashcard> flashcardsInPlay = std::vector<Flashcard>();
	if (tags.empty())
	{
		flashcardsInPlay = flashcards;
		std::wcout << L"all ";
	}
	else
	{
		for (unsigned int i = 0; i < flashcards.size(); i++)
		{
			std::vector<std::wstring> thisCardTags = flashcards[i].getTags();
			std::transform(thisCardTags.begin(), thisCardTags.end(), thisCardTags.begin(), toUpper);
			if (shareAnyElems<std::wstring>(tags, thisCardTags))
				flashcardsInPlay.push_back(flashcards[i]);
		}
	}
	std::wcout << flashcardsInPlay.size()
		<< (flashcardsInPlay.size() > 1 ? L" cards" : L" card") << L"...\n\n"
		<< Globals::horizontalDoubleRule << L"\n\n"
		<< L"You'll get given the front of the card, and you'll have to input the back of the card. "
		<< L"If you think you've been marked down unfairly, type the command <"
		<< toLower(Globals::cmdBoost) << L"> before the next card rolls on. Good luck!\n\n"
		<< Globals::horizontalRule << L"\n\n";


	PlayStage::startPlay(flashcardsInPlay);

	CmdHandler::setHandler(playHandler);
}

CmdHandler::Returns playHandler(std::wstring userInput)
{
	if (toUpper(userInput) == Globals::cmdFinishPlay)
	{
		std::wcout << L"You still have " << PlayStage::getNumSkipped() << L" questions to answer. Are you sure you want to finish play here? [Y/N]\n";
		if (getUserYesNo())
		{
			PlayStage::finishPlay();
			return CmdHandler::Returns::SUCCESS;
		}
	}

	if (PlayStage::getValue() == PlayStage::Stage::QUESTION)
	{
		bool isCorrect = PlayStage::updateAnswer(userInput);

		if (isCorrect)
		{
			std::wcout << L"Correct! Press enter to continue.\n\n";
			PlayStage::setValue(PlayStage::Stage::ANSWER);
			return CmdHandler::Returns::SUCCESS;
		}
		else
		{
			std::wcout << L"Incorrect! The correct answer was:\n"
				<< indent(PlayStage::getCurrentCorrectAnswer(), 1)
				<< "\nPress enter to continue, or enter the command <"
				<< Globals::cmdBoost
				<< "> if you have been marked down unfairly.\n";
			PlayStage::setValue(PlayStage::Stage::ANSWER);
			return CmdHandler::Returns::SUCCESS;
		}
	}

	if (PlayStage::getValue() == PlayStage::Stage::ANSWER)
	{
		if (toUpper(userInput) == Globals::cmdBoost)
			PlayStage::boost();

		PlayStage::setValue(PlayStage::Stage::QUESTION);
		return CmdHandler::Returns::SUCCESS;
	}

	std::wcout << L"\nSomething went wrong interpreting that input. Exiting play...\n";
	PlayStage::finishPlay();
	return CmdHandler::Returns::CMD_NOT_RECOGNISED;
}
