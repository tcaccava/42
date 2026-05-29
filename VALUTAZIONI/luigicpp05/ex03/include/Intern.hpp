#ifndef INTERN_HPP
#define INTERN_HPP

//class AForm;
#include "AForm.hpp"

class Intern
{
  public:  
    AForm *makeForm(std::string name, std::string target);
  
    static AForm *shrubbery_function(std::string target);
    static AForm *presidential_function(std::string target);
    static AForm *robotomy_function(std::string target);

};

#endif

