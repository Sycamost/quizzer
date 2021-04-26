#pragma once
#include "QuestionTypeInfo.h"
#include "Handlers.h"

class Write
{
public:
	static DECLARE_CMD_FUNC(cmdFuncWrite);
	static DECLARE_CMD_FUNC(cmdFuncCancel);
	static DECLARE_CMD_FUNC(cmdFuncQuitWrite);
	static void nextQuestion();
private:
	static QuestionTypeInfo _typeInfo;
	static easy_list::list<Question*> _newQuestions;
	static void startWriting(const QuestionTypeInfo qti);
	static easy_list::list<Question*> writeToFile();
	static void finishWriting();
};