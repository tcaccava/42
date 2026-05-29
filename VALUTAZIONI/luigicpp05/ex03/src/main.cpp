
#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"
#include <map>
#include <vector>
#include "Intern.hpp"

int all_bureaucrats_sign_and_execute_the_form(std::vector<Bureaucrat *>& b_vector, AForm& form)
{   
    size_t i = 0;

    while (i < b_vector.size())
    {
        try {
            form.beSigned(*b_vector[i]);
            form.execute(*b_vector[i]);
        } catch (std::exception& e){
            std::cout << e.what() << std::endl;
            return (0);
        }
        i++;
    }
    return (1);
}


int create_bureaucrat_vector(std::vector<Bureaucrat *>& b_vector, std::vector<std::pair<std::string, int> >& v_vector)
{
    size_t v_vector_size = v_vector.size();

    size_t i = 0;
    while (i < v_vector_size)
    {
        try {
            b_vector.push_back(new Bureaucrat(v_vector[i].first, v_vector[i].second));
            std::cout << *b_vector[i];
        } catch(std::exception& e) {
            return (0);
        }
        i++;
    }
    return (1);
}

int main ()
{
    srandom(time(0));

    std::vector<Bureaucrat *> b_vector;
    std::vector<std::pair<std::string, int> > v_vector;
    
    //billy
    std::pair<std::string, int> billy_values("Billy", 1);
    v_vector.push_back(billy_values);
    //mario
    std::pair<std::string, int> mario_values("Mario", 45);
    v_vector.push_back(mario_values);

    if (create_bureaucrat_vector(b_vector, v_vector) == 0)
     return (0);
    

    ShrubberyCreationForm shrubbery("shruberry");
    all_bureaucrats_sign_and_execute_the_form(b_vector, shrubbery);
   
    RobotomyRequestForm robot("robotino");
    all_bureaucrats_sign_and_execute_the_form(b_vector, robot);

  
    PresidentialPardonForm president("president");
    all_bureaucrats_sign_and_execute_the_form(b_vector, president);



    Intern a;
    
    a.makeForm("Shrubbery Creation", "pasticcio");


   
    int i = 0;
    while (i < 2)
    {
        delete b_vector[i];
        i++;
    }
        
}

/*PresidentialPardonForm: Required grades: sign 25, exec 5
Informs that <target> has been pardoned by Zaphod Beeblebrox.
All of them take only one parameter in thei*/