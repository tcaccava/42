#include "Bureaucrat.hpp"
#include "Form.hpp"
#include <iostream>

int main()
{
    std::cout << "=== TESTING BUREAUCRAT AND FORM CLASSES ===" << std::endl;

    try
    {
        std::cout << "\n=== CREATING BUREAUCRATS ===" << std::endl;

        // Create bureaucrats with different grades
        Bureaucrat highGrade("Alice", 1);
        Bureaucrat midGrade("Bob", 50);
        Bureaucrat lowGrade("Charlie", 150);

        std::cout << highGrade << std::endl;
        std::cout << midGrade << std::endl;
        std::cout << lowGrade << std::endl;

        std::cout << "\n=== CREATING FORMS ===" << std::endl;

        // Create forms with different requirements
        Form easyForm("Easy Form", 100, 80);
        Form mediumForm("Medium Form", 50, 30);
        Form hardForm("Hard Form", 10, 5);

        std::cout << easyForm << std::endl;
        std::cout << mediumForm << std::endl;
        std::cout << hardForm << std::endl;

        std::cout << "\n=== TESTING FORM SIGNING ===" << std::endl;

        // Test signing forms with different bureaucrats
        std::cout << "--- Low grade bureaucrat trying to sign easy form ---" << std::endl;
        lowGrade.signForm(easyForm);

        std::cout << "--- Low grade bureaucrat trying to sign hard form ---" << std::endl;
        lowGrade.signForm(hardForm);

        std::cout << "--- Mid grade bureaucrat trying to sign medium form ---" << std::endl;
        midGrade.signForm(mediumForm);

        std::cout << "--- High grade bureaucrat trying to sign hard form ---" << std::endl;
        highGrade.signForm(hardForm);

        std::cout << "\n=== TESTING PROMOTIONS AND DEMOTIONS ===" << std::endl;

        Bureaucrat testBureaucrat("Test", 75);
        std::cout << "Initial: " << testBureaucrat << std::endl;

        // Test promotions
        for (int i = 0; i < 3; i++)
        {
            testBureaucrat.Promotion();
            std::cout << "After promotion: " << testBureaucrat << std::endl;
        }

        // Test demotions
        for (int i = 0; i < 5; i++)
        {
            testBureaucrat.Demotion();
            std::cout << "After demotion: " << testBureaucrat << std::endl;
        }

        std::cout << "\n=== TESTING BOUNDARY CONDITIONS ===" << std::endl;

        // Test creating bureaucrat with grade 1 and trying to promote
        std::cout << "--- Testing promotion at grade 1 ---" << std::endl;
        Bureaucrat topBureaucrat("Top", 1);
        std::cout << topBureaucrat << std::endl;
        try
        {
            topBureaucrat.Promotion();
        }
        catch (const std::exception& e)
        {
            std::cout << "Exception caught: " << e.what() << std::endl;
        }

        // Test creating bureaucrat with grade 150 and trying to demote
        std::cout << "--- Testing demotion at grade 150 ---" << std::endl;
        Bureaucrat bottomBureaucrat("Bottom", 150);
        std::cout << bottomBureaucrat << std::endl;
        try
        {
            bottomBureaucrat.Demotion();
        }
        catch (const std::exception& e)
        {
            std::cout << "Exception caught: " << e.what() << std::endl;
        }

        std::cout << "\n=== TESTING INVALID BUREAUCRAT CREATION ===" << std::endl;

        // Test creating bureaucrat with invalid grades
        try
        {
            Bureaucrat invalidHigh("Invalid High", 0);
        }
        catch (const Bureaucrat::GradeTooHighException& e)
        {
            std::cout << "Exception caught for grade 0: " << e.what() << std::endl;
        }

        try
        {
            Bureaucrat invalidLow("Invalid Low", 151);
        }
        catch (const Bureaucrat::GradeTooLowException& e)
        {
            std::cout << "Exception caught for grade 151: " << e.what() << std::endl;
        }

        std::cout << "\n=== TESTING INVALID FORM CREATION ===" << std::endl;

        // Test creating forms with invalid grades
        try
        {
            Form invalidForm1("Invalid Form 1", 0, 50);
        }
        catch (const Form::GradeTooLowException& e)
        {
            std::cout << "Exception caught for form with sign grade 0: " << e.what() << std::endl;
        }

        try
        {
            Form invalidForm2("Invalid Form 2", 50, 151);
        }
        catch (const Form::GradeTooLowException& e)
        {
            std::cout << "Exception caught for form with exec grade 151: " << e.what() << std::endl;
        }

        std::cout << "\n=== TESTING COPY CONSTRUCTOR AND ASSIGNMENT ===" << std::endl;

        // Test copy constructor and assignment operator
        Bureaucrat original("Original", 42);
        Bureaucrat copy1(original);
        Bureaucrat copy2("Temporary", 100);

        std::cout << "Original: " << original << std::endl;
        std::cout << "Copy1: " << copy1 << std::endl;
        std::cout << "Copy2 before assignment: " << copy2 << std::endl;

        copy2 = original;
        std::cout << "Copy2 after assignment: " << copy2 << std::endl;

        // Test form copy
        Form originalForm("Original Form", 50, 30);
        Form copyForm(originalForm);

        std::cout << "Original Form: " << originalForm << std::endl;
        std::cout << "Copy Form: " << copyForm << std::endl;

        // Sign original and check if copy is affected
        highGrade.signForm(originalForm);
        std::cout << "After signing original:" << std::endl;
        std::cout << "Original Form: " << originalForm << std::endl;
        std::cout << "Copy Form: " << copyForm << std::endl;

        std::cout << "\n=== TESTING MULTIPLE SIGNING ATTEMPTS ===" << std::endl;

        Form multiSignForm("Multi Sign", 75, 50);
        std::cout << "Initial form: " << multiSignForm << std::endl;

        // Try signing with insufficient grade
        lowGrade.signForm(multiSignForm);
        std::cout << "After failed signing: " << multiSignForm << std::endl;

        // Sign with sufficient grade
        midGrade.signForm(multiSignForm);
        std::cout << "After successful signing: " << multiSignForm << std::endl;

        // Try signing again (should still work)
        highGrade.signForm(multiSignForm);
        std::cout << "After second signing: " << multiSignForm << std::endl;

        std::cout << "\n=== ALL TESTS COMPLETED SUCCESSFULLY ===" << std::endl;

    }
    catch(const std::exception& e)
    {
        std::cout << "Unexpected exception: " << e.what() << std::endl;
    }

    return 0;
}