#include <iostream>
#include <algorithm>
#include <time.h>
#include <random>
#include "Play.h"
#include "CmdHandler.h"
#include "Question.h"
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

	static void startPlay(std::vector<Question*> questions)
	{
		_questions = questions;
		std::shuffle(_questions.begin(), _questions.end(), std::default_random_engine((unsigned int)time(NULL)));
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

			if (_index >= _questions.size())
			{
				finishPlay();
				return;
			}

			std::wcout << L"\n" << _questions[_index]->getQuestion() << L"\n";
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
			_isCorrect = _questions[_index]->isCorrect(answer);
			if (_isCorrect)
				_correct++;
			else
				_wrong++;
			_hasAnswered = true;
		}
		return _isCorrect;
	}

	static void boost()
	{
		if (_hasAnswered && !_isCorrect)
		{
			_isCorrect = true;
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
		return _questions.size() - (_correct + _wrong);
	}

	static std::wstring getCurrentCorrectAnswer()
	{
		return _questions[_index]->getAnswer();
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
	static std::vector<Question*> _questions;
	static unsigned int _index;
	static int _correct;
	static int _wrong;
	static bool _hasAnswered;
	static bool _isCorrect;
};
PlayStage::Stage PlayStage::_value = PlayStage::Stage::QUESTION;
std::vector<Question*> PlayStage::_questions = std::vector<Question*>();
unsigned int PlayStage::_index = 0;
int PlayStage::_correct = 0;
int PlayStage::_wrong = 0;
bool PlayStage::_hasAnswered = false;
bool PlayStage::_isCorrect = true;

DECLARE_CMD_FUNC(startPlaying)
{
	std::wcout << L"Starting play with ";
	std::vector<Question*> questionsInPlay = std::vector<Question*>();
	if (args.empty())
	{
		questionsInPlay = Question::getQuestionList();
		std::wcout << L"all ";
	}
	else
	{
		std::vector<Question*> questionList = Question::getQuestionList();
		for (unsigned int i = 0; i < questionList.size(); i++)
		{
			std::vector<std::wstring> thisQuestionTags = questionList[i]->getTags();
			std::transform(thisQuestionTags.begin(), thisQuestionTags.end(), thisQuestionTags.begin(), toUpper);
			if (shareAnyElems<std::wstring>(args, thisQuestionTags))
				questionsInPlay.push_back(questionList[i]);
		}
	}
	std::wcout << questionsInPlay.size()
		<< (questionsInPlay.size() > 1 ? L" cards" : L" card") << L"...\n\n"
		<< Globals::horizontalDoubleRule << L"\n\n"
		<< L"You'll get given questions, and you'll have to answer correctly. "
		<< L"If you think you've been marked down unfairly, type the command <"
		<< toLower(Command::getCommandInfo(CommandType::BOOST)->code)
		<< L"> before the next card rolls on. Good luck!\n\n"
		<< Globals::horizontalRule << L"\n\n";


	PlayStage::startPlay(questionsInPlay);

	CmdHandler::setHandler(playHandler);
};

CmdHandler::Returns playHandler(std::wstring userInput)
{
	Command* command = Command::read(userInput);

	if (command != nullptr)
	{
		CommandInfo commandInfo = command->getCommandInfo();
		if (commandInfo.isType(CommandType::FINISH))
		{
			std::wcout << L"You still have " << PlayStage::getNumSkipped() << L" questions to answer. Are you sure you want to finish play here? [Y/N]\n";
			if (getUserYesNo())
			{
				PlayStage::finishPlay();
				return CmdHandler::Returns::SUCCESS;
			}
		}
		return CmdHandler::Returns::CMD_NOT_RECOGNISED;
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
				<< toLower(Command::getCommandInfo(CommandType::BOOST)->code)
				<< "> if you have been marked down unfairly.\n";
			PlayStage::setValue(PlayStage::Stage::ANSWER);
			return CmdHandler::Returns::SUCCESS;
		}
	}

	if (PlayStage::getValue() == PlayStage::Stage::ANSWER)
	{
		if (command != nullptr && command->getCommandInfo().isType(CommandType::BOOST))
			PlayStage::boost();

		PlayStage::setValue(PlayStage::Stage::QUESTION);
		return CmdHandler::Returns::SUCCESS;
	}

	std::wcout << L"\nSomething went wrong interpreting that input. Exiting play...\n";
	PlayStage::finishPlay();
	return CmdHandler::Returns::CMD_NOT_RECOGNISED;
}
