#include "Brain.hpp"

Brain::Brain() {}

Brain::Brain(const Brain &obj)
{
    for (int i = 0; i < 100; i++)
        ideas[i] = obj.ideas[i];
}

Brain &Brain::operator=(const Brain &obj)
{
    if (this != &obj)
    {
        for (int i = 0; i < 100; i++)
            ideas[i] = obj.ideas[i];
    }
    return *this;
}

Brain::~Brain() {}

void Brain::setIdea(int i, const std::string& idea)
{
    if (i >= 0 && i < 100)
        ideas[i] = idea;
}

std::string Brain::getIdea(int i) const
{
    if (i >= 0 && i < 100)
        return ideas[i];
    return "";
}