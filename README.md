# CodeAlpha_StudentManagementSystem

[![C Programming](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A robust, menu-driven Student Management System developed as **Task 3** for the **CodeAlpha C Programming Internship**. This application implements core systems programming concepts including dynamic memory management, structures, and binary file handling.

---

## 🚀 Features

* **Add Student**: Enables adding new student records with a unique roll number up to 999,999, student name, course title, and marks.
* **Display All Records**: Outputs all saved student entries in a clean, structured tabular format[cite: 2] along with the total student count[cite: 2].
* **Search**: Looks up specific student information instantly using their unique roll number[cite: 2].
* **Update**: Modifies details for an existing student record while keeping the original roll number fixed[cite: 2].
* **Delete**: Removes a student record securely after prompting for user confirmation (`y/n`)[cite: 2] and updates the dynamic database table[cite: 2].
* **Persistent Binary Storage**: Automatically writes and saves changes instantly to `students.dat` so data is preserved between program executions[cite: 2].

---

## 🛠️ Concepts & Technologies

* **Language**: C (`-std=c11`)[cite: 2]
* **Data Structures**: Custom structs (`Student` and `Table`) combined with dynamic memory allocation (`realloc`, `free`)[cite: 2].
* **File Handling**: Binary file input/output (`fopen`, `fread`, `fwrite`) using `students.dat`[cite: 2].
* **Defensive Programming**: Comprehensive input validation loops ensuring valid integers, marks within range `0–100`, and non-blank text entries[cite: 2].

---

## ⚙️ Getting Started

### Prerequisites
* A C compiler supporting the C11 standard (such as `gcc` or `clang`)[cite: 2].

### Compilation
Clone or download the repository, open your terminal in the project directory, and compile the code using the recommended flags[cite: 2]:
./student
student.exe
========== STUDENT MANAGEMENT SYSTEM ==========
 1. Add Student
 2. Display All
 3. Search
 4. Update
 5. Delete
 6. Exit
Enter your choice:
Select an option from 1 to 6 to manage your database[cite: 2].

Follow the on-screen prompts to input roll numbers, names, courses, and marks[cite: 2].

All additions, updates, and deletions are committed immediately to students.dat[cite: 2].
📜 License
This project is open-source and available under the MIT License.

```bash
gcc -Wall -Wextra -std=c11 -o student student_management.c
