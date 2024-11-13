#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <stdexcept>

class Author;  // Предварительное объявление класса Author

// Класс Book
class Book {
private:
    std::string title;
    std::string isbn;
    std::vector<Author*> authors;
    int publicationYear;
    bool isAvailable;

public:
    Book(const std::string& title, const std::string& isbn, int publicationYear)
        : title(title), isbn(isbn), publicationYear(publicationYear), isAvailable(true) {}

    void changeAvailabilityStatus(bool status) {
        isAvailable = status;
    }

    std::string getBookInfo() const {
        return "Title: " + title + ", ISBN: " + isbn + ", Year: " + std::to_string(publicationYear);
    }

    bool isBookAvailable() const {
        return isAvailable;
    }
};

// Класс User и его производные Reader и Librarian
class User {
protected:
    int id;
    std::string name;
    std::string email;

public:
    User(int id, const std::string& name, const std::string& email)
        : id(id), name(name), email(email) {}

    virtual void login() = 0;
    std::string getName() const { return name; }  // Добавлен метод для доступа к имени
    virtual ~User() {}
};

class Reader : public User {
public:
    Reader(int id, const std::string& name, const std::string& email)
        : User(id, name, email) {}

    void borrowBook(Book& book) {
        if (book.isBookAvailable()) {
            book.changeAvailabilityStatus(false);
            std::cout << "Книга \"" << book.getBookInfo() << "\" выдана читателю " << name << ".\n";
        } else {
            throw std::runtime_error("Книга недоступна для выдачи");
        }
    }

    void login() override {
        std::cout << "Читатель " << name << " вошел в систему.\n";
    }
};

class Librarian : public User {
public:
    Librarian(int id, const std::string& name, const std::string& email)
        : User(id, name, email) {}

    void addBook(Book& book) {
        std::cout << "Книга \"" << book.getBookInfo() << "\" добавлена библиотекарем " << name << ".\n";
    }

    void login() override {
        std::cout << "Библиотекарь " << name << " вошел в систему.\n";
    }
};

// Класс Loan для операций выдачи и возврата
class Loan {
private:
    Book* book;
    Reader* reader;
    std::time_t loanDate;
    std::time_t returnDate;

public:
    Loan(Book* book, Reader* reader)
        : book(book), reader(reader) {
        loanDate = std::time(nullptr);
        std::cout << "Книга \"" << book->getBookInfo() << "\" выдана " << reader->getName() << ".\n";
    }

    void returnBook() {
        returnDate = std::time(nullptr);
        book->changeAvailabilityStatus(true);
        std::cout << "Книга \"" << book->getBookInfo() << "\" возвращена.\n";
    }
};

// Класс Library для управления системой
class Library {
private:
    std::vector<Book> books;
    std::vector<User*> users;
    std::vector<Loan> loans;

public:
    void addBook(const Book& book) {
        books.push_back(book);
    }

    void addUser(User* user) {
        users.push_back(user);
    }

    void issueLoan(Book& book, Reader& reader) {
        if (book.isBookAvailable()) {
            loans.emplace_back(&book, &reader);
            book.changeAvailabilityStatus(false);
        } else {
            throw std::runtime_error("Книга недоступна для выдачи");
        }
    }
};

// Основная функция
int main() {
    // Создаем экземпляры книг русских авторов
    Book book1("Война и мир", "978-5-17-118366-1", 1869);
    Book book2("Преступление и наказание", "978-5-699-75137-1", 1866);
    Book book3("Мастер и Маргарита", "978-5-17-080407-9", 1966);

    // Создаем пользователей
    Reader reader1(1, "Иван Иванов", "ivan@example.com");
    Librarian librarian1(2, "Анна Петрова", "anna@example.com");

    // Создаем библиотеку и добавляем книги и пользователей
    Library library;
    library.addBook(book1);
    library.addBook(book2);
    library.addBook(book3);
    library.addUser(&reader1);
    library.addUser(&librarian1);

    // Пример авторизации и действий
    librarian1.login();
    librarian1.addBook(book1);

    reader1.login();
    try {
        library.issueLoan(book1, reader1);
    } catch (const std::exception& e) {
        std::cout << e.what() << '\n';
    }

    return 0;
}
