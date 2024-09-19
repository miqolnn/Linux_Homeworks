#pragma once

#include "Complex.h"

Complex::Complex(double r, double i):real(r),im(i){}

double Complex::getReal(){
	return real;
}

double Complex::getIm(){
	return im;
}

double Complex::getAbs(){
	return std::sqrt(real * real + im * im);
}

Complex Complex::operator+(const Complex& a){
	return Complex(real + a.real, im + a.im);
}

Complex Complex::operator-(const Complex& a){
	return Complex(real-a.real, im - a.im);
}

Complex Complex::operator*(double a){
	return Complex(real*a, im*a);
}




