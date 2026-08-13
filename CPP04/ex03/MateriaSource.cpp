#include "MateriaSource.hpp"

MateriaSource::MateriaSource()
{
    for (int i = 0; i < 4; i++)
        materias[i] = NULL;
}

MateriaSource::~MateriaSource()
{
    for (int i = 0; i < 4; i++)
    {
        if (materias[i])
            delete materias[i];
    }
}

MateriaSource::MateriaSource(const MateriaSource &other)
{
    for (int i = 0; i < 4; i++)
        this->materias[i] = NULL;
    for (int i = 0; i < 4; i++)
    {
        if (other.materias[i])
            this->materias[i] = other.materias[i]->clone();
    }
}

MateriaSource &MateriaSource::operator=(const MateriaSource &other)
{
    if (this != &other)
    {
        for (int i = 0; i < 4; i++)
        {
            delete materias[i];
            if (other.materias[i])
                materias[i] = other.materias[i]->clone();
            else
                materias[i] = NULL;
        }
    }
    return *this;
}

void MateriaSource::learnMateria(AMateria *m)
{
    if (!m)
        return;													
    for (int i = 0; i < 4; i++)
    {
        if (!materias[i])
        {
            materias[i] = m->clone();
            delete m;
            return;
        }
    }
    delete m;
}

AMateria *MateriaSource::createMateria(std::string const &type)
{
    for (int i = 0; i < 4; i++)
    {
        if (materias[i] && materias[i]->getType() == type)
        {
            return materias[i]->clone();
        }
    }
    return 0;
}