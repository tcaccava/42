#include "Functions.hpp"
#include "Base.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"

Base* generate(void){
  switch (rand() % 3) {
    case 0: return new A();
    case 1: return new B();
    case 2: return new C();
  }
  return NULL;
}

void identify(Base *p){
    if (A *a = dynamic_cast<A*>(p)) std::cout << "A" << std::endl;
    else if (B *b = dynamic_cast<B*>(p)) std::cout << "B" << std::endl;
    else if (C *c = dynamic_cast<C*>(p)) std::cout << "C" << std::endl;
}

void identify(Base &p) {
    try{
        A &a = dynamic_cast<A&>(p);
        std::cout << "A" << std::endl;
        return;
    } catch(std::bad_cast &e){}
    try {
        B &b = dynamic_cast<B&>(p);
        std::cout << "B" << std::endl;
        return;
    }catch(std::bad_cast &e){}
    try {
        C &c = dynamic_cast<C&>(p);
        std::cout << "C" << std::endl;
        return;
    }catch(std::bad_cast &e){}
}