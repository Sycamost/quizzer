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
	const std::wstring getCode() const { return _code; }
	const std::string getFileAddress() const;
	QuestionWriter* getWriter() { return &_writer; }
	QuestionReader* getReader() { return &_reader; }
	bool operator==(const QuestionTypeInfo& other)
	{
		return
			_type == other._type &&
			_displaySingular == other._displaySingular &&
			_displayPlural == other._displayPlural &&
			_code == other._code;
	}
	static const easy_list::list<QuestionTypeInfo> getList();
	static const easy_list::list<QuestionTypeInfo>::const_iterator get(QuestionType type);
	static const easy_list::list<QuestionTypeInfo>::const_iterator get(std::wstring code);
private:
	QuestionType _type;
	std::wstring _displaySingular;
	std::wstring _displayPlural;
	std::wstring _code;
	QuestionWriter _writer;
	QuestionReader _reader;
	QuestionTypeInfo(
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
		_reader(reader)
	{}
	QuestionTypeInfo(
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
		_reader(reader)
	{}
};
