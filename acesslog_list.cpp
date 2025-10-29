#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sstream>
#include <limits>
#include <cstdlib> 
#include <chrono>
#include <ctime>
#include <algorithm>
#include <cctype>
#include <locale>
#include <iomanip>
using namespace std;

// Optional color codes for nicer console display
#define RESET   "\033[0m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define BOLD    "\033[1m"



class Logentry
{
public:
    int id;
    string name;
    string lasttime;
    int count;
    static int index;

    Logentry(const string &n, const string &t) // cnstructor uses address of string so there is no need of copying every time .
    {
        id = index++;
        name = n;
        lasttime = t;
        count = 1;
    }
};

int Logentry::index = 1; // TO KEEP NUMBER OF PEOPLES WHO LOGGED IN INTO THE SYSTEM;

string getCurrentTimeString()
{
    auto now = chrono::system_clock::now();
    time_t t = chrono::system_clock::to_time_t(now);

    tm local_tm = *localtime(&t);

    char buf[64];
    //  %d = day, %m = month, %Y = year
    //  %I = hour (12-hour format), %M = minutes, %S = seconds
    //  %p = AM/PM
    strftime(buf, sizeof(buf), "%d-%m-%Y %I:%M:%S %p", &local_tm);

    return string(buf);
}

// rmeolve leading and trailing spaces from a string
string trim(const string &s)
{
    auto start = s.begin();
    while (start != s.end() && isspace(*start))
        start++;

    auto end = s.end();
    do
    {
        end--;
    } while (distance(start, end) > 0 && isspace(*end));

    return string(start, end + 1);
}

class acesslist
{
private:
    list<Logentry> logs;
    string lastLoggerName;
    string lastLoggerTime;

public:
    acesslist()
    {
        lastLoggerName = "";
        lastLoggerTime = "";
    }
    void saveToFile(const string &filename) const
    {
        ofstream file(filename);
        if (!file)
        {
            cerr << "Error: Cannot open file for writing.\n";
            return;
        }

        for (const auto &log : logs)
        {
            file << log.id << "," << log.name << "," << log.count << "," << log.lasttime << "\n";
        }
    }

    void loadFromFile(const string &filename)
    {
        ifstream file(filename);
        if (!file)
        {
            cout << "(No existing records found. Starting new log.)\n";
            return;
        }

        string line;
        while (getline(file, line))
        {
            if(line.empty()) continue;      // is the file is empty then it will continue without crashing the code
            stringstream ss(line);
            string idStr, name, countStr, time;
            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, countStr, ',');
            getline(ss, time, ',');
            

            //  this portions ensure if the file handling file is empty and have some spaces then the code wont crash  so we used try catch block
            if (idStr.empty() || name.empty() || countStr.empty() || time.empty())
        continue;
            
            try{
                logs.push_back(Logentry(name, time));
            logs.back().id = stoi(idStr);
            logs.back().count = stoi(countStr); 
            }
            catch(...)
            {
                cerr<<"Error loading record: "<<line<<"\n";
            }
        }

        // Set the next index correctly
        if (!logs.empty())
        {
            Logentry::index = logs.back().id + 1;
        }
    }

    void login(const string &name)
    {
        string lowerName = name;
        transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        string timestr = getCurrentTimeString();

        for (auto &log : logs)
        {
            string stored = log.name;
            transform(stored.begin(), stored.end(), stored.begin(), ::tolower);
            // if same name  user again enetrs the system
            if (stored == lowerName)
            {
                log.count++;
                log.lasttime = timestr;
                lastLoggerName = log.name;
                lastLoggerTime = timestr;
                cout << "Welcome back, " << name << "! (ID: " << log.id << ")\n";
                saveToFile("database.txt");
                return;
            }
        }

        // if new user enters the system then only it is added to the list, else this doesnt add duplicate users
        logs.push_back(Logentry(name, timestr));
        lastLoggerName = name;
        lastLoggerTime = timestr;
        cout << "New user added: " << name << " (ID: " << logs.back().id << ")\n";
        saveToFile("database.txt"); // 🔹 Save after each login
    }

    // showing options to the admin of all the users who have logged in

    void showAll() const
    {
        if (logs.empty())
        {
            cout << "No records yet.\n";
            return;
        }

        // Print header
        // this code helps in formatting the output in a tabular form
        cout << left // left-align text
             << setw(6) << "Index"
             << setw(5) << "ID"
             << setw(7) << "Count"
             << setw(30) << "Name"
             << "Last login time\n";
        cout << "--------------------------------------------------------\n";

        int idx = 1;
        for (const auto &log : logs)
        {
            cout << left
                 << setw(6) << idx++
                 << setw(5) << log.id
                 << setw(7) << log.count
                 << setw(30) << log.name
                 << log.lasttime << "\n";
        }
    }

    void mostFrequent() const
    {
        if (logs.empty())
        {
            cout << "No records yet.\n";
            return;
        }

        // Step 1: Find the maximum count
        int maxCount = 0;
        for (const auto &log : logs)
        {
            if (log.count > maxCount)
                maxCount = log.count;
        }

        // Step 2: Collect all users who have that maxCount
        list<const Logentry *> topUsers;
        for (const auto &log : logs)
        {
            if (log.count == maxCount)
                topUsers.push_back(&log);
        }

        // Step 3: Display results
        if (topUsers.size() == logs.size())
        {
            cout << "All users have logged in the same number of times: " << maxCount << " times.\n";
            return;
        }

        cout << "Most frequent user(s):\n";
        for (const auto *log : topUsers)
        {
            cout << " - " << log->name << " (ID: " << log->id << ") - "
                 << log->count << " times. Last at " << log->lasttime << "\n";
        }
    }

    void lastLogger() const
    {
        if (lastLoggerName.empty())
            cout << "No logins yet.\n";
        else
            cout << "Last logger: " << lastLoggerName << " at " << lastLoggerTime << "\n";
    }

    void searchUser(const string &name) const
    {
        string lowerName = name;
        transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

        for (const auto &log : logs)
        {
            string stored = log.name;
            transform(stored.begin(), stored.end(), stored.begin(), ::tolower);

            if (stored == lowerName)
            {
                cout << log.name << " (ID: " << log.id << ") logged in "
                     << log.count << " times. Last at " << log.lasttime << "\n";
                return;
            }
        }
        cout << "User '" << name << "' not found.\n";
    }
};

int main()
{
    acesslist acl;
    acl.loadFromFile("database.txt"); // 🔹 Load existing records

    const string adminPassword = "akash";

    while (true)
    {
        system("cls"); // clear screen (works on Windows)
        cout << CYAN << BOLD;
        cout << "==============================================\n";
        cout << "        ACCESS LOG AUDITOR SYSTEM\n";
        cout << "==============================================\n" << RESET;

        cout << YELLOW;
        cout << left << setw(5) << " " << "1 User Login\n";
        cout << left << setw(5) << " " << "2 Admin Login\n";
        cout << left << setw(5) << " " << "3 Exit Program\n";
        // cout << RESET;
        cout << "----------------------------------------------\n";
        cout << " Enter your choice (1-3): ";

        int choice;
        if (!(cin >> choice))
        {
                // this part does that if user enters a character instead of number then it will not crash the code, it will remove the invalid input from the buffer and ask for input again
            cin.clear();
            string skip;
            getline(cin, skip);
            


            cout << RED << " Please enter a valid number.\n" << RESET;
            // cin.get();
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1)
        {
            system("cls");
            cout << GREEN << BOLD << "\n USER LOGIN\n" << RESET;
            cout << "Enter your username: ";
            string name;
            getline(cin, name);
            name = trim(name);
            acl.login(name);

            cout << GREEN << "\nLogin action recorded successfully.\n" << RESET;
            // cout << "Press Enter to return to main menu...";
            // cin.get(); 
        }

        else if (choice == 2)
        {
            system("cls");
            cout << CYAN << BOLD << "\n ADMIN LOGIN\n" << RESET;
            cout << "Enter admin password: ";
            string pass;
            cin >> pass;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (pass == adminPassword)
            {
                cout << GREEN << " Access granted!\n" << RESET;
                // cin.get(); // wait for Enter

                while (true)
                {
                    
                    cout << CYAN << BOLD;
                    cout << "==============================================\n";
                    cout << "                ADMIN DASHBOARD\n";
                    cout << "==============================================\n" << RESET;

                    cout << YELLOW;
                    cout << left << setw(5) << " " << "A) Show all records\n";
                    cout << left << setw(5) << " " << "B) Most frequent user\n";
                    cout << left << setw(5) << " " << "C) Last logged user\n";
                    cout << left << setw(5) << " " << "D) Search user by name\n";
                    cout << left << setw(5) << " " << "E) Back to main menu\n";
                    cout << RESET;
                    cout << "----------------------------------------------\n";
                    cout << " Enter your choice (A-E): ";

                    string input;
                    getline(cin, input);
                    input = trim(input);
                    if (input.empty()) continue;

                    char op = toupper(input[0]);

                    switch (op)
                    {
                    case 'A':
                        cout << GREEN << " All Access Records:\n" << RESET;
                        acl.showAll();
                        break;
                    case 'B':
                        cout << GREEN << " Most Frequent User:\n" << RESET;
                        acl.mostFrequent();
                        break;
                    case 'C':
                        cout << GREEN << "Last Logged User:\n" << RESET;
                        acl.lastLogger();
                        break;
                    case 'D':
                        cout << "Enter name to search: ";
                        {
                            string s;
                            getline(cin, s);
                            s = trim(s);
                            cout << GREEN << " Search Results for '" << s << "':\n" << RESET;
                            acl.searchUser(s);
                        }
                        break;
                    case 'E':
                        cout << YELLOW << "↩ Returning to main menu...\n" << RESET;
                        goto admin_exit;
                    default:
                        cout << RED << " Invalid option.\n" << RESET;
                        break;
                    }

                    // cout << "\nPress Enter to continue...";
                    // cin.get();
                }
            admin_exit:
                continue;
            }
            else
            {
                cout << RED << " Wrong password! Access denied.\n" << RESET;
                // cout << "Press Enter to return...";
                // cin.get();
            }
        }

        else if (choice == 3)
        {
            system("cls");
            cout << GREEN << BOLD << "\n Goodbye! Logging out...\n" << RESET;
            break;
        }

        else
        {
            cout << RED << "Invalid choice. Try again.\n" << RESET;
            // cin.get();
        }
    }
    return 0;
}
   
