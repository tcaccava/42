#include "RobotomyRequestForm.hpp"
#include <unistd.h>   //for sleep

RobotomyRequestForm::RobotomyRequestForm(std::string target) : AForm(target, 72, 54)
{

}


void RobotomyRequestForm::execute(Bureaucrat const &executor)
{
    std::cout << "Bureaucrat " << executor.getName() << ": execution in progress ...\n" << std::flush;;
    sleep(1);
    if (!this->getSigned())
       throw  (GradeTooLowException());
    if (this->getGradeToExec() < executor.getGrade())
        throw (GradeTooLowException());
    //specific behaviour ...overloading
    int random_num;

    std::cout << "drrrr noise drrrr noise drrrrr noise drrrrr\n";

    /* in main I used srand() to initialize rand.
        Thanks to that inizializzation rand() starts always 
        with a different value. To be more precise it starts
        with current time-stamp. 
        Indeed srand() is initialize as follow:
        srand(time(null)).
        So i passede time(null) to srand.
        time(null) is timestamp from 1970 and it changes evry second.
        In this way if I call now rand(), then it executes with the current timestamp,
        but if I call again later, it executes with the different and newest timestamp. 
        To be more precise, when we initialize srand(), we pass to it the seed (seme in italiano).
        If we passed the same seed e.g. 42, then random will produce the same random_number each time it is called.
        That's why we pass time(null) as seed, so that the seed is different evry second,
        and consequently random() will give us always a different time.
    */
    random_num = rand();

    if (random_num % 2)
        std::cout << getName() << " has successfully robotomized!!\n";
    else    
        std::cout << "robotomy failed!!\n";
    std::cout << "The bureucrat named " << executor.getName() << " has executed the form\n";
}

