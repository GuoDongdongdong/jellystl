#define CATCH_CONFIG_MAIN
#include<catch2/catch_test_macros.hpp>

#include"Item.cpp"
#include"jellystl/debug.hpp"
#include"jellystl/stack.hpp"

TEST_CASE("test") {
    using jelly::stack;
    using jelly::Item;
    using jelly_test::random_string;

    static int section_number = 0;
    ++section_number;
    cout << "--------------------Section " << section_number << ": start--------------------" << endl;
    SECTION("stack()") { // 1
        stack<Item> s;
        REQUIRE(s.size() == 0);
        REQUIRE(s.empty());
    }
    SECTION("push(U&& value)") { // 2
        stack<Item> s;
        s.push(Item(random_string()));
        s.push(Item(random_string()));
        s.push(Item(random_string()));
        REQUIRE(s.size() == 3);
        REQUIRE(!s.empty());
    }
    SECTION("emplace(Args&&... args)") { // 3
        stack<Item> s;
        s.emplace(random_string());
        s.emplace(random_string());
        s.emplace(random_string());
        REQUIRE(s.size() == 3);
        REQUIRE(!s.empty());
    }
    SECTION("pop()") { // 4
        stack<Item> s;
        s.emplace(random_string());
        s.emplace(random_string());
        s.emplace(random_string());
        while(!s.empty()) {
            s.pop();
        }
        REQUIRE(s.size() == 0);
        s.push(Item(random_string()));
        s.push(Item(random_string()));
        s.push(Item(random_string()));
        REQUIRE(s.size() == 3);
    }
    SECTION("top()") { // 5
        stack<Item> s;
        s.emplace(random_string());
        s.emplace(random_string());
        s.emplace(random_string());
        while(!s.empty()) {
            cout << s.top();
            s.pop();
        }
    }
    SECTION("stack(const self& other)") { // 6
        stack<Item> _s;
        _s.emplace(random_string());
        _s.emplace(random_string());
        _s.emplace(random_string());
        stack<Item> s = _s;
        while(!s.empty()) {
            cout << s.top() << endl;
            s.pop();
        }
    }
    SECTION("stack(self&& other)") { // 7
        stack<Item> _s;
        _s.emplace(random_string());
        _s.emplace(random_string());
        _s.emplace(random_string());
        stack<Item> s = std::move(_s);
        while(!s.empty()) {
            cout << s.top() << endl;
            s.pop();
        }
    }
    SECTION("operator=(const self& other)") { // 8
        stack<Item> _s;
        _s.emplace(random_string());
        _s.emplace(random_string());
        _s.emplace(random_string());
        stack<Item> s;
        s.emplace(random_string());
        s.emplace(random_string());
        s = _s;
        while(!s.empty()) {
            cout << s.top() << endl;
            s.pop();
        }
    }
    SECTION("operator=(self&& other)") { // 9
        stack<Item> _s;
        _s.emplace(random_string());
        _s.emplace(random_string());
        _s.emplace(random_string());
        stack<Item> s;
        s.emplace(random_string());
        s.emplace(random_string());
        s = std::move(_s);
        while(!s.empty()) {
            cout << s.top() << endl;
            s.pop();
        }
    }
    SECTION("swap()") { // 10
        stack<Item> _s;
        _s.emplace(random_string());
        _s.emplace(random_string());
        _s.emplace(random_string());
        stack<Item> s;
        s.emplace(random_string());
        s.emplace(random_string());
        swap(_s, s);
        while(!_s.empty()) {
            cout << _s.top() << endl;
            _s.pop();
        }
        while(!s.empty()) {
            cout << s.top() << endl;
            s.pop();
        }
    }
    cout << "--------------------Section " << section_number << ": finish--------------------" << endl;

}