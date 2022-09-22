#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

const std::string STUDENT_FILE = "StudentData.txt";

struct StudentData
{
	std::string first_name;
	std::string last_name;

	StudentData(std::string raw_student_data, char delimiter = ',')
	{
		// Constructs a StudentData instance from a delimited string.
		// delimiter defaults to ','


		auto found_delimiter = raw_student_data.find(delimiter);

		if (found_delimiter == std::string::npos)
		{
			throw std::invalid_argument("Delimiter not found in raw_student_data");
		}
		else
		{
			this->last_name = raw_student_data.substr(0, found_delimiter);
			this->first_name = raw_student_data.substr(found_delimiter + 1);
		}
	}
};


int main()
{
	auto file = std::ifstream(STUDENT_FILE);

	auto students = std::vector<StudentData>();

	for (std::string line; std::getline(file, line);)
	{
		students.push_back(StudentData(line));
	}
	
	return 1;
}