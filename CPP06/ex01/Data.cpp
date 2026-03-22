#include "Data.hpp"

Data::Data(std::string _name,int _age,float _weight): name(_name), age(_age), weight(_weight){} 
Data::~Data() {}
Data::Data(const Data& other): name(other.name), age(other.age), weight(other.weight){}
Data &Data::operator=(const Data& other) {
    if(this != &other){
        name = other.name;
        age = other.age;
        weight = other.weight;
    }
    return *this;
}
