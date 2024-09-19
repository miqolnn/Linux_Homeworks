#pragma once

#include "Sort.h"

void sort(std::vector<Complex>& a){
	int n = a.size();
	for(int i = 0; i < n - 1; i++)
		for(int j = 0; j < n - i - 1; j++)
			if(a[j].getAbs() > a[j+1].getAbs())
				std::swap(a[j],a[j+1]);


}

