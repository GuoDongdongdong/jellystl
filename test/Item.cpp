#include<iostream>
#include<string>
#include<random>

namespace jelly {

struct Item {
    std::string name;
    
    Item(): name("default") {
        std::cout << "Item() " << this << " default" << std::endl;
    }

    Item(std::string name): name(name) {
        std::cout << "Item(string) " << this << " " << name << std::endl;
    }

    Item(const Item& other): name(other.name) {
        std::cout << "Item(const Item&) from " << other.name << " " << &other << " to " << this << std::endl;
    }

    Item(Item&& other) noexcept {
        std::cout << "Item(Item&&) from " << other.name << " " << &other << " to " << name << this << std::endl;
        name = std::move(other.name);
    }

    Item& operator=(const Item& other){
        std::cout << "operator=(const Item&) from " << other.name << " " << &other << " to " << name << " " << this << std::endl;
        if (this == &other) return *this;
        name = other.name;
        return *this;
    }

    Item& operator=(Item&& other) noexcept {
        std::cout << "operator=(Item&&) from " << other.name << " " << &other << " to " << name << " " << this << std::endl;
        if (this == &other) return *this;
        name = std::move(other.name);
        return *this;
    }
    
    bool operator==(const Item& other) const {
        return name == other.name;
    }

    ~Item(){
        std::cout << this << " destroy" << std::endl;
    }
    
    friend std::ostream& operator<<(std::ostream&, const Item&);
    friend bool operator<(const Item& lhs, const Item& rhs) {
        return lhs.name < rhs.name;
    }
};
std::ostream& operator<<(std::ostream& os, const Item& item) {
    os << "print: Item name = " << item.name << " " << &item;
    return os;
}

}

namespace jelly_test {

std::string random_string() {
    int length = 10;
    std::string re{};
    while (length--) {
        char c = std::rand() % 26 + 'a';
        re += c;
    }
    return re;
}

int random_int() {
    return std::rand() % INTMAX_MAX;
}

}