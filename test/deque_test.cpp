#define CATCH_CONFIG_MAIN
#include<catch2/catch_test_macros.hpp>

#include"Item.cpp"
#include"jellystl/debug.hpp"
#include"jellystl/deque.hpp"
#include"jellystl/vector.hpp"

TEST_CASE("test") {
    using jelly::vector;
    using jelly::deque;
    using jelly::Item;
    using jelly_test::random_string;

    static int section_number = 0;
    ++section_number;
    cout << "--------------------Section " << section_number << ": start--------------------" << endl;
    SECTION("deque()") { // 1
        deque<Item> d;
        REQUIRE(d.size() == 0);
        REQUIRE(d.empty());
        REQUIRE(d.begin() == d.end());
        REQUIRE(d.cbegin() == d.cend());
    }
    SECTION("deque(size_type n)") { // 2
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        REQUIRE(d.size() == 5);
        REQUIRE(!d.empty());
        REQUIRE(d.begin() != d.end());
        REQUIRE(d.cbegin() != d.cend());
        for(auto& i:d) {
            cout << i << endl;
        }
    }
    SECTION("deque(InputIterator first, InputIterator last)") { // 3
        deque<Item> _d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        deque<Item> d(_d.begin(), _d.end());
        REQUIRE(d.size() == 3);
        REQUIRE(!d.empty());
        REQUIRE(d.begin() != d.end());
        REQUIRE(d.cbegin() != d.cend());
        for(auto& i:d) {
            cout << i << endl;
        }
    }
    SECTION("deque(std::initializer_list<T> list)") { // 4
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        REQUIRE(d.size() == 3);
        REQUIRE(!d.empty());
        REQUIRE(d.begin() != d.end());
        REQUIRE(d.cbegin() != d.cend());
        for(auto& i:d) {
            cout << i << endl;
        }
    }
    SECTION("deque(const deque& other)") { // 5
        deque<Item> _d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        deque<Item> d = _d;
        REQUIRE(d.size() == 3);
        REQUIRE(!d.empty());
        REQUIRE(d.begin() != d.end());
        REQUIRE(d.cbegin() != d.cend());
        for(auto& i:d) {
            cout << i << endl;
        }
    }
    SECTION("deque(deque&& other)") { // 6
        deque<Item> _d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        deque<Item> d = std::move(_d);
        REQUIRE(d.size() == 3);
        REQUIRE(!d.empty());
        REQUIRE(d.begin() != d.end());
        REQUIRE(d.cbegin() != d.cend());
        for(auto& i:d) {
            cout << i << endl;
        }
    }
    SECTION("operator=(const deque& other)") { // 7
        deque<Item> _d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        d = _d;
        REQUIRE(d.size() == 5);
        d.push_back(Item(random_string()));
        d.push_back(Item(random_string()));
        d.push_back(Item(random_string()));
        REQUIRE(d.size() == 8);
        
        for(Item& i: d){
            cout << i << endl;
        }
    }
    SECTION("operator=(deque&& other)") { // 8
        deque<Item> _d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        d = std::move(_d);
        REQUIRE(d.size() == 5);
        d.push_back(Item(random_string()));
        d.push_back(Item(random_string()));
        d.push_back(Item(random_string()));
        REQUIRE(d.size() == 8);
        for(Item& i: d){
            cout << i << endl;
        }
    }
    SECTION("operator=(std::initializer_list<T> list)") { // 9
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        d = {
            Item(random_string()),
        };
        REQUIRE(d.size() == 1);
        for(Item& i: d){
            cout << i << endl;
        }
    }
    SECTION("assign(size_type n, const T& value) ") { // 10
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        d.assign(5, Item(random_string()));
        REQUIRE(d.size() == 5);
        for(Item& i: d){
            cout << i << endl;
        }
    }
    SECTION("assign(InputIterator first, InputIterator last) ") { // 11
        deque<Item> _d{
            Item(random_string()),
            Item(random_string()),
        };
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        d.assign(_d.cbegin(), _d.cend());
        REQUIRE(d.size() == 2);
        for(Item& i: d){
            cout << i << endl;
        }
    }
    SECTION("front()") { // 12
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        REQUIRE(*d.begin() == d.front());
        cout << d.front() << endl;
    }
    SECTION("back()") { // 13
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        REQUIRE(d[2] == d.back());
        cout << d.back() << endl;
    }
    SECTION("operator[](distance_type n)") { // 14
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        for(int i = 0; i < d.size(); ++i) {
            cout << d[i] << endl;
        }
    }
    SECTION("emplace_front(Args&&... args)") { // 15
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        d.emplace_front(random_string());
        d.emplace_front(random_string());
        d.emplace_front(random_string());
        REQUIRE(d.size() == 6);
        for(int i = 0; i < d.size(); ++i) {
            cout << d[i] << endl;
        }
    }
    SECTION("push_front(U&& value)") { // 16
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        d.push_front(Item(random_string()));
        d.push_front(Item(random_string()));
        d.push_front(Item(random_string()));
        REQUIRE(d.size() == 6);
        for(int i = 0; i < d.size(); ++i) {
            cout << d[i] << endl;
        }
    }
    SECTION("emplace_back(Args&&... args)") { // 17
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        d.emplace_back(random_string());
        d.emplace_back(random_string());
        d.emplace_back(random_string());
        REQUIRE(d.size() == 6);
        for(int i = 0; i < d.size(); ++i) {
            cout << d[i] << endl;
        }
    }
    SECTION("push_back(U&& value)") { // 18
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        d.push_back(Item(random_string()));
        d.push_back(Item(random_string()));
        d.push_back(Item(random_string()));
        REQUIRE(d.size() == 6);
        for(int i = 0; i < d.size(); ++i) {
            cout << d[i] << endl;
        }
    }
    SECTION("pop_front()") { // 19
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        while(!d.empty()) {
            d.pop_front();
        }
        REQUIRE(d.size() == 0);
    }
    SECTION("pop_back()") { // 20
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        while(!d.empty()) {
            d.pop_back();
        }
        REQUIRE(d.size() == 0);
    }
    SECTION("emplace(const_iterator position, Args&&... args)") { // 21
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        d.emplace(d.begin(), random_string());
        d.emplace(d.cbegin(), random_string());
        d.emplace(d.begin() + 1, random_string());
        d.emplace(d.begin() + 2, random_string());
        d.emplace(d.begin() + 3, random_string());

        REQUIRE(d.size() == 8);
        for(auto& i: d) {
            cout << i << endl;
        }
    }
    SECTION("insert(const_iterator position, U&& value)") { // 22
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        d.insert(d.begin(), Item(random_string()));
        d.insert(d.cbegin(), Item(random_string()));
        d.insert(d.begin() + 1, Item(random_string()));
        d.insert(d.begin() + 2, Item(random_string()));
        d.insert(d.begin() + 3, Item(random_string()));
        d.insert(d.end(), Item(random_string()));
        d.insert(d.cend(), Item(random_string()));

        REQUIRE(d.size() == 10);
        for(auto& i: d) {
            cout << i << endl;
        }
    }
    SECTION("insert(const_iterator position, size_type n, const T& value)") { // 23
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        d.insert(d.begin() + 1, 3, Item(random_string()));

        REQUIRE(d.size() == 6);
        for(auto& i: d) {
            cout << i << endl;
        }
    }
    SECTION("insert(const_iterator position, InputIterator first, InputIterator last)") { // 24
        deque<Item> _d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        d.insert(d.begin() + 1, _d.begin(), _d.end());

        REQUIRE(d.size() == 6);
        for(auto& i: d) {
            cout << i << endl;
        }
    }
    SECTION("insert(const_iterator position, std::initializer_list<T> list)") { // 25
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        d.insert(d.begin() + 1, {
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        });
        REQUIRE(d.size() == 6);
        for(auto& i: d) {
            cout << i << endl;
        }
    }
    SECTION("erase(const_iterator position)") { // 26
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        d.insert(d.begin() + 1, {
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        });
        d.erase(d.begin() + 3);
        d.erase(d.begin() + 1);
        REQUIRE(d.size() == 4);
        for(auto& i: d) {
            cout << i << endl;
        }
    }
    SECTION("erase(const_iterator first, const_iterator last)") { // 27
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        d.insert(d.begin() + 1, {
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        });
        d.erase(d.begin(), d.end() - 2);
        REQUIRE(d.size() == 2);
        for(auto& i: d) {
            cout << i << endl;
        }
    }
     SECTION("swap") { // 28
        deque<Item> _d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        deque<Item> d{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        swap(d, _d);
        REQUIRE(_d.size() == 4);
        REQUIRE(d.size() == 3);
        for(auto& i: _d) {
            cout << i << endl;
        }
        for(auto& i: d) {
            cout << i << endl;
        }
    }
    
    cout << "--------------------Section " << section_number << ": finish--------------------" << endl;

}