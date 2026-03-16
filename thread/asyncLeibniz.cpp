#include <vector>
#include <future>
#include "asyncLeibniz.h"


//р€д лейбница 1 - 1/3 + 1/5 - 1/7 + 1/9 - 1/11 и т.д.
//сумма р€да лейбница равна pi/4

#define coefPI 4.0

double leibnizPart(long long start, long long end) //star и end нужны чтобы разделить вычислени€ на несколько независимых кусков
{
	double sum = 0.0;

	for (long long i = start; i < end; ++i)
	{
		int sign = (i % 2 == 0) ? 1 : -1;
		sum = sum + (sign * (coefPI / (2 * i + 1)));//2 * i + 1 вычисление нечЄтных чисел р€да, например, если i = 2,
		                                            //то ему соответствует чиcло 5 = i + (i + 1) = 2 + 3
	}
	return sum;
}


double leibniz(long long numSummand, int countPart)
{
	if (countPart <= 0) return 0.0; 
	if (countPart > numSummand) countPart = numSummand;

	double sum = 0.0;

	std::vector<std::future<double>>futures;
	futures.reserve(countPart); //чтобы не было переаллокаций 

	int sizePart = numSummand / countPart;   
	int remainder = numSummand % countPart;    

	for (int part = 0; part < countPart; ++part)
	{
		long long shift = part < remainder ? part : remainder; //смещение
		long long start = part * sizePart + shift;
		long long end = start + sizePart + (part < remainder ? 1 : 0); //если номер части меньше остатка, то добавл€ем 1 - распредел€ем остаток
		                                                    //если больше - остаток закончилс€ 
		futures.push_back(std::async(std::launch::async, leibnizPart, start, end));
	}

	for (std::future<double>& f : futures)
	{
		sum = sum + f.get();
	}

	return sum;
}
