// Roope Suvanto
// Mygrep assignment

#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

using namespace std;

int findSubstring(const char *source, const char *search)
{
    if (*search == '\0')
        return 0; // Empty search string matches at the beginning

    // Iterate through each character in the source string using pointers
    for (const char *p1 = source; *p1 != '\0'; ++p1)
    {
        const char *p1_advance = p1;
        const char *p2 = search;

        // Check if characters match and advance both pointers
        while (*p1_advance != '\0' && *p2 != '\0' && *p1_advance == *p2)
        {
            ++p1_advance;
            ++p2;
        }
        if (*p2 == '\0')
        {
            return p1 - source; // Found a match, return the index
        }
    }
    return -1; // Return -1 if no match is found
}

void runInteractiveMode()
{
    string sourceString;
    string searchString;

    // Prompt the user for source string
    cout << "Give a string to search in: ";
    getline(cin, sourceString);

    // Prompt the user for search string
    cout << "Give search string: ";
    getline(cin, searchString);

    // Pass the C-style string pointers to the findSubstring function
    int position = findSubstring(sourceString.c_str(), searchString.c_str());

    // Output the formatted result
    if (position != -1)
    {
        cout << "Search string found at index: " << position << endl;
    }
    else
    {
        cout << "Search string not found." << endl;
    }
}

void runFileSearchMode(const char *searchString, const char *fileName)
{
    try
    {
        ifstream file(fileName, ios::in | ios::binary | ios::ate);
        if (!file.is_open())
        {
            throw runtime_error("Could not open file: " + string(fileName) + "\n");
        }

        streamsize size = file.tellg();
        file.seekg(0, ios::beg);

        char *buffer = new char[size + 1];

        file.read(buffer, size);
        streamsize bytesRead = file.gcount();
        buffer[bytesRead] = '\0'; // Null-terminate the buffer

        char *lineStart = buffer;
        char *ptr = buffer;

        while (*ptr != '\0')
        {
            if (*ptr == '\n')
            {
                *ptr = '\0';

                if (findSubstring(lineStart, searchString) != -1)
                {
                    cout << lineStart << endl;
                }

                *ptr = '\n';
                lineStart = ptr + 1;
            }
            ++ptr;
        }
        if (lineStart < ptr && *lineStart != '\0')
        {
            if (findSubstring(lineStart, searchString) != -1)
            {
                cout << lineStart << endl;
            }
        }

        else
        {
            delete[] buffer;
            throw runtime_error("Error reading file: " + string(fileName) + "\n");
        }

        delete[] buffer;
        file.close();
    }
    catch (const exception &e)
    {
        cerr << "An exception occurred: " << e.what() << endl;
    }
}

int main(int argc, char *argv[])
{
    // Check command-line argumenets to determine mode of operation
    // If no arguments are provided, run in interactive mode
    if (argc == 1)
    {
        runInteractiveMode();
    }
    // If two arguments are provided, treat them as search string and file name for file search mode
    else if (argc == 3)
    {
        runFileSearchMode(argv[1], argv[2]);
    }
    // Handle incorrect usage
    else
    {
        cerr << "Usage: " << argv[0] << " [search_string file_name]" << endl;
        return 1;
    }
    return 0;
}
