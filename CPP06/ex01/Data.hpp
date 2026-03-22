#ifndef DATA_HPP
#define DATA_HPP

#include <string>

class Data
{
public:
    Data(std::string _name, int _age, float _weight);
    ~Data();
    Data(const Data &other);
    Data &operator=(const Data &other);

private:
    int age;
    std::string name;
    float weight;
};

#endif