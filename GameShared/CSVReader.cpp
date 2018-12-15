#include "CSVReader.h"
#include <sstream>
#include <locale>
#include <codecvt>

//#define _ITERATOR_DEBUG_LEVEL 0

std::wstring const& CSVReader::operator[](std::size_t index) const
{
	return m_data[index];
}

std::size_t CSVReader::size() const
{
	return m_data.size();
}

CSVReader::CSVReader()
	: file (nullptr)
{

}

CSVReader::~CSVReader()
{
	if (file != nullptr && file->is_open())
		file->close();
}

const std::vector<std::wstring>& CSVReader::readNextRow()
{
	std::wstring line;
	std::getline(*file, line);

	std::wstringstream lineStream(line);
	std::wstring cell;

	m_data.clear();
	while (std::getline(lineStream, cell, L','))
		m_data.push_back(cell);
	
	if (!lineStream && cell.empty()) // Check for training comma
		m_data.push_back(L"");
	return m_data;
}

void CSVReader::open(const std::string& filename)
{
	if (file != nullptr && file->is_open())
		file->close();
	file = new std::wifstream(filename);
	const auto utf8_locale =  std::locale(file->getloc(), new std::codecvt_utf8 <wchar_t, 0x10ffff, std::consume_header>());
	const auto utf16_locale = std::locale(file->getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>());
	file->imbue(utf8_locale);
	if (file->is_open())
		printf("File parsed: %s\n", filename.c_str());
	else
		printf("Could not open file for parsing: %s\n", filename.c_str());
	readNextRow();
}
