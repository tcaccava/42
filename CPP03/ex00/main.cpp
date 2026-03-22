#include "ClapTrap.hpp"

int main() {
    ClapTrap a("Aldo");
    ClapTrap b("Bruno");

    a.attack("Bruno");
    b.takeDamage(3);

    b.attack("Aldo");
    a.takeDamage(5);

    a.beRepaired(4);
    b.beRepaired(2);

    for (int i = 0; i < 10; i++) {
        b.attack("Aldo");
    }

    b.attack("Aldo");

    a.takeDamage(100);

    a.attack("Bruno");
    a.beRepaired(5);

    return 0;
}
