
// Libraries
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <cctype>

using namespace std;

//Course info struct
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
};

// Prints the menu
void PrintMenu() {
    cout << "\n1. Load Data Structure" << endl;
    cout << "2. Print Course List" << endl;
    cout << "3. Print Course" << endl;
    cout << "9. Exit" << endl;
    cout << "\nEnter choice: ";
}

// Splits a CSV line into parts
vector<string> Split(const string& line, char delimiter) {
    vector<string> tokens;
    string token;

    for (char c : line) {
        if (c == delimiter) {
            tokens.push_back(token);
            token.clear();
        } else {
            token += c;
        }
    }
    tokens.push_back(token); // last token
    return tokens;
}

//Main program (menu loop)
int main() {
    unordered_map<string, Course> courses;
    int choice = 0;

    while (choice != 9) {
        PrintMenu();
        cin >> choice;

        switch (choice) {
            //Option 1: Load file
            case 1: {
                string fileName;

                cout << "Enter file name: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear leftover newline
                getline(cin, fileName);

                ifstream file(fileName);

                if (!file.is_open()) {
                    cout << "Error: Could not open file " << fileName << endl;
                } else {
                    cout << "File opened successfully: " << fileName << endl;

                    string line;

                    while (getline(file, line)) {
                        vector<string> parts = Split(line, ',');

                        if (parts.size() < 2) {
                            cout << "Skipping bad line: " << line << endl;
                            continue;
                        }

                        Course c;
                        c.courseNumber = parts[0];
                        c.title = parts[1];

                        for (int i = 2; i < (int)parts.size(); i++) {
                            string prereq = parts[i];

                            // remove ALL whitespace characters from prereq
                            prereq.erase(remove_if(prereq.begin(), prereq.end(), ::isspace), prereq.end());

                            if (!prereq.empty()) {
                                c.prerequisites.push_back(prereq);
                            }
                        }
                        // Store course by course number
                        courses[c.courseNumber] = c;
                    }

                    cout << "Courses loaded: " << courses.size() << endl;
                    file.close();
                }

                break;
            }
            //Option 2:Print sorted course list
            case 2: {
                if (courses.empty()) {
                    cout << "No courses loaded. Please load the file first (Option 1)." << endl;
                    break;
                }

                vector<string> courseNumbers;
                for (const auto& pair : courses) {
                    courseNumbers.push_back(pair.first);
                }

                sort(courseNumbers.begin(), courseNumbers.end());

                cout << "Course List:" << endl;
                for (const string& num : courseNumbers) {
                    cout << num << endl;
                }

                break;
            }
            //Option 3: Print one course
            case 3: {
                if (courses.empty()) {
                    cout << "No courses loaded. Please load the file first (Option 1)." << endl;
                    break;
                }

                string courseNum;
                cout << "Enter course number: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, courseNum);

                for (char &ch : courseNum) {
                    ch = toupper((unsigned char)ch);
                }

                auto it = courses.find(courseNum);
                if (it == courses.end()) {
                    cout << "Course not found: " << courseNum << endl;
                } else {
                    Course c = it->second;

                    cout << c.courseNumber << ", " << c.title << endl;

                    cout << "Prerequisites:" << endl;
                    if (c.prerequisites.empty()) {
                        cout << "None" << endl;
                    } else {
                        for (const string& p : c.prerequisites) {
                            auto pit = courses.find(p);
                            if (pit != courses.end()) {
                                cout << p << ", " << pit->second.title << endl;
                            } else {
                                cout << p << endl;
                            }
                        }
                    }
                }

                break;
            }
            case 9:
                cout << "Goodbye." << endl;
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }

    return 0;
}


