#pragma once
#include <string>
#include <iostream>
#include <easy_list.h>

class YesNo {
public:
	static const YesNo YES;
	static const YesNo NO;
	static const YesNo COMMAND;
	static const YesNo OTHER;
	operator bool() const { return _value == Value::YES; }
	bool operator!() const { return _value == Value::NO; }
	bool operator==(const YesNo& other) const { return _value == other._value; }
private:
	enum class Value {
		YES,
		NO,
		COMMAND,
		OTHER
	};
	Value _value;
	YesNo(Value value) { _value = value; }
};
std::wstring getInputLine(std::wistream &stream = std::wcin);
bool inputYesNo(std::wstring message, bool doPrintResult = true);
std::wstring toUpper(std::wstring wstr);
std::wstring toLower(std::wstring wstr);
std::wstring indent(std::wstring wstr, int numTabs);
const YesNo getYesNo(std::wstring wstr);
easy_list::list<std::wstring> splitByWord(std::wstring wstr);

template <typename T1, typename T2> easy_list::list<T2> convv(easy_list::list<T1> v)
{
	easy_list::list<T2> result = easy_list::list<T2>();
	result.reserve(v.size());
	for (int i = 0; i < v.size(); i++)
		result.push_back((T2)v[i]);
	return result;
}

template <typename T> easy_list::list<T> slice(easy_list::list<T> v, int start = 0, int end = -1)
{
	easy_list::list<T> result = easy_list::list<T>();

	if (end < 0)
		end = v.size();

	for (int i = start; i < end; i++)
		result.push_back(v[i]);

	return result;
}

template <typename T> bool shareAnyElems(easy_list::list<T> v1, easy_list::list<T> v2)
{

	for (unsigned int i = 0; i < v1.size(); i++)
	{
		for (unsigned int j = 0; j < v2.size(); j++)
		{
			if (v1[i] == v2[j])
				return true;
		}
	}
	return false;
}