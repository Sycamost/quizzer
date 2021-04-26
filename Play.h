#pragma once
#include <easy_list.h>
#include <string>
#include "Command.h"
#include "Question.h"

class Play
{
public:
	static void askQuestion();
	static void nextQuestion();
	static bool updateAnswer(std::wstring answer);
	static void finishPlaying();
	static DECLARE_CMD_FUNC(cmdFuncPlay);
	static DECLARE_CMD_FUNC(cmdFuncBoost);
private:
	static bool inputYesNoFinishPlaying(std::wstring message);
public:
	static DECLARE_CMD_FUNC(cmdFuncConcede);
	static DECLARE_CMD_FUNC(cmdFuncQuitPlay);
	static int getNumCorrect();
	static int getNumWrong();
	static int getNumSkipped();
	static std::wstring getCurrentCorrectAnswer();
private:
	static easy_list::list<Question*> _questions;
	static unsigned int _index;
	static int _correct;
	static int _wrong;
	static bool _isCorrect;
};