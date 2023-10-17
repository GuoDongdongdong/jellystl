#define CATCH_CONFIG_MAIN
#include<catch2/catch_test_macros.hpp>

#include"jellystl/list.hpp"
#include"jellystl/vector.hpp"
#include"Item.cpp"
#include"jellystl/iterator.hpp"
TEST_CASE("test") {
    using jelly::list;
    using jelly::Item;
    using std::cout;
    using std::endl;
    using jelly_test::random_string;

    static int section_number = 0;
    ++section_number;
    cout << "--------------------Section " << section_number << ": start--------------------" << endl;

    SECTION("list()"){ //1
        list<Item> l{};
        REQUIRE(l.size() == 0);
        REQUIRE(l.empty());
        REQUIRE(l.begin() == l.end());
        REQUIRE(l.cbegin() == l.cend());
        for (Item &i : l) {
            cout << i << endl;
        }
    }
    SECTION("list(size_type n)"){ // 2
        list<Item> l(3);
        REQUIRE(l.size() == 3);
        REQUIRE(!l.empty());
        REQUIRE(!(l.begin() == l.end()));
        REQUIRE(!(l.cbegin() == l.cend()));
        for (Item &i : l) {
            cout << i << endl;
        }
    }
    SECTION("list(initializer_list list)"){ // 3
        list<Item> l{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        REQUIRE(l.size() == 3);
        REQUIRE(!l.empty());
        REQUIRE(l.begin() != l.end());
        REQUIRE(l.cbegin() != l.cend());
        for (Item &i : l) {
            cout << i << endl;
        }
    }
    SECTION("list(InputIterator first, InputIterator last)"){ // 4
        using jelly::vector;
        vector<Item> v(3);
        list<Item> l(v.begin(), v.end());
        REQUIRE(l.size() == 3);
        REQUIRE(!l.empty());
        REQUIRE(l.begin() != l.end());
        REQUIRE(l.cbegin() != l.cend());
        for (Item &i : l) {
            cout << i << endl;
        }
        int pos = 0;
        for (auto i = l.begin(); i != l.end(); ++i, ++pos) {
            REQUIRE(v[pos] == *i);
        }

        list<Item> _l(l.begin(), l.end());
        REQUIRE(_l.size() == 3);
        REQUIRE(!_l.empty());
        REQUIRE(_l.begin() != _l.end());
        REQUIRE(_l.cbegin() != _l.cend());
        for (Item &i : _l) {
            cout << i << endl;
        }
        pos = 0;
        for (auto i = _l.begin(); i != _l.end(); ++i, ++pos) {
            REQUIRE(v[pos] == *i);
        }
    }
    SECTION("list(const list& other)"){ // 5
        list<Item> l{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        list<Item> _l(l);
        REQUIRE(_l.size() == 3);
        REQUIRE(!_l.empty());
        REQUIRE(_l.begin() != _l.end());
        REQUIRE(_l.cbegin() != _l.cend());
        for (Item &i : _l) {
            cout << i << endl;
        }
    }
    SECTION("list(list&& other)"){ // 6
        list<Item> l{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        list<Item> _l(std::move(l));
        REQUIRE(_l.size() == 3);
        REQUIRE(!_l.empty());
        REQUIRE(_l.begin() != _l.end());
        REQUIRE(_l.cbegin() != _l.cend());
        for (Item &i : _l) {
            cout << i << endl;
        }
    }
    SECTION("operator=(const list& other)"){ // 7
        list<Item> l{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        list<Item> _l{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        _l = l;
        REQUIRE(_l.size() == 3);
        REQUIRE(!_l.empty());
        REQUIRE(_l.begin() != _l.end());
        REQUIRE(_l.cbegin() != _l.cend());
        for (Item &i : _l) {
            cout << i << endl;
        }
    }
    SECTION("operator=(list&& other)"){ // 8
        list<Item> l{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        list<Item> _l;
        _l = std::move(l);

        REQUIRE(l.size() == 0);
        REQUIRE(l.empty());
        REQUIRE(l.begin() == l.end());
        REQUIRE(l.cbegin() == l.cend());

        REQUIRE(_l.size() == 3);
        REQUIRE(!_l.empty());
        REQUIRE(_l.begin() != _l.end());
        REQUIRE(_l.cbegin() != _l.cend());
        for (Item &i : _l) {
            cout << i << endl;
        }
    }
    SECTION("swap"){ // 9
        list<Item> l{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        list<Item> _l{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        l.swap(_l);
        for (Item &i : l) {
            cout << i << endl;
        }
        for (Item &i : _l) {
            cout << i << endl;
        }
    }
    SECTION("push_back, push_front, pop_back, pop_front") { // 10
        list<Item> l;
        l.push_back(Item(random_string()));
        l.push_back(Item(random_string()));
        l.push_front(Item(random_string()));
        l.push_back(Item(random_string()));
        l.pop_back();
        l.pop_front();
        REQUIRE(l.size() == 2);
        for (Item &i : l) {
            cout << i << endl;
        }
    }
    SECTION("emplace insert") { // 11
        list<Item> l;
        l.push_back(Item(random_string()));
        l.emplace(l.begin());
        l.emplace(l.begin(), random_string());
        l.emplace(l.begin(), Item(random_string()));
        l.insert(l.begin(), Item(random_string()));
        REQUIRE(l.size() == 5);
        for (Item &i : l) {
            cout << i << endl;
        }
    }
    SECTION("insert(const_iterator position, const T& value, size_type n)") { // 12
        list<Item> l;
        l.emplace(l.begin());
        l.emplace(l.begin());
        auto p = l.begin();
        ++p;
        l.insert(p, Item(random_string()), 3);
        REQUIRE(l.size() == 5);
        for (Item &i : l) {
            cout << i << endl;
        }
    }
    SECTION("insert(const_iterator position, InputIterator first, InputIterator last)") { // 13
        list<Item> l{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        list<Item> _l;
        _l.insert(_l.begin(), l.begin(), l.end());
        REQUIRE(_l.size() == 3);
        for (Item &i : _l) {
            cout << i << endl;
        }
    }
    SECTION("const_iterator position") { // 14
        list<Item> l{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        auto p = l.begin();
        ++p;
        l.erase(p);
        l.erase(l.begin());

        REQUIRE(l.size() == 1);
        for (Item &i : l) {
            cout << i << endl;
        }
    }
    SECTION("const_iterator first, const_iterator last") { // 15
        list<Item> l{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        auto first = l.begin();
        auto last = l.end();
        ++first; --last;
        l.erase(first, last);

        REQUIRE(l.size() == 2);
        for (Item &i : l) {
            cout << i << endl;
        }
    }
    SECTION("merge(list&& other)") { // 16
        list<int> l{3,5,8,9,21};
        list<int> _l{1,2,3,4,5,6,7,120,200};
        l.merge(_l);
        REQUIRE(l.size() == 14);
        REQUIRE(_l.size() == 0);
        REQUIRE(_l.empty());
        for (auto i : l) {
            cout << i << endl;
        }
    }
    SECTION("swap()") { // 17
        list<int> l;
        list<int> _l{1,2,3,4,5,6,7,120,200};
        l.swap(_l);
        REQUIRE(l.size() == 9);
        REQUIRE(_l.size() == 0);
        for (auto i : l) {
            cout << i << endl;
        }
        for (auto i : _l) {
            cout << i << endl;
        }
    }
    SECTION("swap()") { // 18
        list<int> l{3,5,8,9,21};
        list<int> _l;
        l.swap(_l);
        REQUIRE(l.size() == 0);
        REQUIRE(_l.size() == 5);
        for (auto i : l) {
            cout << i << endl;
        }
        for (auto i : _l) {
            cout << i << endl;
        }
    }
    SECTION("swap()") { // 19
        list<int> l;
        list<int> _l;
        l.swap(_l);
        REQUIRE(l.size() == 0);
        REQUIRE(_l.size() == 0);
        for (auto i : l) {
            cout << i << endl;
        }
        for (auto i : _l) {
            cout << i << endl;
        }
    }
    SECTION("sort()") { // 20
        list<int> l{3,6,1,2,0,-1, 20, 500, -1000};
        l.sort();
        REQUIRE(l.size() == 9);
        REQUIRE(!l.empty());
        for (auto i : l) {
            cout << i << endl;
        }
    }
    SECTION("operator=(std::initializer_list<T> list)") { // 21
        list<Item> l(3, Item(random_string()));
        l = {
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        for (Item &i : l) {
            cout << i << endl;
        }
    }
    cout << "--------------------Section " << section_number << ": finish--------------------" << endl;

}