#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <utility>

class Book {
    char* title_;
    std::size_t year_;
public:
    Book();
    
    Book(const char* name, std::size_t age);

    ~Book();

    Book(const Book& other);

    Book(Book&& other) noexcept;

    friend void swap(Book& a, Book& b) noexcept {
        using std::swap;
        swap(a.title_, b.title_);
        swap(a.year_, b.year_);
    }

    Book& operator=(Book other);

    bool operator==(const Book& other) const;
    bool operator<(const Book& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Book& b);

    const char* get_name() const { return title_ ? title_ : ""; }
};

class BookSet {
    std::size_t size_;
    std::size_t capacity_;
    Book* book_arr_;
public:
    BookSet();
    BookSet(int capacity);
    BookSet(int capacity, Book* array);
    BookSet(const BookSet&);
    BookSet(BookSet&&) noexcept;
    friend void swap(BookSet& a, BookSet& b) noexcept {
        using std::swap;
        swap(a.size_, b.size_);
        swap(a.capacity_, b.capacity_);
        swap(a.book_arr_, b.book_arr_);
    };

    BookSet& operator=(const BookSet& other) {
        if(this != &other) {
            BookSet tmp(other); 
            swap(*this, tmp);
        } 
        return *this;
    }
    BookSet& operator=(BookSet&& other) noexcept {
        if (this != &other) {
            delete[] book_arr_;
            book_arr_ = other.book_arr_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.book_arr_ = nullptr; 
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }
    void resize(std::size_t capacity);
    ~BookSet();
    int binary_search(const Book& b) const;
    int binary_search_pos(const Book& b) const;
    bool add(const Book&);              
    bool remove(const Book&);           
    bool contains(const Book&) const; 
    bool operator()(const Book&) const; 
    std::size_t size() const;        
    bool empty() const;              
    BookSet& operator+=(const Book&); 
    BookSet& operator-=(const Book&); 
    BookSet operator|(const BookSet&) const;
    BookSet operator&(const BookSet&) const; 
    BookSet operator-(const BookSet&) const; 
    bool operator==(const BookSet&) const; 
    bool operator!=(const BookSet&) const; 
    friend std::ostream& operator<<(std::ostream&, const BookSet&); 
};
