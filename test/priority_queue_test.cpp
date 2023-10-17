#define CATCH_CONFIG_MAIN
#include<catch2/catch_test_macros.hpp>

#include"Item.cpp"
#include"jellystl/debug.hpp"
#include"jellystl/queue.hpp"

TEST_CASE("test") {
    using jelly::priority_queue;
    using jelly_test::random_int;

    static int section_number = 0;
    ++section_number;
    cout << "--------------------Section " << section_number << ": start--------------------" << endl;
    SECTION("priority_queue()") { // 1
        priority_queue<int> pq;
        REQUIRE(pq.size() == 0);
        REQUIRE(pq.empty());
    }
    SECTION("push(U&& value)") { // 2
        priority_queue<int> pq;
        pq.push(random_int());
        pq.push(random_int());
        pq.push(random_int());
        REQUIRE(pq.size() == 3);
        REQUIRE(!pq.empty());
        while(!pq.empty()) {
            cout << pq.front() << endl;
            pq.pop();
        }
    }
    SECTION("emplace(Args&&... args)") { // 3
        priority_queue<int> pq;
        pq.emplace(random_int());
        pq.emplace(random_int());
        pq.emplace(random_int());
        REQUIRE(pq.size() == 3);
        REQUIRE(!pq.empty());
        while(!pq.empty()) {
            cout << pq.front() << endl;
            pq.pop();
        }
    }
    SECTION("pop()") { // 4
        priority_queue<int> pq;
        pq.emplace(random_int());
        pq.emplace(random_int());
        pq.emplace(random_int());
        while(!pq.empty()) {
            cout << pq.front() << endl;
            pq.pop();
        }
        REQUIRE(pq.size() == 0);
    }
    SECTION("front()") { // 5
        priority_queue<int> s;
        s.emplace(random_int());
        s.emplace(random_int());
        s.emplace(random_int());
        while(!s.empty()) {
            cout << s.front() << endl;
            s.pop();
        }
    }
    SECTION("priority_queue(const self& other)") { // 6
        priority_queue<int> _pq;
        _pq.emplace(random_int());
        _pq.emplace(random_int());
        _pq.emplace(random_int());
        priority_queue<int> pq = _pq;
        while(!pq.empty()) {
            cout << pq.front() << endl;
            pq.pop();
        }
    }
    SECTION("priority_queue(self&& other)") { // 7
        priority_queue<int> _pq;
        _pq.emplace(random_int());
        _pq.emplace(random_int());
        _pq.emplace(random_int());
        priority_queue<int> pq = std::move(_pq);
        while(!pq.empty()) {
            cout << pq.front() << endl;
            pq.pop();
        }
    }
    SECTION("operator=(const self& other)") { // 8
        priority_queue<int> _pq;
        _pq.emplace(random_int());
        _pq.emplace(random_int());
        _pq.emplace(random_int());
        priority_queue<int> pq;
        pq.emplace(random_int());
        pq.emplace(random_int());
        pq = _pq;
        while(!pq.empty()) {
            cout << pq.front() << endl;
            pq.pop();
        }
    }
    SECTION("operator=(self&& other)") { // 9
        priority_queue<int> _pq;
        _pq.emplace(random_int());
        _pq.emplace(random_int());
        _pq.emplace(random_int());
        priority_queue<int> pq;
        pq.emplace(random_int());
        pq.emplace(random_int());
        pq = std::move(_pq);
        while(!pq.empty()) {
            cout << pq.front() << endl;
            pq.pop();
        }
    }
    SECTION("swap()") { // 10
        priority_queue<int> _pq;
        _pq.emplace(random_int());
        _pq.emplace(random_int());
        _pq.emplace(random_int());
        priority_queue<int> pq;
        pq.emplace(random_int());
        pq.emplace(random_int());
        swap(_pq, pq);
        cout << "_pq:";
        while(!_pq.empty()) {
            cout << _pq.front() << endl;
            _pq.pop();
        }
        cout << "pq:";
        while(!pq.empty()) {
            cout << pq.front() << endl;
            pq.pop();
        }
    }
    cout << "--------------------Section " << section_number << ": finish--------------------" << endl;
}