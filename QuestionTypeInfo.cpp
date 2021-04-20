#include <easy_list.h>
#include <algorithm>
#include "QuestionTypeInfo.h"
#include "FlashcardWriter.h"
#include "FlashcardReader.h"
#include "util.h"


const easy_list::list<QuestionTypeInfo>* QuestionTypeInfo::getList()
{
	static const auto list = easy_list::list<QuestionTypeInfo>({
		QuestionTypeInfo(QuestionType::FLASHCARD, L"flashcard", L"flashcard", FlashcardWriter::get(), FlashcardReader::get())
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
	std::string typeName = std::string(_displaySingular.begin(), _displaySingular.end());
	easy_list::list<char> fileAddress = easy_list::list<char>(typeName.begin(), typeName.end());
	fileAddress = fileAddress.replace('_', ' ');
	return "userdata_" + std::string(fileAddress.begin(), fileAddress.end()) + ".txt";
}
