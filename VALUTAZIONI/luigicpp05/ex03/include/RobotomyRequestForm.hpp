#ifndef ROBOTO_HPP
#define ROBOTO_HPP

#include "AForm.hpp"

class RobotomyRequestForm : public AForm
{
  public:  
    RobotomyRequestForm(std::string target);
    void execute(Bureaucrat const& executor);
    //void do_something();
};

#endif

