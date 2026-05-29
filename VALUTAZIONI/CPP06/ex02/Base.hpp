#pragma once
#include <iostream>
#include <cstdlib>

class Base{
public:
	virtual ~Base();
};

class A : public Base
{
public:
	A();
	virtual ~A();
};

class B : public Base
{
public:
	B();
	virtual ~B();
};

class C : public Base
{
public:
	C();
	virtual ~C();
};

Base* generate(void);
void identify(Base* p);
void identify(Base& p);