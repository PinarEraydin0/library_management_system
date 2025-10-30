#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Book {
private:
    int id;
    string title;
    string author;
    bool isBorrowed;

public:
    void setBookDetails(int bookID, string bookTitle, string bookAuthor, bool status) {
        id = bookID;
        title = bookTitle;
        author = bookAuthor;
        isBorrowed = status;
    }

    void display() {
        cout << "ID: " << id << " | Title: " << title << " | Author: " << author
            << " [" << (isBorrowed ? "Borrowed" : "Available") << "]" << endl;
    }

    void borrowBook() {
        isBorrowed = true;
    }

    void returnBook() {
        isBorrowed = false;
    }

    bool isAvailable() {
        return !isBorrowed;
    }

    int getID() {
        return id;
    }

    string getSaveFormat() {
        return to_string(id) + "," + title + "," + author + "," + (isBorrowed ? "1" : "0");
    }
};

class Library {
private:
    Book bookCatalog[50];
    int bookCount;
    string bookFileName;

public:
    Library(string filename) {
        bookFileName = filename;
        bookCount = 0;
        loadBooksFromFile();
    }

    void loadBooksFromFile() {
        ifstream file(bookFileName);
        if (!file.is_open()) {
            cout << "Error: Unable to open file." << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            int id;
            string title, author, statusStr;
            bool status;

            size_t pos1 = line.find(',');
            size_t pos2 = line.find(',', pos1 + 1);
            size_t pos3 = line.find(',', pos2 + 1);

            if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos) continue;

            id = stoi(line.substr(0, pos1));
            title = line.substr(pos1 + 1, pos2 - pos1 - 1);
            author = line.substr(pos2 + 1, pos3 - pos2 - 1);
            statusStr = line.substr(pos3 + 1);
            status = (statusStr == "1");

            bookCatalog[bookCount].setBookDetails(id, title, author, status);
            bookCount++;
        }
        file.close();
    }

    void saveBooksToFile() {
        ofstream file(bookFileName);
        if (!file.is_open()) {
            cout << "Error: Unable to save file." << endl;
            return;
        }
        for (int i = 0; i < bookCount; i++) {
            file << bookCatalog[i].getSaveFormat() << endl;
        }
        file.close();
    }

    void listAllBooks() {
        if (bookCount == 0) {
            cout << "No books available." << endl;
            return;
        }
        for (int i = 0; i < bookCount; i++) {
            bookCatalog[i].display();
        }
    }

    void borrowBook(int bookID) {
        for (int i = 0; i < bookCount; i++) {
            if (bookCatalog[i].getID() == bookID) {
                if (bookCatalog[i].isAvailable()) {
                    bookCatalog[i].borrowBook();
                    cout << "Book borrowed successfully." << endl;
                }
                else {
                    cout << "Error: Book is already borrowed." << endl;
                }
                return;
            }
        }
        cout << "Error: Book not found." << endl;
    }

    void returnBook(int bookID) {
        for (int i = 0; i < bookCount; i++) {
            if (bookCatalog[i].getID() == bookID) {
                if (!bookCatalog[i].isAvailable()) {
                    bookCatalog[i].returnBook();
                    cout << "Book returned successfully." << endl;
                }
                else {
                    cout << "Error: Book was not borrowed." << endl;
                }
                return;
            }
        }
        cout << "Error: Book not found." << endl;
    }
};

int main() {
    Library myLibrary("books.txt");
    int choice;
    int bookID;

    do {
        cout << "\n===== Library Menu =====\n";
        cout << "1. List All Books\n";
        cout << "2. Borrow a Book\n";
        cout << "3. Return a Book\n";
        cout << "4. Save and Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        switch (choice) {
        case 1:
            myLibrary.listAllBooks();
            break;
        case 2:
            cout << "Enter Book ID to borrow: ";
            cin >> bookID;
            myLibrary.borrowBook(bookID);
            break;
        case 3:
            cout << "Enter Book ID to return: ";
            cin >> bookID;
            myLibrary.returnBook(bookID);
            break;
        case 4:
            myLibrary.saveBooksToFile();
            cout << "Saving changes... Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 4);

    return 0;
}


