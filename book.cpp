#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <utility>
#include "book.h"


Book::Book() : title_(nullptr), year_(0) {}

Book::Book(const char* title, std::size_t year) : year_(year) {
    if (title) {
        title_ = new char[std::strlen(title) + 1];
        std::strcpy(title_, title);
    } else {
        title_ = nullptr;
    }
}

Book::~Book() {
    delete[] title_;
}

Book::Book(const Book& other) : year_(other.year_) {
    if (other.title_) {
        title_ = new char[std::strlen(other.title_) + 1];
        std::strcpy(title_, other.title_);
    } else {
        title_ = nullptr;
    }
}

Book::Book(Book&& other) noexcept : title_(other.title_), year_(other.year_) {
    other.title_ = nullptr;
    other.year_ = 0;
}

Book& Book::operator=(Book other) {
    swap(*this, other);
    return *this;
}

bool Book::operator==(const Book& other) const {
    if (title_ == nullptr && other.title_ == nullptr) return year_ == other.year_;
    if (title_ == nullptr || other.title_ == nullptr) return false;
    return (std::strcmp(title_, other.title_) == 0) && (year_ == other.year_);
}

bool Book::operator<(const Book& other) const {
    if (title_ == nullptr) return other.title_ != nullptr;
    if (other.title_ == nullptr) return false;
    int cmp = std::strcmp(title_, other.title_);
    if (cmp != 0) {
        return cmp < 0;
    }
    return year_ < other.year_;
}

std::ostream& operator<<(std::ostream& os, const Book& b) {
    os << "(" << (b.title_ ? b.title_ : "NoTitle") << ", " << b.year_ << ")";
    return os;
}

BookSet::BookSet() : capacity_(1), size_(0), book_arr_(new Book[capacity_]{}) {}

BookSet::BookSet(int capacity)
    : size_(0),
    capacity_(capacity),
    book_arr_((capacity > 0) ? new Book[capacity]{} : nullptr) {}

BookSet::BookSet(int capacity, Book* array)
    : BookSet(capacity) {
    for(int i = 0; i < capacity; ++i) {
        book_arr_[i] = array[i];
    }
}

BookSet::BookSet(const BookSet& other)
    : size_(other.size_),
    capacity_(other.capacity_),
    book_arr_(other.capacity_ > 0 ? new Book[other.capacity_]{} : nullptr) {
    std::copy(other.book_arr_, other.book_arr_ + size_, book_arr_);
}

BookSet::BookSet(BookSet&& other) noexcept : size_(other.size_), capacity_(other.capacity_), book_arr_(other.book_arr_) {
        other.book_arr_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
}

void BookSet::resize(std::size_t capacity) {
    Book* new_arr = new Book[capacity]{};
    if(book_arr_ != nullptr) {
        for(int i = 0; i < size_; ++i) {
            new_arr[i] = book_arr_[i];
        }
    }
    delete[] book_arr_;
    book_arr_ = new_arr;
    capacity_ = capacity;   
}

BookSet::~BookSet() {
    delete[] book_arr_;
}

size_t BookSet::size() const {
    return size_;
}

bool BookSet::empty() const {
    return size_ == 0;
}

int BookSet::binary_search(const Book& b) const {
    if(size_ == 0 || book_arr_ == nullptr) return -1;
    int l = 0;
    int r = size_ - 1;
    while(l <= r) {
        int mid = (l + r) / 2;
        if(book_arr_[mid] < b) {
            l = mid + 1;
        } else if(b < book_arr_[mid]) {
            r = mid - 1;
        } else {
            return mid;
        }
    }
    return -1;
}

int BookSet::binary_search_pos(const Book& b) const {
    if(size_ == 0 || book_arr_ == nullptr) return 0;
    int l = 0;
    int r = size_ - 1;
    while(l <= r) {
        int mid = (l + r) / 2;
        if(book_arr_[mid] < b) {
            l = mid + 1;
        } else if(b < book_arr_[mid]) {
            r = mid - 1;
        } else {
            return mid;
        }
    }
    return l;
}

bool BookSet::contains(const Book& b) const {
    int pos = binary_search(b);
    if(pos == -1) return false;
    return true;
}

bool BookSet::add(const Book& b) {
    if(capacity_ == 0) {
        capacity_ = 2;
        size_ = 0;
        book_arr_ = new Book[capacity_]{};
    } else {
        if(!contains(b)) {
            if(size_ == capacity_) {
                resize(capacity_ * 2);
            }
        } else return false;
    }
    int pos = binary_search_pos(b);
    Book* data = new Book[capacity_]{};
    for (int i = 0; i < pos; ++i) {
        data[i] = book_arr_[i];
    }
    data[pos] = b;
    for (int i = pos; i < size_; ++i) {
        data[i + 1] = book_arr_[i];
    }
    delete[] book_arr_;
    book_arr_ = data;
    ++size_;
    return true;
}
    
bool BookSet::remove(const Book& b) {
    if(size_ == 0) return false;
    int pos = binary_search(b);
    if(pos == -1) return false;
    for(int i = pos; i < size_ - 1; ++i) {
        book_arr_[i] = book_arr_[i + 1];
    }
    --size_;
    return true;
}

bool BookSet::operator()(const Book& b) const {
    return contains(b);
}

BookSet& BookSet::operator+=(const Book& b) {
    add(b);
    return *this;
}

BookSet& BookSet::operator-=(const Book& b) {
    remove(b);
    return *this;
}

BookSet BookSet::operator|(const BookSet& other) const {
    BookSet res(size_ + other.size_);
    std::size_t i = 0;
    std::size_t j = 0;
    while (i < size_ && j < other.size_) {
        if (book_arr_[i] < other.book_arr_[j]) {
            res.book_arr_[res.size_++] = book_arr_[i++];
        } else if (other.book_arr_[j] < book_arr_[i]) {
            res.book_arr_[res.size_++] = other.book_arr_[j++];
        } else {
            res.book_arr_[res.size_++] = book_arr_[i++];
            ++j;
        }
    }
    while (i < size_) {
        res.book_arr_[res.size_++] = book_arr_[i++];
    }
    while (j < other.size_) {
        res.book_arr_[res.size_++] = other.book_arr_[j++];
    }
    return res;
}

BookSet BookSet::operator&(const BookSet& other) const {
    BookSet res(std::min(size_, other.size_));
    std::size_t i = 0;
    std::size_t j = 0;
    while (i < size_ && j < other.size_) {
        if (book_arr_[i] < other.book_arr_[j]) {
            ++i;
        } else if (other.book_arr_[j] < book_arr_[i]) {
            ++j;
        } else {
            res.book_arr_[res.size_++] = book_arr_[i++];
            ++j;
        }
    }
    return res;
}

BookSet BookSet::operator-(const BookSet& other) const {
    BookSet res(size_);
    std::size_t i = 0;
    std::size_t j = 0;
    while (i < size_ && j < other.size_) {
        if (book_arr_[i] < other.book_arr_[j]) {
            res.book_arr_[res.size_++] = book_arr_[i++];
        } else if (other.book_arr_[j] < book_arr_[i]) {
            ++j;
        } else {
            ++i;
            ++j;
        }
    }
    while (i < size_) {
        res.book_arr_[res.size_++] = book_arr_[i++];
    }
    return res;
}

bool BookSet::operator==(const BookSet& other) const {
    if (size_ != other.size_) {
        return false;
    }
    for (std::size_t i = 0; i < size_; ++i) {
        if (!(book_arr_[i] == other.book_arr_[i])) {
            return false;
        }
    }
    return true;
}

bool BookSet::operator!=(const BookSet& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const BookSet& set) {
    os << "{";
    for (std::size_t i = 0; i < set.size_; ++i) {
        if (i > 0) {
            os << ", ";
        }
        os << set.book_arr_[i];
    }
    os << "}";
    return os;
}