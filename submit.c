#include <stdio.h>
#include <string.h>

#define MAX_BOOKS 100
#define MAX_STUDENTS 100
#define MAX_ISSUES 200

struct Book {
    int id;
    char name[100];
    int totalCopies;
    int availableCopies;
};

struct Student {
    char name[50];
    char rollNo[20];
};

struct issue_record {
    int bookId;
    char rollNo[20];
    int issue_day, issue_month, issue_year;
    int return_day, return_month, return_year;
    int returned;
};

struct Book books[MAX_BOOKS];
struct Student students[MAX_STUDENTS];
struct issue_record issues[MAX_ISSUES];

int b_count = 0, s_count = 0, i_count = 0;

// Utility Functions
int dateToDays(int d, int m, int y) {
    return y * 365 + m * 30 + d;
}

int calculateFine(int issueDate, int returnDate) {
    int delay = returnDate - issueDate;
    int fine;
    if (delay > 15) {
        fine = (delay - 15) * 5;
    } else {
        fine = 0;
    }
    return fine;
}

int findBookId(char bookName[]) {
    for (int i = 0; i < b_count; i++) {
        if (strcmp(books[i].name, bookName) == 0)
            return books[i].id;
    }
    return -1;
}

int isBookAvailable(int id) {
    return books[id].availableCopies > 0;
}

void addBook() {
    printf("Enter Book Name: ");
    scanf(" %[^\n]", books[b_count].name);
    books[b_count].id = b_count;

    printf("Enter total copies: ");
    scanf("%d", &books[b_count].totalCopies);
    books[b_count].availableCopies = books[b_count].totalCopies;

    printf("Book added with ID: %d\n", books[b_count].id);
    b_count++;
}

void addStudent() {
    printf("Enter Student Name: ");
    scanf(" %[^\n]", students[s_count].name);

    printf("Enter Roll No: ");
    scanf("%s", students[s_count].rollNo);

    printf("Student Registered.\n");
    s_count++;
}

void issueBook() {
    char roll[20], bookName[100];
    int bookId;

    printf("Enter Roll No: ");
    scanf("%s", roll);
    printf("Enter Book Name: ");
    scanf(" %[^\n]", bookName);

    bookId = findBookId(bookName);
    if (bookId == -1) {
        printf("Book not found!\n");
        return;
    }

    if (!isBookAvailable(bookId)) {
        printf("Book is out of stock.\n");
        return;
    }

    printf("Enter Issue Date (dd mm yyyy): ");
    scanf("%d %d %d", &issues[i_count].issue_day,
                      &issues[i_count].issue_month,
                      &issues[i_count].issue_year);

    strcpy(issues[i_count].rollNo, roll);
    issues[i_count].bookId = bookId;
    issues[i_count].returned = 0;

    books[bookId].availableCopies--;
    i_count++;

    printf("Book issued successfully!\n");
}

void returnBook() {
    char roll[20];
    printf("Enter Roll No: ");
    scanf("%s", roll);

    int found = 0;
    for (int i = 0; i < i_count; i++) {
        if (strcmp(issues[i].rollNo, roll) == 0 && issues[i].returned == 0) {
            found = 1;
            printf("Returning book: %s\n", books[issues[i].bookId].name);

            printf("Enter Return Date (dd mm yyyy): ");
            scanf("%d %d %d", &issues[i].return_day,
                              &issues[i].return_month,
                              &issues[i].return_year);

            int issueDays = dateToDays(issues[i].issue_day, issues[i].issue_month, issues[i].issue_year);
            int returnDays = dateToDays(issues[i].return_day, issues[i].return_month, issues[i].return_year);
            int fine = calculateFine(issueDays, returnDays);

            issues[i].returned = 1;
            books[issues[i].bookId].availableCopies++;

            printf("Book Returned. Fine: ₹%d\n", fine);
        }
    }

    if (!found)
        printf("No book found issued to this student.\n");
}

void showAllRecords() {
    printf("\n--- Issued Book Records ---\n");
    for (int i = 0; i < i_count; i++) {
        printf("\nRoll No: %s\n", issues[i].rollNo);
        printf("Book: %s\n", books[issues[i].bookId].name);
        printf("Issue Date: %02d/%02d/%04d\n", issues[i].issue_day,
               issues[i].issue_month, issues[i].issue_year);
        if (issues[i].returned) {
            printf("Returned on: %02d/%02d/%04d\n", issues[i].return_day,
                   issues[i].return_month, issues[i].return_year);
        } else {
            printf("Not yet returned.\n");
        }
    }
}

int main() {
    int choice;

    do {
        printf("\n======= LIBRARY MENU =======\n");
        printf("1. Add Book\n");
        printf("2. Add Student\n");
        printf("3. Issue Book\n");
        printf("4. Return Book\n");
        printf("5. Show Records\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(); break;
            case 2: addStudent(); break;
            case 3: issueBook(); break;
            case 4: returnBook(); break;
            case 5: showAllRecords(); break;
            case 6: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }

    } while (choice != 6);

    return 0;
}
