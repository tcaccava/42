#include "BitcoinExchange.hpp"

int main(int c, char **v) {
    if(c != 2){
        std::cerr << "Error: could not open file" << std::endl;
        return 1;
    }

    BitcoinExchange exchange;
    if(!exchange.create_database("data.csv"))
        return 1;
    exchange.handle_input(v[1]);
    return 0;
}