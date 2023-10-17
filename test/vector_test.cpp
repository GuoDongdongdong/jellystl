#define CATCH_CONFIG_MAIN
#include<catch2/catch_test_macros.hpp>
#include<iostream>
#include"jellystl/vector.hpp"
#include"Item.cpp"

TEST_CASE("test") {
    using jelly::vector;
    using jelly::Item;
    using std::cout;
    using std::endl;
    using jelly_test::random_string;
    static int section_number = 0;
    ++section_number;
    cout << "--------------------Section " << section_number << ": start--------------------" << endl;
    SECTION("empty vector"){ // 1
        vector<Item> v;
        REQUIRE(v.begin() == v.end());
        REQUIRE(v.begin() == nullptr);
        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() == 0);
        REQUIRE(v.empty());
    }
    SECTION("vector(size_type n)"){ // 2
        vector<Item> v(3);
        REQUIRE(v.begin() != v.end());
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 3);
        REQUIRE(!v.empty());
        for(const Item& i : v){
            cout << i << endl;
        }
    }
    SECTION("vector(size_type n, const T& value)"){ // 3
        Item item(random_string());
        vector<Item> v(3, item);
        REQUIRE(v.begin() != v.end());
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 3);
        REQUIRE(!v.empty());
        for(const Item& i : v){
            cout << i << endl;
        }
    }
    SECTION("vector(initializer_list)"){ // 4
        vector<Item> v{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        REQUIRE(v.begin() != v.end());
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 3);
        REQUIRE(!v.empty());
        for(const Item& i : v){
            cout << i << endl;
        }
    }
    SECTION("vector(iterator first, iterator last)"){ // 5
        vector<Item> v{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        vector<Item> _v(v.begin(), v.end());
        REQUIRE(_v.begin() != _v.end());
        REQUIRE(_v.size() == 3);
        REQUIRE(_v.capacity() == 3);
        REQUIRE(!_v.empty());
        for(const Item& i : _v){
            cout << i << endl;
        }
    }
    SECTION("vector(const vector&)"){ // 6
        vector<Item> v{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        vector<Item> _v(v);
        REQUIRE(_v.begin() != _v.end());
        REQUIRE(_v.size() == 3);
        REQUIRE(_v.capacity() == 3);
        REQUIRE(!_v.empty());
        for(const Item& i : _v){
            cout << i << endl;
        }
    }
    SECTION("vector(vecotr&&)"){ // 7
        vector<Item> v{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        vector<Item> _v(std::move(v));
        REQUIRE(_v.begin() != _v.end());
        REQUIRE(_v.size() == 3);
        REQUIRE(_v.capacity() == 3);
        REQUIRE(!_v.empty());
        for(const Item& i : _v){
            cout << i << endl;
        }
        REQUIRE(v.begin() == v.end());
        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() == 0);
        REQUIRE(v.empty());
        for(const Item& i : v){
            cout << i << endl;
        }
    }
    SECTION("opeartor=(const vector &)"){ // 8
        vector<Item> v{};
        v.reserve(3);
        for(int i = 0; i < 3; ++i)
            v.push_back(Item(random_string()));
        vector<Item> _v;
        _v = v;
        REQUIRE(_v.begin() != _v.end());
        REQUIRE(_v.size() == 3);
        REQUIRE(_v.capacity() == 3);
        REQUIRE(!_v.empty());
        for(int i = 0; i < 3; ++i){
            REQUIRE(v[i] == _v[i]);
        }
    }
    SECTION("opeartor=(vector &&)"){ // 9
        vector<Item> v{};
        v.reserve(3);
        for(int i = 0; i < 3; ++i)
            v.push_back(Item(random_string()));
        vector<Item> _v{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        _v = std::move(v);
        REQUIRE(_v.begin() != _v.end());
        REQUIRE(_v.size() == 3);
        REQUIRE(_v.capacity() == 3);
        REQUIRE(!_v.empty());

        REQUIRE(v.begin() == v.end());
        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() == 0);
        REQUIRE(v.empty());
    }
    SECTION("push_back"){ // 10
        vector<Item> v;
        v.push_back(Item(random_string()));
        v.push_back(Item(random_string()));
        v.push_back(Item(random_string()));
        REQUIRE(v.begin() != v.end());
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() >= 3);
        REQUIRE(!v.empty());
        for(const Item& i : v){
            cout << i << endl;
        }
    }
    SECTION("emplace_back"){ // 11
        vector<Item> v{};
        v.emplace_back(random_string());
        v.emplace_back(random_string());
        v.emplace_back(random_string());
        v.emplace_back(random_string());
        REQUIRE(v.begin() != v.end());
        REQUIRE(v.size() == 4);
        REQUIRE(v.capacity() >= 4);
        REQUIRE(!v.empty());
        for(const Item& i : v){
            cout << i << endl;
        }
    }
    SECTION("pop_back"){ // 12
        vector<Item> v{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        v.pop_back();
        REQUIRE(v.begin() != v.end());
        REQUIRE(v.size() == 2);
        REQUIRE(v.capacity() == 3);
        REQUIRE(!v.empty());
        for(const Item& i : v){
            cout << i << endl;
        }
    }
    SECTION("earse(iterator position)"){ // 13
        vector<Item> v{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        v.erase(v.begin());
        v.push_back(Item(random_string()));
        v.erase(v.begin() + 1);
        REQUIRE(v.begin() != v.end());
        REQUIRE(v.size() == 2);
        REQUIRE(v.capacity() == 3);
        REQUIRE(!v.empty());
        for(const Item& i : v){
            cout << i << endl;
        }
    }
    SECTION("erase(iterator first, iterator last)"){ // 14
        vector<Item> v{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        v.erase(v.begin(), v.begin() + 2);
        REQUIRE(v.begin() != v.end());
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() == 5);
        REQUIRE(!v.empty());
        for(const Item& i : v){
            cout << i << endl;
        }
    }
    SECTION("insert(iterator position, const T& value)"){ // 15
        vector<Item> v{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        v.insert(v.begin() + 1, Item(random_string()));
        REQUIRE(v.begin() != v.end());
        REQUIRE(v.size() == 6);
        REQUIRE(v.capacity() >= 6);
        REQUIRE(!v.empty());
        for(const Item& i : v){
            cout << i << endl;
        }
    }
    SECTION("insert(iterator position const T& value, size_type n)"){ // 16
        vector<Item> v{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        v.insert(v.begin(), Item(random_string()), 3);
        REQUIRE(v.begin() != v.end());
        REQUIRE(v.size() == 8);
        REQUIRE(v.capacity() >= 8);
        REQUIRE(!v.empty());
        for(const Item& i : v){
            cout << i << endl;
        }
    }
    SECTION("insert(iterator first, iterator last, iterator position)"){ // 17
        vector<Item> v{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        vector<Item> _v{};
        _v.insert(_v.begin(), v.begin(), v.end());
        REQUIRE(_v.begin() != _v.end());
        REQUIRE(_v.size() == 5);
        REQUIRE(_v.capacity() == 5);
        REQUIRE(!_v.empty());
        for(const Item& i : _v){
            cout << i << endl;
        }
    }
    SECTION("insert(initializer_list list, iterator position)"){ // 18
        vector<Item> v{};
        v.insert(v.begin(), {
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        });
        REQUIRE(v.begin() != v.end());
        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() == 5);
        REQUIRE(!v.empty());
        for(const Item& i : v){
            cout << i << endl;
        }
    }
    SECTION("emplace(iterator position, const T& value)"){ // 19
        vector<Item> v{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        v.emplace(v.begin() + 1, random_string());
        v.emplace(v.end() - 1, random_string());
        REQUIRE(v.begin() != v.end());
        REQUIRE(v.size() == 7);
        REQUIRE(v.capacity() >= 6);
        REQUIRE(!v.empty());
        for(const Item& i : v){
            cout << i << endl;
        }
    }
    SECTION("operator=(initializer_list)"){ // 20
        vector<Item> v{};
        v = {
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        REQUIRE(v.begin() != v.end());
        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() == 5);
        REQUIRE(!v.empty());
        for(const Item& i : v){
            cout << i << endl;
        }
    }
    SECTION("swap") { // 21
        vector<Item> v{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        vector<Item> _v{
            Item(random_string()),
            Item(random_string()),
            Item(random_string()),
        };
        swap(v, _v);
        for(const Item& i : v){
            cout << i << endl;
        }
        for(const Item& i : _v){
            cout << i << endl;
        }
    }
    cout << "--------------------Section " << section_number << ": finish--------------------" << endl;
}
