#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <queue>
#include <utility>
#include <vector>

#define PRE_RELEASE

#ifdef PRE_RELEASE
const std::string STUDENT_FILE = "StudentData_Emails.txt";
#else
const std::string STUDENT_FILE = "StudentData.txt";
#endif // PRE_RELEASE

// This enum represents the order that the txt files store data
// The values of the enum correspond with the order it appears (e.g., last_name first)
enum TxtOrdering
{
	last_name, // Will start at 0 https://stackoverflow.com/questions/34811486/do-c-enums-start-at-0
	first_name,

#ifdef PRE_RELEASE
	email,
#endif // PRE_RELEASE

    __SIZE // The size of this enum. No 'real' values should be declared after this
};

// PrintOrdering stores what elements to print and which order (first element)
// It also stores a string identifier of the data type (second element)
const std::pair<TxtOrdering, std::string> PrintOrdering[] = {
	std::make_pair(first_name, "First Name: "),
	std::make_pair(last_name, "Last Name: "),

#ifdef PRE_RELEASE
	std::make_pair(email, "Email: "),
#endif // PRE_RELEASE
};

struct StudentData
{
    std::vector<std::string> data_items;

    StudentData(std::string raw_student_data, char delimiter = ',')
    {
        // Constructs a StudentData instance a delimited string.
        // delimiter defaults to ','


        std::queue<size_t> delimiter_locations;
        for (
            size_t found_delimiter = raw_student_data.find(delimiter);
            found_delimiter != std::string::npos;
            found_delimiter = raw_student_data.find(delimiter, found_delimiter + 1)
            )
        {
            delimiter_locations.push(found_delimiter);
        }

        // there should be n-1 delimiters locations (n being the number of elements of StudentData
        if (delimiter_locations.size() != (TxtOrdering::__SIZE - 1))
        {
            throw std::invalid_argument("StudentData(): raw_student_data does not contain the right number of data elements. "
                "Expected " + std::to_string(TxtOrdering::__SIZE - 1) +
                ", Received " + std::to_string(delimiter_locations.size()));
        }

        size_t start = 0;
        while (!delimiter_locations.empty())
        {
            auto const& end = delimiter_locations.front();
            this->data_items.push_back(raw_student_data.substr(start, end - start));
            start = end + 1;

            delimiter_locations.pop();
        }

        // Remaining data after last delimter
        this->data_items.push_back(raw_student_data.substr(start));
    }

    std::string getFirstName() const {
        return this->data_items[TxtOrdering::first_name];
    }

    std::string getLastName() const {
        return this->data_items[TxtOrdering::last_name];
    }

#ifdef PRE_RELEASE
    std::string getEmail() const {
        return this->data_items[TxtOrdering::email];
    }
#endif // PRE_RELEASE


    std::string getRepresentation() const
    {
        // Returns a basic string representation of the student


        std::string result = "";

        // Get total size of string
        size_t result_size = 1; // 1 for null at the end
        for (auto& s : this->data_items)
        {
            result_size += s.length();
        }

        result.reserve(result_size);

        // Appending strings is faster than using stringstream
        // https://stackoverflow.com/questions/19844858/c-stdostringstream-vs-stdstringappend
        for (auto const& po : PrintOrdering)
        {
            result.append(po.second + this->data_items[po.first] + " | ");
        }

        return result;
    }
};


std::ostream& operator<< (std::ostream& out, StudentData const& s)
{
	out << s.getRepresentation();

	return out;
}


template<typename T>
void log(T message, std::ostream& dest = std::cout)
{
	dest << "LOG: " << message << std::endl;
}


int main()
{
#ifdef PRE_RELEASE
    log("Running in PRE_RELEASE Mode");
#else
    log("Running in STANDARD Mode");
#endif // PRE_RELEASE

	auto file = std::ifstream(STUDENT_FILE);

	std::vector<StudentData> students;
	for (std::string line; std::getline(file, line);)
	{
        try
        {
            students.push_back(StudentData(line));
        }
        catch (std::invalid_argument e)
        {
            log(e.what());
            exit(-1);
        }
		    
	}

#ifdef _DEBUG
	log("DEBUG: Read students:");
	for (auto& s : students)
	{
		log(s);
	}
	log("DEBUG: End of read students");
#endif // _DEBUG
	
	return 1;
}