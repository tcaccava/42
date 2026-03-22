#include "ScalarConverter.hpp"

int static getDecimalPlaces(const std::string &literal)
{
    size_t dot_pos = literal.find('.');
    if (dot_pos == std::string::npos)
        return 1;
    size_t len = literal.length();
    if (literal[len - 1] == 'f' || literal[len - 1] == 'd')
        len--;
    int decimal_places = len - dot_pos - 1;
    return decimal_places > 0 ? decimal_places : 1;
}

bool ScalarConverter::isCharLiteral(const std::string &literal)
{
    if (literal.length() == 3 && literal.front() == '\'' && literal.back() == '\'' && !isdigit(literal[1]))
        return true;

    if (literal.length() == 1 && !isdigit(literal[0]))
        return true;

    return false;
}

char decodeEscapeChar(const std::string &s)
{
    if (s.size() == 2 && s[0] == '\\')
    {
        switch (s[1])
        {
        case 'n':
            return '\n';
        case 't':
            return '\t';
        case 'r':
            return '\r';
        case '\\':
            return '\\';
        case '0':
            return '\0';
        case '\'':
            return '\'';
        case '"':
            return '"';
        default:
            return s[1];
        }
    }
    return s.empty() ? '\0' : s[0];
}

void ScalarConverter::handleCharLiteral(char c)
{
    if (!isprint(c))
        std::cout << "char: Non displayable" << std::endl;
    else
        std::cout << "char: '" << c << "'" << std::endl;

    int i = static_cast<int>(c);
    std::cout << "int: " << i << std::endl;
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "float: " << static_cast<float>(i) << "f" << std::endl;
    std::cout << "double: " << static_cast<double>(i) << std::endl;
}

bool ScalarConverter::isIntLiteral(const std::string &literal)
{
    if (literal.empty())
        return false;

    int i = 0;
    if (literal[i] == '-' || literal[i] == '+')
        i++;
    for (; i < literal.size(); i++)
        if (!isdigit(literal[i]))
            return false;
    return true;
}

void ScalarConverter::handleIntLiteral(long value)
{
    if (value >= 0 && value <= 127 && isprint(static_cast<char>(value)))
        std::cout << "char: '" << static_cast<char>(value) << "'" << std::endl;
    else if (value >= 0 && value <= 127 && !isprint(static_cast<char>(value)))
        std::cout << "char: Non displayable" << std::endl;
    else
        std::cout << "char: impossible" << std::endl;

    std::cout << "int: " << static_cast<int>(value) << std::endl;
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "float: " << static_cast<float>(value) << "f" << std::endl;
    std::cout << "double: " << static_cast<double>(value) << std::endl;
}

bool ScalarConverter::isFloatLiteral(const std::string &literal)
{
    if (literal == "+inff" || literal == "-inff" || literal == "nanf")
        return true;
    if (literal.empty() || literal.back() != 'f')
        return false;
    std::string core = literal.substr(0, literal.length() - 1);
    char *end;
    errno = 0;
    const char *string = core.c_str();
    float value = strtof(string, &end);
    return (*end == '\0' && errno != ERANGE);
}

void ScalarConverter::handleFloatLiteral(float value, const std::string &literal)
{
    if (std::isnan(value) || std::isinf(value))
        std::cout << "char: impossible" << std::endl;
    else if (value >= 0 && value <= 127 && value == static_cast<int>(value))
    {
        char c = static_cast<char>(value);
        if (isprint(c))
            std::cout << "char: " << c << "'" << std::endl;
        else
            std::cout << "char: Non displayable" << std::endl;
    }
    else
        std::cout << "char: impossible" << std::endl;
    if (std::isnan(value) || std::isinf(value))
        std::cout << "int: impossible" << std::endl;
    else
        std::cout << "int: " << static_cast<int>(value) << std::endl;
    int precision = getDecimalPlaces(literal);
    std::cout << std::fixed << std::setprecision(precision);
    std::cout << "float: " << value << "f" << std::endl;
    std::cout << "double: " << static_cast<double>(value) << std::endl;
}
bool ScalarConverter::isDoubleLiteral(const std::string &literal)
{
    if (literal == "+inf" || literal == "-inf" || literal == "nan")
        return true;
    char *end;
    errno = 0;
    const char *string = literal.c_str();
    double value = strtod(string, &end);
    return (*end == '\0' && errno != ERANGE);
}

void ScalarConverter::handleDoubleLiteral(double value, const std::string &literal)
{
    if (std::isnan(value) || std::isinf(value))
        std::cout << "char: impossible" << std::endl;
    else if (value >= 0 && value <= 127 && value == static_cast<int>(value))
    {
        char c = static_cast<char>(value);
        if (isprint(c))
            std::cout << "char: " << c << "'" << std::endl;
        else
            std::cout << "char: Non displayable" << std::endl;
    }
    else
        std::cout << "char: impossible" << std::endl;

    if (std::isnan(value) || std::isinf(value))
        std::cout << "int: impossible" << std::endl;
    else
        std::cout << "int: " << static_cast<int>(value) << std::endl;
    int precision = getDecimalPlaces(literal);
    std::cout << std::fixed << std::setprecision(precision);
    std::cout << "float: " << static_cast<float>(value) << "f" << std::endl;
    std::cout << "double: " << value << std::endl;
}

void ScalarConverter::convert(const std::string &literal)
{

    if (literal.length() == 3 && literal.front() == '\'' && literal.back() == '\'')
    {
        char c = literal[1];

        std::cout << "char: ";
        if (std::isprint(c))
            std::cout << "'" << c << "'" << std::endl;
        else
            std::cout << "Non displayable" << std::endl;

        std::cout << "int: " << static_cast<int>(c) << std::endl;
        std::cout << "float: " << static_cast<float>(c) << ".0f" << std::endl;
        std::cout << "double: " << static_cast<double>(c) << ".0" << std::endl;
        return;
    }

    if (literal.size() == 2 && literal[0] == '\\')
    {
        char c = decodeEscapeChar(literal);
        handleCharLiteral(c);
        return;
    }
    if (isCharLiteral(literal))
    {
        char c;
        if (literal.length() == 3)
            c = literal[1];
        else
            c = literal[0];
        handleCharLiteral(c);
        return;
    }
    if (isIntLiteral(literal))
    {
        char *end;
        errno = 0;
        const char *string = literal.c_str();
        long value = std::strtol(string, &end, 10);
        if (value < INT_MIN || value > INT_MAX || *end != '\0' || errno == ERANGE)
        {
            std::cout << "char: impossible\nint: impossible\nfloat: impossible\ndouble: impossible\n";
            return;
        }
        else
            handleIntLiteral(value);
        return;
    }

    if (isFloatLiteral(literal))
    {
        std::string core = literal.substr(0, literal.size() - 1);
        const char *string = core.c_str();
        char *end;
        float f = strtof(string, &end);
        handleFloatLiteral(f, literal);
        return;
    }

    if (isDoubleLiteral(literal))
    {
        const char *string = literal.c_str();
        char *end;
        double d = strtod(string, &end);
        handleDoubleLiteral(d, literal);
        return;
    }
    std::cout << "char: impossible\nint: impossible\nfloat: impossible\ndouble: impossible\n";
}
