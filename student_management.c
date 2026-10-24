/*
 * ============================================================================
 *  CodeAlpha C Programming Internship - Task 3 : Student Management System
 * ============================================================================
 *  Features : Add | Display | Search | Update | Delete   (menu driven)
 *  Concepts : structures, dynamic memory, binary file handling
 *  Storage  : students.dat (binary) - every change is saved immediately, so
 *             records persist between runs.
 *
 *  Build    : gcc -Wall -Wextra -std=c11 -o student student_management.c
 *  Run      : ./student         (Windows: student.exe)
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DB_FILE    "students.dat"
#define NAME_LEN   31                           /* 30 characters + '\0' */
#define COURSE_LEN 16                           /* 15 characters + '\0' */
#define MAX_ROLL   999999

typedef struct {
    int   roll;
    char  name[NAME_LEN];
    char  course[COURSE_LEN];
    float marks;
} Student;

typedef struct {                                /* in-memory copy of the file */
    Student *rec;
    size_t   n;
} Table;

/* ------------------------------ Input helpers ------------------------------ */

/* Prompts and reads one line safely: handles EOF and discards over-long input. */
static void readLine(const char *prompt, char *buf, int size) {
    printf("%s", prompt);
    fflush(stdout);
    if (!fgets(buf, size, stdin)) { puts(""); exit(EXIT_SUCCESS); }
    if (!strchr(buf, '\n')) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
    buf[strcspn(buf, "\n")] = '\0';
}

/* Reads an integer in [min, max], re-prompting until the input is valid. */
static int readInt(const char *prompt, int min, int max) {
    char buf[32], *end;
    for (;;) {
        readLine(prompt, buf, sizeof buf);
        long v = strtol(buf, &end, 10);
        if (end != buf && *end == '\0' && v >= min && v <= max) return (int)v;
        printf("  Please enter a number between %d and %d.\n", min, max);
    }
}

/* Reads marks in the range 0-100. */
static float readMarks(const char *prompt) {
    char buf[32], junk;
    float v;
    for (;;) {
        readLine(prompt, buf, sizeof buf);
        if (sscanf(buf, "%f %c", &v, &junk) == 1 && v >= 0 && v <= 100) return v;
        puts("  Marks must be a number between 0 and 100.");
    }
}

/* Reads a non-blank line. */
static void readText(const char *prompt, char *buf, int size) {
    do {
        readLine(prompt, buf, size);
    } while (buf[strspn(buf, " \t")] == '\0');
}

static int confirm(const char *question) {
    char buf[8];
    readLine(question, buf, sizeof buf);
    return buf[0] == 'y' || buf[0] == 'Y';
}

/* ------------------------- Storage and lookup helpers ---------------------- */

static int append(Table *t, const Student *s) {
    Student *p = realloc(t->rec, (t->n + 1) * sizeof *p);
    if (!p) { puts("  Error: out of memory."); return 0; }
    t->rec = p;
    t->rec[t->n++] = *s;
    return 1;
}

static void load(Table *t) {
    FILE *fp = fopen(DB_FILE, "rb");
    Student s;
    if (!fp) return;                                    /* first run: no file yet */
    while (fread(&s, sizeof s, 1, fp) == 1 && append(t, &s)) { }
    fclose(fp);
}

static void save(const Table *t) {
    FILE *fp = fopen(DB_FILE, "wb");
    if (!fp) { perror("  Could not save changes"); return; }
    if (t->n && fwrite(t->rec, sizeof(Student), t->n, fp) != t->n)
        perror("  Could not save changes");
    fclose(fp);
}

/* Returns the record with the given roll number, or NULL. */
static Student *find(const Table *t, int roll) {
    for (size_t i = 0; i < t->n; i++)
        if (t->rec[i].roll == roll) return &t->rec[i];
    return NULL;
}

/* --------------------------------- Display --------------------------------- */

static void printHeader(void) {
    printf("\n%-6s %-30s %-15s %6s\n", "Roll", "Name", "Course", "Marks");
    puts("------ ------------------------------ --------------- ------");
}

static void printRow(const Student *s) {
    printf("%-6d %-30s %-15s %6.2f\n", s->roll, s->name, s->course, s->marks);
}

static void show(const Student *s) {
    printHeader();
    printRow(s);
}

/* -------------------------------- Operations ------------------------------- */

/* Shared by Add and Update. */
static void readDetails(Student *s) {
    readText("Name          : ", s->name, sizeof s->name);
    readText("Course        : ", s->course, sizeof s->course);
    s->marks = readMarks("Marks (0-100) : ");
}

static void addStudent(Table *t) {
    Student s = {0};
    s.roll = readInt("Roll number   : ", 1, MAX_ROLL);
    if (find(t, s.roll)) { puts("  A student with this roll number already exists."); return; }
    readDetails(&s);
    if (append(t, &s)) { save(t); puts("  Student added successfully."); }
}

static void displayAll(const Table *t) {
    if (!t->n) { puts("  No records found."); return; }
    printHeader();
    for (size_t i = 0; i < t->n; i++) printRow(&t->rec[i]);
    printf("\nTotal students: %lu\n", (unsigned long)t->n);
}

static void searchStudent(const Table *t) {
    const Student *s = find(t, readInt("Roll number to search: ", 1, MAX_ROLL));
    if (s) show(s); else puts("  Student not found.");
}

static void updateStudent(Table *t) {
    Student *s = find(t, readInt("Roll number to update: ", 1, MAX_ROLL));
    if (!s) { puts("  Student not found."); return; }
    show(s);
    puts("Enter new details (roll number cannot be changed):");
    readDetails(s);
    save(t);
    puts("  Record updated successfully.");
}

static void deleteStudent(Table *t) {
    Student *s = find(t, readInt("Roll number to delete: ", 1, MAX_ROLL));
    if (!s) { puts("  Student not found."); return; }
    show(s);
    if (!confirm("Delete this record? (y/n): ")) { puts("  Cancelled."); return; }

    memmove(s, s + 1, (size_t)(t->rec + t->n - s - 1) * sizeof *s);   /* close the gap */
    t->n--;
    save(t);
    puts("  Record deleted successfully.");
}

/* ---------------------------------- Main ----------------------------------- */

int main(void) {
    Table t = {0};
    load(&t);

    for (;;) {
        puts("\n========== STUDENT MANAGEMENT SYSTEM ==========");
        puts(" 1. Add Student\n 2. Display All\n 3. Search\n 4. Update\n 5. Delete\n 6. Exit");

        switch (readInt("Enter your choice: ", 1, 6)) {
            case 1: addStudent(&t);    break;
            case 2: displayAll(&t);    break;
            case 3: searchStudent(&t); break;
            case 4: updateStudent(&t); break;
            case 5: deleteStudent(&t); break;
            case 6: free(t.rec);
                    puts("Goodbye!");
                    return EXIT_SUCCESS;
        }
    }
}
