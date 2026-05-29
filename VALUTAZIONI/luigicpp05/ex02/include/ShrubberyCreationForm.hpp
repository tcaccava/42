#ifndef SHRUBBERY
#define SHRUBBERY

#include "AForm.hpp"

class ShrubberyCreationForm : public AForm
{
    public:

        ShrubberyCreationForm(const std::string target);
        void execute(Bureaucrat const& executor);
        //void do_something();
};

#endif
 
/*


ShrubberyCreationForm: Required grades: sign 145, exec 137
Creates a file <target>_shrubbery in the working directory and writes ASCII trees
inside it.


*/