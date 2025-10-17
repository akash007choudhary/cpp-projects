#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <cctype>
#include <locale>
#include <iomanip>
using namespace std;

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
            stringstream ss(line);
            string idStr, name, countStr, time;
            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, countStr, ',');
            getline(ss, time, ',');

            logs.push_back(Logentry(name, time));
            logs.back().id = stoi(idStr);
            logs.back().count = stoi(countStr);
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
                saveToFile("dataset.txt");
                return;
            }
        }

        // if new user enters the system then only it is added to the list, else this doesnt add duplicate users
        logs.push_back(Logentry(name, timestr));
        lastLoggerName = name;
        lastLoggerTime = timestr;
        cout << "New user added: " << name << " (ID: " << logs.back().id << ")\n";
        saveToFile("dataset.txt"); // 🔹 Save after each login
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
    acl.loadFromFile("dataset.txt"); // 🔹 Load existing records

    const string adminPassword = "password";

    // infinite loop for the menu until we break out of it by choosing exit option
    while (true)
    {
        cout << "\nMain menu:\n"
             << "1) User login\n"
             << "2) Admin login\n"
             << "3) Exit\n"
             << "Choose: ";

        int choice;
        if (!(cin >> choice))
        {
            cin.clear();
            string skip;
            getline(cin, skip);
            cout << "Enter a valid number.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear newline

        if (choice == 1)
        {
            cout << "Enter your name: ";
            string name;
            getline(cin, name);
            name = trim(name); // trim whitespace
            acl.login(name);
        }

        else if (choice == 2)
        {
            cout << "Enter admin password: ";
            string pass;
            cin >> pass;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (pass == adminPassword)
            {
                while (true)
                {
                    cout << "\nAdmin menu:\n"
                         << "a) Show all records\n"
                         << "b) Most frequent user\n"
                         << "c) Last logger\n"
                         << "d) Search user by name\n"
                         << "e) Back to main menu\n"
                         << "Choose: ";

                    string input;
                    getline(cin, input);
                    input = trim(input);
                    if (input.empty())
                        continue;

                    char op = input[0];

                    if (op == 'a')
                        acl.showAll();
                    else if (op == 'b')
                        acl.mostFrequent();
                    else if (op == 'c')
                        acl.lastLogger();
                    else if (op == 'd')
                    {
                        cout << "Enter name to search: ";
                        string s;
                        getline(cin, s);
                        s = trim(s);
                        acl.searchUser(s);
                    }
                    else if (op == 'e')
                        break;
                    else
                        cout << "Invalid option.\n";
                }
            }
            else
            {
                cout << "Wrong password.\n";
            }
        }

        else if (choice == 3)
        {

            cout << "Goodbye!\n";
            break;
        }

        else
        {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}
