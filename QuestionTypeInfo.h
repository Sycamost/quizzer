#pragma once
#include <string>
#include <easy_list.h>
#include "QuestionType.h"
#include "QuestionWriter.h"
#include "QuestionReader.h"

class QuestionTypeInfo {
public:
	const QuestionType getType() const { return _type; }
	const std::wstring getDisplaySingular() const { return _displaySingular; }
	const std::wstring getDisplayPlural() const { return _displayPlural; }
	const bool hasCode(std::wstring code) const { return _codes.contains(code); }
	const std::string getFileAddress() const;
	QuestionWriter* getWriter() { return &_writer; }
	QuestionReader* getReader() { return &_reader; }
	bool operator==(const QuestionTypeInfo& other)
	{
		return _type == other._type;
	}
	static const easy_list::list<QuestionTypeInfo>* getList();
	static const easy_list::list<QuestionTypeInfo>::const_iterator get(QuestionType type);
	static const easy_list::list<QuestionTypeInfo>::const_iterator get(std::wstring code);
private:
	QuestionType _type;
	std::wstring _displaySingular;
	std::wstring _displayPlural;
	easy_list::list<std::wstring> _codes;
	QuestionWriter _writer;
	QuestionReader _reader;
	std::string _fileAddress;
	QuestionTypeInfo(
		QuestionType type,
		std::wstring display,
		easy_list::list<std::wstring> codes,
		QuestionWriter writer,
		QuestionReader reader);
	QuestionTypeInfo(
		QuestionType type,
		std::wstring displaySingular,
		std::wstring displayPlural,
		easy_list::list<std::wstring> codes,
		QuestionWriter writer,
		QuestionReader reader);
};
