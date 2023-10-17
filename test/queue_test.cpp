#define CATCH_CONFIG_MAIN
#include<catch2/catch_test_macros.hpp>

#include"Item.cpp"
#include"jellystl/debug.hpp"
#include"jellystl/queue.hpp"

TEST_CASE("test") {
    using jelly::queue;
    using jelly::Item;
    using jelly_test::random_string;

    static int section_number = 0;
    ++section_number;
    cout << "--------------------Section " << section_number << ": start--------------------" << endl;
    SECTION("queue()") { // 1
        queue<Item> q;
        REQUIRE(q.size() == 0);
        REQUIRE(q.empty());
    }
    SECTION("push(U&& value)") { // 2
        queue<Item> q;
        q.push(Item(random_string()));
        q.push(Item(random_string()));
        q.push(Item(random_string()));
        REQUIRE(q.size() == 3);
        REQUIRE(!q.empty());
    }
    SECTION("emplace(Args&&... args)") { // 3
        queue<Item> q;
        q.emplace(random_string());
        q.emplace(random_string());
        q.emplace(random_string());
        REQUIRE(q.size() == 3);
        REQUIRE(!q.empty());
    }
    SECTION("pop()") { // 4
        queue<Item> q;
        q.emplace(random_string());
        q.emplace(random_string());
        q.emplace(random_string());
        while(!q.empty()) {
            q.pop();
        }
        REQUIRE(q.size() == 0);
        q.push(Item(random_string()));
        q.push(Item(random_string()));
        q.push(Item(random_string()));
        REQUIRE(q.size() == 3);
    }
    SECTION("front()") { // 5
        queue<Item> s;
        s.emplace(random_string());
        s.emplace(random_string());
        s.emplace(random_string());
        while(!s.empty()) {
            cout << s.front() << endl;
            s.pop();
        }
    }
    SECTION("back()") { // 6
        queue<Item> q;
        for(int i = 0; i < 3; ++i) {
            q.emplace(random_string());
            cout << q.back() << endl;
        }
        REQUIRE(q.size() == 3);
    }
    SECTION("queue(const self& other)") { // 7
        queue<Item> _q;
        _q.emplace(random_string());
        _q.emplace(random_string());
        _q.emplace(random_string());
        queue<Item> q = _q;
        while(!q.empty()) {
            cout << q.front() << endl;
            q.pop();
        }
    }
    SECTION("queue(self&& other)") { // 8
        queue<Item> _q;
        _q.emplace(random_string());
        _q.emplace(random_string());
        _q.emplace(random_string());
        queue<Item> q = std::move(_q);
        while(!q.empty()) {
            cout << q.front() << endl;
            q.pop();
        }
    }
    SECTION("operator=(const self& other)") { // 9
        queue<Item> _q;
        _q.emplace(random_string());
        _q.emplace(random_string());
        _q.emplace(random_string());
        queue<Item> q;
        q.emplace(random_string());
        q.emplace(random_string());
        q = _q;
        while(!q.empty()) {
            cout << q.front() << endl;
            q.pop();
        }
    }
    SECTION("operator=(self&& other)") { // 10
        queue<Item> _q;
        _q.emplace(random_string());
        _q.emplace(random_string());
        _q.emplace(random_string());
        queue<Item> q;
        q.emplace(random_string());
        q.emplace(random_string());
        q = std::move(_q);
        while(!q.empty()) {
            cout << q.front() << endl;
            q.pop();
        }
    }
    SECTION("swap()") { // 11
        queue<Item> _q;
        _q.emplace(random_string());
        _q.emplace(random_string());
        _q.emplace(random_string());
        queue<Item> q;
        q.emplace(random_string());
        q.emplace(random_string());
        swap(_q, q);
        while(!_q.empty()) {
            cout << _q.front() << endl;
            _q.pop();
        }
        while(!q.empty()) {
            cout << q.front() << endl;
            q.pop();
        }
    }
    cout << "--------------------Section " << section_number << ": finish--------------------" << endl;

}