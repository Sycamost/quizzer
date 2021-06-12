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
#include "TrueOrFalseWriter.h"
#include "TrueOrFalseReader.h"
#include "util.h"
#include <functional>

const std::string makeFileName(std::wstring displaySingular)
{
	// Get type name into list form
	auto fileAddress = easy_list::list<wchar_t>(displaySingular.begin(), displaySingular.end());

	// Replace any non-alphanumeric characters with _ (sorry!)
	fileAddress = fileAddress.replace([](wchar_t _) -> wchar_t { return L'_'; }, std::not1(std::function<bool(wchar_t)>(iswalnum)));

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
	_fileAddress(initFileAddress(getDirectory() + makeFileName(_displaySingular)))
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
	_fileAddress(initFileAddress(getDirectory() + makeFileName(_displaySingular)))
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
		},
		QuestionTypeInfo{
			QuestionType::TRUE_OR_FALSE,
			L"true/false question",
			L"true/false questions",
			L"trueorfalse",
			TrueOrFalseWriter::get(),
			TrueOrFalseReader::get()
		},
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
