#include "poly.h"

/******you code these 3 functions******/
//function returns the greatest common denominator of a and b using Euclid's algorithm.
int GCD(int a, int b){
	return (b == 0) ? a : GCD(b, a % b);
}

//returns the quotient of dividing two functions by calling the divide() function
Polynomial operator/(const Polynomial& p1, const Polynomial& p2){
	Polynomial quotient, remainder;
	divide(p1, p2, quotient, remainder);
	return quotient;
}

//returns the remainder of dividing two functions by calling the divide() function
Polynomial operator%(const Polynomial& p1, const Polynomial& p2){
	Polynomial quotient, remainder;
	divide(p1, p2, quotient, remainder);
	return remainder;
}
//divides two Polynomial, takes in referances to quatient and remainder so they can be used anywere in the recursion function 
Polynomial divide(const Polynomial& p1, const Polynomial& p2, Polynomial& quotient, Polynomial& remainder) {
	auto it1 = p1.poly_.begin();
	auto it2 = p2.poly_.begin();
	Term term1 = *it1;
	Term term2 = *it2;
	Fraction merge;//step 2
	Polynomial step2Poly, step3Poly, step4Poly;

	//step1: If the highest degree in p2 > highest degree in p1, division is complete.
	if (term2.exponent_ > term1.exponent_) {
		remainder = p1;
		return p1;
	}
	//step 2: simplify coefficient merge 
	merge = term1.coefficient_ / term2.coefficient_;
	merge.reduce();

	//step2:create a term
	Term step2(merge.numerator(), merge.denominator(), term1.exponent_ - term2.exponent_);

	//step 3: place term in a list so that the multiplication operator can be used
	step2Poly.addTerm(step2.coefficient_, step2.exponent_);

	//step 3: multiply that term by each term of p2
	step3Poly = step2Poly * p2;

	//step 4 : subtract the polynomial created in step 3 from p1
	step4Poly = p1 - step3Poly;

	//term created in step 2 is a part of the quotient, each term in the recursion is added to the quotient list giving the full answer when the function ends
	quotient.addTerm(step2.coefficient_, step2.exponent_);

	//step 5: find the next term by repeating steps 1 to 4 using polynomial from step 4 as the new p1 until step 1 is true
	return divide(step4Poly, p2, quotient, remainder);
}
int abs(int v){
	return (v>=0)?v:-v;
}

Fraction::Fraction(){
	numerator_=0;
	denominator_=1;
}
Fraction::Fraction(int num,int denom){
	set(num,denom);
}
void Fraction::set(int num,int denom){
	numerator_=num;
	denominator_=denom;
	reduce();
}
void Fraction::reduce(){
	int divisor=GCD(numerator_,denominator_);
	numerator_=numerator_/divisor;
	denominator_=denominator_/divisor;
	//always store negative in numerator
	if((numerator_ < 0 && denominator_ < 0) || 
		(numerator_ > 0 && denominator_ < 0)){
		numerator_=numerator_*-1;
		denominator_=denominator_*-1;
	}
}
bool isSame(const Term& a,const Term& b)
{
	bool rc=false;
	if(a.coefficient_==b.coefficient_ && a.exponent_==b.exponent_){
		rc=true;
	}
	return rc;
}
Fraction operator+(const Fraction& f1, const Fraction& f2){
	return Fraction(f1.numerator_*f2.denominator_+f2.numerator_*f1.denominator_, f1.denominator_*f2.denominator_);
	
}
Fraction operator-(const Fraction& f1, const Fraction& f2){
	return Fraction(f1.numerator_*f2.denominator_-f2.numerator_*f1.denominator_, f1.denominator_*f2.denominator_);

}
Fraction operator*(const Fraction& f1, const Fraction& f2){
	return Fraction(f1.numerator_*f2.numerator_, f1.denominator_*f2.denominator_);
}
Fraction operator/(const Fraction& f1, const Fraction& f2){
	return Fraction(f1.numerator_*f2.denominator_, f1.denominator_*f2.numerator_);
}
ostream& operator << (ostream& os, const Fraction& data){
	if(abs(data.numerator_) < abs(data.denominator_)){
		if(data.numerator_!=0){
			os << data.numerator_ << "/"  << data.denominator_;
		}
		else{
			os << "0";
		}
	}
	else{
		os << data.numerator_/data.denominator_;
		if(data.numerator_ % data.denominator_){
			os << " " << abs(data.numerator_%data.denominator_) << "/";
			os << data.denominator_;
		}
	}
	return os;
}

const Fraction& Fraction::operator+=(const Fraction& rhs){
	*this=*this+rhs;
	return *this;
}
const Fraction& Fraction::operator-=(const Fraction& rhs){
	*this=*this-rhs;
	return *this;
}
const Fraction& Fraction::operator*=(const Fraction& rhs){
	*this = *this * rhs;
	return *this;
}
const Fraction& Fraction::operator/=(const Fraction& rhs){
	*this = *this / rhs;
	return *this;
}
bool operator==(const Fraction& f1, const Fraction& f2){
	bool rc=true;
	if((f1.numerator_!=f2.numerator_) || (f1.denominator_!=f2.denominator_)){
		rc=false;
	}
	return rc;
}
bool operator!=(const Fraction& f1, const Fraction& f2){
	bool rc=true;
	if((f1.numerator_==f2.numerator_)  && (f1.denominator_==f2.denominator_)){
		rc=false;
	}
	return rc;
}
Polynomial::Polynomial(int data[][3],int numTerms){
	for(int i=0;i<numTerms;i++){
		poly_.insert(Term(data[i][0],data[i][1],data[i][2]));
	}
}
Polynomial::Polynomial(const Polynomial& poly){
	poly_=poly.poly_;
}
const Polynomial& Polynomial::operator=(const Polynomial& poly){
	poly_=poly.poly_;
	return *this;
}
void Polynomial::clear(){
	poly_.erase(poly_.begin(),poly_.end());
}
Polynomial::~Polynomial(){
}
void Polynomial::addTerm(const Fraction& coefficient, int degree){
	Term t(coefficient,degree);
	//search for a term with the same exponent
	auto it = poly_.search(t);
	if(it==poly_.end()){
		poly_.insert(t);
	}
	else{
		(*it).coefficient_+=coefficient;
	}

}
//Made solve function O(n) using horners method
Fraction Polynomial::solve(const Fraction& x) const{
	Fraction rc;
	auto it = poly_.begin();
	Term t = *it;
    //load first term
	rc = t.coefficient_;
	it++;

	for (it;it != poly_.end();it++) {
		t = *it;
		rc = rc*x + t.coefficient_; 
	}
	rc = rc*x;
	return rc;
}
const Polynomial& Polynomial::operator+=(const Polynomial& rhs){
	auto it1=poly_.begin();
	auto it2=rhs.poly_.begin();
	while(it1!=poly_.end() && it2!=rhs.poly_.end()){
		if(*it1 == *it2){
			(*it1).coefficient_ += (*it2).coefficient_;
			if((*it1).coefficient_==Fraction(0,1)){
				it1=poly_.erase(it1);
			}
			else{
				it1++;
			}
			it2++;
		}
		else if(*it1 < *it2){
			it1++;			
		}
		else{
			addTerm((*it2).coefficient_ ,(*it2).exponent_);
			it2++;
		}
	}
	while(it2!=rhs.poly_.end()){
		addTerm((*it2).coefficient_ ,(*it2).exponent_);
		it2++;
	}

	return *this;
}
const Polynomial& Polynomial::operator-=(const Polynomial& rhs){
	auto it1=poly_.begin();
	auto it2=rhs.poly_.begin();
	while(it1!=poly_.end() && it2!=rhs.poly_.end()){
		if(*it1 == *it2){
			(*it1).coefficient_ -= (*it2).coefficient_;
			if((*it1).coefficient_==Fraction(0,1)){
				it1=poly_.erase(it1);
			}
			else{
				it1++;
			}
			it2++;
		}
		else if(*it1 < *it2){
			it1++;			
		}
		else{
			addTerm(Fraction(-1,1)* (*it2).coefficient_ ,(*it2).exponent_);
			it2++;
		}
	}
	while(it2!=rhs.poly_.end()){
		addTerm(Fraction(-1,1)*(*it2).coefficient_ ,(*it2).exponent_);
		it2++;
	}
	return *this;

}
const Polynomial& Polynomial::operator*=(const Polynomial& rhs){
	auto it2=rhs.poly_.begin();
	Polynomial temp;
	while(it2!=rhs.poly_.end()){
		auto it1=poly_.begin();
		while(it1!=poly_.end()){
			temp.addTerm((*it1).coefficient_*(*it2).coefficient_,(*it1).exponent_+(*it2).exponent_);
			it1++;
		}
		it2++;
	}
	*this = temp;
	return *this;
}



Polynomial operator+(const Polynomial& p1, const Polynomial& p2){
	Polynomial rc=p1;
	rc+=p2;
	return rc;
}
Polynomial operator-(const Polynomial& p1, const Polynomial& p2){
	Polynomial rc=p1;
	rc-=p2;
	return rc;
}
Polynomial operator*(const Polynomial& p1, const Polynomial& p2){
	Polynomial rc=p1;
	rc*=p2;
	return rc;
}

bool operator==(const Polynomial& p1, const Polynomial& p2){
	auto it1=p1.poly_.begin();
	auto it2=p2.poly_.begin();
	bool rc=true;
	while(rc && it1 != p1.poly_.end() && it2!=p2.poly_.end()){
		if(!isSame(*it1,*it2)){
			rc=false;
		}
		it1++;
		it2++;
	}
	if(rc){
		return (it1==p1.poly_.end() && it2==p2.poly_.end());
	}
	else{
		return rc;
	}
}
ostream& operator<<(ostream& os, const Polynomial& dat){
	auto it=dat.poly_.begin();
	if(it!=dat.poly_.end()){
		os << (*it).coefficient_ << " x^" << (*it).exponent_;
		it++;
		while(it != dat.poly_.end()){
			os << " + ";
			os << (*it).coefficient_;
			if((*it).exponent_ > 1) {
				os << " x^" << (*it).exponent_;
			}
			else if ((*it).exponent_==1){
				os << " x";
			}
			it++;
		}
	}
	else{
		os << "0" << endl;
	}
	return os;
}
