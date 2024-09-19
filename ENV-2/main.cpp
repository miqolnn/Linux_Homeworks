#pragma once

#include <iostream>
#include <vector>

#include "Complex.h"
#include "Sort.h"

int main(){

	std::vector<Complex> a = { Complex(2,3), Complex(3,3), Complex(1,1), Complex(0,5) };
	int n = a.size();

	for(int i = 0; i < n; i++)
		std::cout<<a[i].getReal()<<","<<a[i].getIm()<<" ";
	std::cout<<std::endl;

	sort(a);

	for(int i = 0; i < n; i++)
                std::cout<<a[i].getReal()<<","<<a[i].getIm()<<" ";
	
	std::cout<<std::endl;

	return 0;
}
