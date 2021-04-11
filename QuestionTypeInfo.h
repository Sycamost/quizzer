#pragma once
#include <string>
#include "QuestionType.h"
#include "QuestionWriter.h"

struct QuestionTypeInfo {
	QuestionType type;
	std::wstring displaySingular;
	std::wstring displayPlural;
	std::wstring code;
	QuestionWriter writer;
	QuestionTypeInfo(QuestionType type, std::wstring display, std::wstring code, QuestionWriter writer) :
		type(type),
		displaySingular(display),
		displayPlural(display + L"s"),
		code(code),
		writer(writer)
	{}
	QuestionTypeInfo(QuestionType type, std::wstring displaySingular, std::wstring displayPlural, std::wstring code, QuestionWriter writer) :
		type(type),
		displaySingular(displaySingular),
		displayPlural(displayPlural),
		code(code),
		writer(writer)
	{}
	bool operator==(const QuestionTypeInfo& other)
	{
		return
			type == other.type &&
			displaySingular == other.displaySingular &&
			displayPlural == other.displayPlural &&
			code == other.code;
	}
};

extern const std::vector<QuestionTypeInfo> questionTypeInfos;
const QuestionTypeInfo* getQuestionTypeInfo(QuestionType type);
const QuestionTypeInfo* getQuestionTypeInfoFromCode(std::wstring code);