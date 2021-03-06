#include <easy_list.h>
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include "QuestionTypeInfo.h"
#include "FlashcardWriter.h"
#include "FlashcardReader.h"
#include "MultipleChoiceWriter.h"
#include "MultipleChoiceReader.h"
#include "SweetieJarWriter.h"
#include "SweetieJarReader.h"
#include "util.h"

const std::string makeFileName(std::wstring displaySingular)
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

	// Add prefix and file extension and return
	return "userdata_" + wstrToStr(fileAddress.toString()) + ".txt";
}

const std::string initDirectory()
{
#if defined CONFIG_DEBUG
	static const std::string directory = ".\\";
#elif defined CONFIG_RELEASE
	static const std::string directory = std::getenv("APPDATA") + std::string("\\Quizzer\\");
#endif
	if (!std::filesystem::exists(directory))
	{
		std::filesystem::create_directory(directory);
	}
	return directory;
}

const std::string getDirectory()
{
	static const std::string directory = initDirectory();
	return directory;
}

const std::string initFileAddress(std::string fileAddress)
{
	using namespace std::filesystem;
	if (!exists(fileAddress))
	{
		std::wofstream file = std::wofstream();
		file.open(fileAddress, std::ios::app);
		file.close();
	}
	return fileAddress;
}

QuestionTypeInfo::QuestionTypeInfo(
	QuestionType type,
	std::wstring display,
	easy_list::list<std::wstring> codes,
	QuestionWriter writer,
	QuestionReader reader)
	:
	_type(type),
	_displaySingular(display),
	_displayPlural(display + L"s"),
	_codes(codes),
	_writer(writer),
	_reader(reader),
	_fileAddress(initFileAddress(getDirectory() + makeFileName(_displaySingular)))
{}

QuestionTypeInfo::QuestionTypeInfo(
	QuestionType type,
	std::wstring displaySingular,
	std::wstring displayPlural,
	easy_list::list<std::wstring> codes,
	QuestionWriter writer,
	QuestionReader reader)
	:
	_type(type),
	_displaySingular(displaySingular),
	_displayPlural(displayPlural),
	_codes(codes),
	_writer(writer),
	_reader(reader),
	_fileAddress(initFileAddress(getDirectory() + makeFileName(_displaySingular)))
{}

const easy_list::list<QuestionTypeInfo>* QuestionTypeInfo::getList()
{
	static const auto list = easy_list::list<QuestionTypeInfo>({
		QuestionTypeInfo{
			QuestionType::FLASHCARD,
			L"flashcard",
			{ L"flashcard", L"fc" },
			FlashcardWriter::get(),
			FlashcardReader::get()
		},
		QuestionTypeInfo{
			QuestionType::MULTIPLE_CHOICE,
			L"multiple choice question",
			{ L"multiplechoice", L"mc" },
			MultipleChoiceWriter::get(),
			MultipleChoiceReader::get()
		},
		QuestionTypeInfo{
			QuestionType::SWEETIE_JAR,
			L"sweetie jar",
			{ L"sweetiejar", L"sj" },
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
	return getList()->search(true, &QuestionTypeInfo::hasCode, code);
}

const std::string QuestionTypeInfo::getFileAddress() const
{
	return _fileAddress;
}
