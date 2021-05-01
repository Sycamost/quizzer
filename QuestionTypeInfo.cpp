#include <easy_list.h>
#include <algorithm>
#include "QuestionTypeInfo.h"
#include "FlashcardWriter.h"
#include "FlashcardReader.h"
#include "MultipleChoiceWriter.h"
#include "MultipleChoiceReader.h"
#include "SweetieJarWriter.h"
#include "SweetieJarReader.h"
#include "util.h"

const std::string makeFileAddress(std::wstring displaySingular)
{
	// Get type name into list form
	auto fileAddress = easy_list::list<wchar_t>(displaySingular.begin(), displaySingular.end());

	// If type name ends with " question", remove that bit!
	static const easy_list::list<wchar_t> question{ L' ', L'q', L'u', L'e', L's', L't', L'i', L'o', L'n' };
	static const int questionSize = question.size();
	if (fileAddress.slice(-questionSize, questionSize) == question)
		fileAddress = fileAddress.slice(0, -questionSize);

	// Replace " " with "_"
	fileAddress = fileAddress.replace(L'_', L' ');

	// Make narrow, convert to string
	std::string narrowStrFileAddress = fileAddress.transform<char>().toString();

	// Add prefix and file extension and return
	return "userdata_" + narrowStrFileAddress + ".txt";
}

QuestionTypeInfo::QuestionTypeInfo(
	QuestionType type,
	std::wstring display,
	std::wstring code,
	QuestionWriter writer,
	QuestionReader reader)
	:
	_type(type),
	_displaySingular(display),
	_displayPlural(display + L"s"),
	_code(code),
	_writer(writer),
	_reader(reader),
	_fileAddress(makeFileAddress(_displaySingular))
{}

QuestionTypeInfo::QuestionTypeInfo(
	QuestionType type,
	std::wstring displaySingular,
	std::wstring displayPlural,
	std::wstring code,
	QuestionWriter writer,
	QuestionReader reader)
	:
	_type(type),
	_displaySingular(displaySingular),
	_displayPlural(displayPlural),
	_code(code),
	_writer(writer),
	_reader(reader),
	_fileAddress(makeFileAddress(_displaySingular))
{}

const easy_list::list<QuestionTypeInfo>* QuestionTypeInfo::getList()
{
	static const auto list = easy_list::list<QuestionTypeInfo>({
		QuestionTypeInfo{
			QuestionType::FLASHCARD,
			L"flashcard",
			L"flashcard",
			FlashcardWriter::get(),
			FlashcardReader::get()
		},
		QuestionTypeInfo{
			QuestionType::MULTIPLE_CHOICE,
			L"multiple choice question",
			L"multiplechoice",
			MultipleChoiceWriter::get(),
			MultipleChoiceReader::get()
		},
		QuestionTypeInfo{
			QuestionType::SWEETIE_JAR,
			L"sweetie jar",
			L"sweetiejar",
			SweetieJarWriter::get(),
			SweetieJarReader::get()
		}
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
	return _fileAddress;
}
