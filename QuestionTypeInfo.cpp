#include <vector>
#include "QuestionTypeInfo.h"
#include "FlashcardWriter.h"
#include "FlashcardReader.h"

extern const std::vector<QuestionTypeInfo> questionTypeInfos = std::vector<QuestionTypeInfo>({
	QuestionTypeInfo(QuestionType::FLASHCARD, L"flashcard", L"FLASHCARD", "flashcards.txt", flashcardWriter, flashcardReader)
});

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