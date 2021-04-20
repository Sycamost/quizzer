#include <easy_list.h>
#include <algorithm>
#include "QuestionTypeInfo.h"
#include "FlashcardWriter.h"
#include "FlashcardReader.h"
#include "util.h"


const easy_list::list<QuestionTypeInfo>* QuestionTypeInfo::getList()
{
	static const auto list = easy_list::list<QuestionTypeInfo>({
		QuestionTypeInfo(QuestionType::FLASHCARD, L"flashcard", L"FLASHCARD", FlashcardWriter::get(), FlashcardReader::get())
	});
	return &list;
}

const easy_list::list<QuestionTypeInfo>::const_iterator QuestionTypeInfo::get(QuestionType type)
{
	return getList()->search(type, &QuestionTypeInfo::getType);
}

const easy_list::list<QuestionTypeInfo>::const_iterator QuestionTypeInfo::get(std::wstring code)
{
	return getList()->search(code, &QuestionTypeInfo::getCode);
}

const std::string QuestionTypeInfo::getFileAddress() const
{
	std::wstring typeName = toLower(_displaySingular);
	std::replace(typeName.begin(), typeName.end(), L' ', L'_');
	return "userdata_" + std::string(typeName.begin(), typeName.end()) + ".txt";
}
