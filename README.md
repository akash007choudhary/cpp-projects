# 🔒 Access Log List Project (C++ Console)

A **C++ console application** designed to securely manage and analyze user login records, tracking login frequency, and providing administrative reporting.

---

## 🎯 Overview

The **Access Log List Project** is a robust console-based system built in C++ to handle user access. It automatically records login events, maintains a permanent list of users, increments login counts, and time-stamps each event. This ensures data integrity through **file persistence** in `dataset.txt`, making it a reliable tool for simple user statistics and access auditing.

## ✨ Features

* **Data Persistence:** All user records are automatically **saved to and loaded from `dataset.txt`**, ensuring no data is lost between sessions.
* **Case-Insensitive Login:** Usernames are handled case-insensitively (e.g., "Akash" and "AKASH" are the same user), preventing duplicate records.
* **Automatic Time Stamping:** Every login is recorded with the **current date and time** using C++ `<chrono>` and `<ctime>`.
* **Admin Dashboard:** A secure, password-protected menu offering advanced views and analytics.
* **Statistical Analysis:** Easily identify the **most frequent user(s)** and view the last user who logged in.
* **Efficient Storage:** Utilizes the C++ Standard Template Library **`std::list`** for efficient in-memory record management.

---

## 🚀 Getting Started

### Prerequisites

* A C++ compiler (like **g++** or Clang) installed on your system.

### Installation & Execution

1.  **Save the Code:** Ensure the provided C++ code is saved in a file named `access_log.cpp`.

2.  **Compile the source code:** Open your terminal and run the compilation command:
    ```bash
    g++ -o access_log access_log.cpp
    ```

3.  **Run the application:** Execute the compiled program:
    ```bash
    ./access_log
    ```

---

## 🖥️ Usage & Menu Options

### Main Menu

| Option | Description |
| :----- | :------------------------------------------- |
| `1`    | **User Login:** Enter your name to log in or register as a new user. |
| `2`    | **Admin Login:** Access the secure administrative dashboard. |
| `3`    | **Exit Program:** Safely terminate the program and ensure data is saved. |

### Admin Credentials

> ⚠️ **Password:** The default administrator password is: `password`

### Admin Menu Options

| Option | Description |
| :----- | :--------------------------------------------- |
| `a`    | **Show all records:** Displays all user data in a formatted table. |
| `b`    | **Most frequent user(s):** Shows who has logged in the most times. |
| `c`    | **Last logger:** Shows the name and time of the latest login. |
| `d`    | **Search user by name:** Look up a specific user's login stats. |
| `e`    | **Back to main menu:** Returns to the Main Menu. |


### SAMPLE OUTPUT :

Main menu:
User login: 1
Admin login: 2 
Exit : 3
Choose: 1 
Enter your name: Akash New user added: Akash (ID: 1)

Main menu:
User login: 1
Admin login: 2 
Exit : 3
Exit Choose: 1 
Enter your name: akash Welcome back, akash! (ID: 1)

Main menu:
User login: 1
Admin login: 2 
Exit : 3
Exit Choose: 2 
Enter admin password: password

Admin menu:
a) Show all records 
b) Most frequent user
c) Last logger 
d) Search user by name
e) Back to main menu 
Choose: a

---------------------------------------------------
Index  | ID  |  Count  |   Name    |      Last login time
1      |  1  |   2     |  Akash    |   18-10-2025 01:35:10 AM
