#pragma once
#include "QuestionTypeInfo.h"
#include "Handlers.h"

class Write
{
public:
	static DECLARE_CMD_FUNC(cmdFuncWrite);
	static DECLARE_CMD_FUNC(cmdFuncCancel);
	static void nextQuestion();
private:
	static QuestionTypeInfo _typeInfo;
	static void startWriting(const QuestionTypeInfo qti);
	static void finishWriting();
};