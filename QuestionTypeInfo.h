#pragma once
#include <string>
#include "QuestionType.h"
#include "QuestionWriter.h"
#include "QuestionReader.h"

struct QuestionTypeInfo {
	QuestionType type;
	std::wstring displaySingular;
	std::wstring displayPlural;
	std::wstring code;
	std::string getFileAddress() const;
	QuestionWriter writer;
	QuestionReader reader;
	bool operator==(const QuestionTypeInfo& other)
	{
		return
			type == other.type &&
			displaySingular == other.displaySingular &&
			displayPlural == other.displayPlural &&
			code == other.code;
	}
private:
	QuestionTypeInfo(
		QuestionType type,
		std::wstring display,
		std::wstring code,
		QuestionWriter writer,
		QuestionReader reader)
		:
		type(type),
		displaySingular(display),
		displayPlural(display + L"s"),
		code(code),
		writer(writer),
		reader(reader)
	{}
	QuestionTypeInfo(
		QuestionType type,
		std::wstring displaySingular,
		std::wstring displayPlural,
		std::wstring code,
		QuestionWriter writer,
		QuestionReader reader)
		:
		type(type),
		displaySingular(displaySingular),
		displayPlural(displayPlural),
		code(code),
		writer(writer),
		reader(reader)
	{}
};

extern const std::vector<QuestionTypeInfo> questionTypeInfos;
const QuestionTypeInfo* getQuestionTypeInfo(QuestionType type);
const QuestionTypeInfo* getQuestionTypeInfoFromCode(std::wstring code);