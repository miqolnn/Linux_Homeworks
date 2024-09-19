#pragma once

#include <iostream>
#include <cmath>

class Complex{
private:
	double real;
	double im;

public:
	Complex(double r = 0, double i = 0);

	double getReal();
	double getIm();
	double getAbs();

	Complex operator+(const Complex& a);
	Complex operator-(const Complex& a);
	Complex operator*(double a);

};
