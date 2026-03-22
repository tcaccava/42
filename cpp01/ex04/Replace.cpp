#include "Replace.hpp"

bool replaceInFile(const std::string& inputFile, const std::string& toreplace, const std::string& replacewith)
{
    if (toreplace.empty()) {
        std::cerr << "String to replace must not be empty" << std::endl;
        return false;
    }

    std::ifstream input(inputFile);
    if (!input) {
        std::cerr << "Error opening input file" << std::endl;
        return false;
    }

    std::ofstream output(inputFile + ".replace");
    if (!output) {
        std::cerr << "Error opening output file" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(input, line)) {
        size_t pos = 0;
        while ((pos = line.find(toreplace, pos)) != std::string::npos) {
            line.erase(pos, toreplace.length());
            line.insert(pos, replacewith);
            pos += replacewith.length();
        }
        output << line << std::endl;
    }
    return true;
}