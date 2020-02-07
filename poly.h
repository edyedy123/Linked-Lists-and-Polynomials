#include "sortedlist.h"
#include <iostream>
#ifndef POLY_H
#define POLY_H
using namespace std;

class Fraction{
private:
	int numerator_;
	int denominator_;
	friend Fraction operator+(const Fraction& f1, const Fraction& f2);
	friend Fraction operator-(const Fraction& f1, const Fraction& f2);
	friend Fraction operator*(const Fraction& f1, const Fraction& f2);
	friend Fraction operator/(const Fraction& f1, const Fraction& f2);
	friend ostream& operator << (ostream& os, const Fraction& data);
	friend bool operator==(const Fraction& f1, const Fraction& f2);
	friend bool operator!=(const Fraction& f1, const Fraction& f2);
	
public:
	Fraction();
	Fraction(int num,int denom);
	void set(int num,int denom);
	int numerator(){return numerator_;}
	int denominator(){return denominator_;}
	void reduce();
	const Fraction& operator+=(const Fraction& rhs);
	const Fraction& operator-=(const Fraction& rhs);
	const Fraction& operator/=(const Fraction& rhs);
	const Fraction& operator*=(const Fraction& rhs);
};
Fraction operator+(const Fraction& f1, const Fraction& f2);
Fraction operator-(const Fraction& f1, const Fraction& f2);
Fraction operator*(const Fraction& f1, const Fraction& f2);
Fraction operator/(const Fraction& f1, const Fraction& f2);
bool operator==(const Fraction& f1, const Fraction& f2);
bool operator!=(const Fraction& f1, const Fraction& f2);
ostream& operator << (ostream& os, const Fraction& data);
struct Term{
	Fraction coefficient_;
	unsigned int exponent_;
	Term(int numerator,int denominator,int exponent):
								coefficient_(numerator,denominator){
		exponent_=exponent;
	}
	Term(const Fraction& coefficient,int exponent){
		coefficient_=coefficient;
		exponent_=exponent;
	}
	Term():coefficient_(Fraction(1,1)),exponent_(0){
	}
	bool operator==(const Term& rhs){
		return exponent_==rhs.exponent_;
	}
	bool operator!=(const Term& rhs){
		return exponent_!=rhs.exponent_;
	}
	bool operator<(const Term& rhs) const{// needs to be cosnt for insert to work
		//this is correct... we are using this to sort our terms
		//and we want the terms with higher exponent at the front
		return exponent_ > rhs.exponent_;
	}
};
bool isSame(const Term& a,const Term& b);

class Polynomial{

	SortedList<Term> poly_;
	void clear();
public:
	Polynomial(){}
	Polynomial(int data[][3],int numTerms);
	Polynomial(const Polynomial& poly);
	const Polynomial& operator=(const Polynomial& poly);
	~Polynomial();
	void addTerm(const Fraction& coefficient, int degree);
	Fraction solve(const Fraction& x) const;
	const Polynomial& operator+=(const Polynomial& rhs);
	const Polynomial& operator-=(const Polynomial& rhs);
	const Polynomial& operator*=(const Polynomial& rhs);
	friend Polynomial operator+(const Polynomial& p1, const Polynomial& p2);
	friend Polynomial operator-(const Polynomial& p1, const Polynomial& p2);
	friend Polynomial operator*(const Polynomial& p1, const Polynomial& p2);
	friend Polynomial operator/(const Polynomial& p1, const Polynomial& p2);
	friend Polynomial operator%(const Polynomial& p1, const Polynomial& p2);
	friend bool operator==(const Polynomial& p1, const Polynomial& p2);
	friend ostream& operator<<(ostream& os, const Polynomial& dat);

	//divides two Polynomial, takes in referances to quatient and remainder so they can be used anywere in the recursion function 
	friend Polynomial divide(const Polynomial& p1, const Polynomial& p2, Polynomial& quotient, Polynomial& remainder);
	
};


#endif