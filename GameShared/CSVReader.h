#pragma once
#include <fstream>
#include <vector>
#include <string>

class CSVReader
{
private:
	std::vector<std::wstring> m_data;
	std::wifstream* file;

public:
	CSVReader();
	~CSVReader();

	std::wstring const& operator[](std::size_t index) const;
	std::size_t size() const;
	const std::vector<std::wstring>& readNextRow();
	void open(const std::string& filename);
};
