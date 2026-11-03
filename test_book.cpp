#include <gtest/gtest.h>
#include <sstream>
#include <utility>
#include <string>
#include "book.h"

TEST(BookSetBasic, EmptySet) {
    BookSet s;
    EXPECT_EQ(s.size(), 0u);
    EXPECT_TRUE(s.empty());
    EXPECT_FALSE(s.contains(Book("Dune", 1965)));
    EXPECT_FALSE(s(Book("Dune", 1965)));
}

TEST(BookSetBasic, AddElements) {
    BookSet s;
    EXPECT_TRUE(s.add(Book("War and Peace", 1869)));
    EXPECT_EQ(s.size(), 1u);
    EXPECT_FALSE(s.empty());
    EXPECT_FALSE(s.add(Book("War and Peace", 1869)));
    EXPECT_EQ(s.size(), 1u);
    EXPECT_TRUE(s.add(Book("1984", 1949)));
    EXPECT_EQ(s.size(), 2u);
}

TEST(BookSetBasic, RemoveElements) {
    BookSet s;
    s.add(Book("Solaris", 1961));
    s.add(Book("Fahrenheit 451", 1953));
    EXPECT_FALSE(s.remove(Book("The Hobbit", 1937)));
    EXPECT_EQ(s.size(), 2u);
    EXPECT_TRUE(s.remove(Book("Solaris", 1961)));
    EXPECT_EQ(s.size(), 1u);
    EXPECT_FALSE(s.contains(Book("Solaris", 1961)));
    EXPECT_TRUE(s.remove(Book("Fahrenheit 451", 1953)));
    EXPECT_EQ(s.size(), 0u);
    EXPECT_TRUE(s.empty());
}

TEST(BookSetBasic, ContainsAndCallOperator) {
    BookSet s;
    Book b1("Hamlet", 1603);
    Book b2("Macbeth", 1606);
    s.add(b1);
    EXPECT_TRUE(s.contains(b1));
    EXPECT_TRUE(s(b1));
    EXPECT_FALSE(s.contains(b2));
    EXPECT_FALSE(s(b2));
}

TEST(BookSetRuleOfFive, CopyIndependence) {
    BookSet s1;
    s1.add(Book("Dune", 1965));
    s1.add(Book("Hyperion", 1989));

    BookSet s2 = s1;
    EXPECT_EQ(s2.size(), 2u);
    EXPECT_TRUE(s2.contains(Book("Dune", 1965)));
    EXPECT_TRUE(s2.contains(Book("Hyperion", 1989)));

    s2.add(Book("Foundation", 1951));
    EXPECT_EQ(s2.size(), 3u);
    EXPECT_EQ(s1.size(), 2u);
    EXPECT_FALSE(s1.contains(Book("Foundation", 1951)));

    s1.remove(Book("Dune", 1965));
    EXPECT_FALSE(s1.contains(Book("Dune", 1965)));
    EXPECT_TRUE(s2.contains(Book("Dune", 1965)));
}

TEST(BookSetRuleOfFive, MoveConstructorAndAssignment) {
    BookSet s1;
    s1.add(Book("The Catcher in the Rye", 1951));
    s1.add(Book("The Great Gatsby", 1925));

    BookSet s2 = std::move(s1);
    EXPECT_EQ(s2.size(), 2u);
    EXPECT_TRUE(s2.contains(Book("The Catcher in the Rye", 1951)));
    EXPECT_TRUE(s2.contains(Book("The Great Gatsby", 1925)));
    EXPECT_EQ(s1.size(), 0u);
    EXPECT_TRUE(s1.empty());

    BookSet s3;
    s3 = std::move(s2);
    EXPECT_EQ(s3.size(), 2u);
    EXPECT_TRUE(s3.contains(Book("The Catcher in the Rye", 1951)));
    EXPECT_EQ(s2.size(), 0u);
    EXPECT_TRUE(s2.empty());
}

TEST(BookSetRuleOfFive, SelfAssignment) {
    BookSet s;
    s.add(Book("Neuromancer", 1984));
    s = s;
    EXPECT_EQ(s.size(), 1u);
    EXPECT_TRUE(s.contains(Book("Neuromancer", 1984)));
}

TEST(BookSetOperators, CompoundAssignmentOperators) {
    BookSet s;
    Book b1("Dracula", 1897);
    Book b2("Frankenstein", 1818);

    s += b1;
    EXPECT_EQ(s.size(), 1u);
    EXPECT_TRUE(s.contains(b1));

    s += b2;
    EXPECT_EQ(s.size(), 2u);
    EXPECT_TRUE(s.contains(b2));

    s -= b1;
    EXPECT_EQ(s.size(), 1u);
    EXPECT_FALSE(s.contains(b1));
    EXPECT_TRUE(s.contains(b2));
}

TEST(BookSetOperations, UnionIntersectionDifference) {
    BookSet a;
    BookSet b;

    Book b1("Book A", 2000);
    Book b2("Book B", 2001);
    Book b3("Book C", 2002);

    a.add(b1);
    a.add(b2);

    b.add(b2);
    b.add(b3);

    BookSet u = a | b;
    EXPECT_EQ(u.size(), 3u);
    EXPECT_TRUE(u.contains(b1));
    EXPECT_TRUE(u.contains(b2));
    EXPECT_TRUE(u.contains(b3));

    BookSet i = a & b;
    EXPECT_EQ(i.size(), 1u);
    EXPECT_FALSE(i.contains(b1));
    EXPECT_TRUE(i.contains(b2));
    EXPECT_FALSE(i.contains(b3));

    BookSet d = a - b;
    EXPECT_EQ(d.size(), 1u);
    EXPECT_TRUE(d.contains(b1));
    EXPECT_FALSE(d.contains(b2));
}

TEST(BookSetOperators, EqualityComparison) {
    BookSet s1;
    BookSet s2;
    Book b1("Metro 2033", 2005);
    Book b2("Metro 2034", 2009);

    EXPECT_TRUE(s1 == s2);
    EXPECT_FALSE(s1 != s2);

    s1.add(b1);
    s1.add(b2);

    s2.add(b2);
    s2.add(b1);

    EXPECT_TRUE(s1 == s2);
    EXPECT_FALSE(s1 != s2);

    s2.add(Book("Metro 2035", 2015));
    EXPECT_FALSE(s1 == s2);
    EXPECT_TRUE(s1 != s2);
}

TEST(BookSetOutput, StreamInsertionFormat) {
    BookSet s;
    std::stringstream ss_empty;
    ss_empty << s;
    EXPECT_EQ(ss_empty.str(), "{}");

    s.add(Book("Brave New World", 1932));
    s.add(Book("Animal Farm", 1945));

    std::stringstream ss;
    ss << s;
    EXPECT_EQ(ss.str(), "{(Animal Farm, 1945), (Brave New World, 1932)}");
}

TEST(BookSetStress, InsertOneThousandSortedUnique) {
    BookSet s;
    for (int i = 999; i >= 0; --i) {
        std::string title = "Book_" + std::string(3 - std::to_string(i).length(), '0') + std::to_string(i);
        EXPECT_TRUE(s.add(Book(title.c_str(), static_cast<std::size_t>(2000 + i))));
    }
    EXPECT_EQ(s.size(), 1000u);

    for (int i = 0; i < 1000; ++i) {
        std::string title = "Book_" + std::string(3 - std::to_string(i).length(), '0') + std::to_string(i);
        EXPECT_FALSE(s.add(Book(title.c_str(), static_cast<std::size_t>(2000 + i))));
    }
    EXPECT_EQ(s.size(), 1000u);

    for (int i = 0; i < 500; ++i) {
        std::string title = "Book_" + std::string(3 - std::to_string(i).length(), '0') + std::to_string(i);
        EXPECT_TRUE(s.remove(Book(title.c_str(), static_cast<std::size_t>(2000 + i))));
    }
    EXPECT_EQ(s.size(), 500u);
}