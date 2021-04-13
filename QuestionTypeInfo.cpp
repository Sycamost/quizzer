#include <vector>
#include <algorithm>
#include "QuestionTypeInfo.h"
#include "FlashcardWriter.h"
#include "FlashcardReader.h"
#include "util.h"

extern const std::vector<QuestionTypeInfo> questionTypeInfos{ std::vector<QuestionTypeInfo>({
	QuestionTypeInfo(QuestionType::FLASHCARD, L"flashcard", L"FLASHCARD", FlashcardWriter::get(), FlashcardReader::get())
}) };

const QuestionTypeInfo* getQuestionTypeInfo(QuestionType type)
{
	auto iter = std::find_if(
		questionTypeInfos.begin(),
		questionTypeInfos.end(),
		[type](QuestionTypeInfo qti) -> bool { return qti.type == type; });
	if (iter == questionTypeInfos.end())
		return nullptr;
	return iter._Ptr;
}

const QuestionTypeInfo* getQuestionTypeInfoFromCode(std::wstring code)
{
	auto iter = std::find_if(
		questionTypeInfos.begin(),
		questionTypeInfos.end(),
		[code](QuestionTypeInfo qti) -> bool { return qti.code == code; });
	if (iter == questionTypeInfos.end())
		return nullptr;
	return iter._Ptr;
}

std::string QuestionTypeInfo::getFileAddress() const
{
	std::wstring typeName = toLower(displaySingular);
	std::replace(typeName.begin(), typeName.end(), L' ', L'_');
	return "userdata_" + std::string(typeName.begin(), typeName.end()) + ".txt";
}
