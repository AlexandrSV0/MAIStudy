/*  SAV   */
#include "mlisp.h"
double day__of__week/*2*/ ();
double zeller/*11*/ (double d, double m
	 , double y, double c);
double neg__to__pos/*21*/ (double d);
double birthday/*26*/ (double dw);
extern double dd/*41*/;
extern double mm/*42*/;
extern double yyyy/*43*/;
//________________ 
double day__of__week/*2*/ (){
 return
 zeller(dd, (10. + ((mm < 2. || mm == 2.) ? mm 
	 :true ? (mm - 12.)
	 : _infinity)), remainder((1. + (mm < 3. ? (yyyy - 2.) 
	 :true ? (yyyy - 1.)
	 : _infinity)), 100.), quotient((!(2. < mm) ? (yyyy - 1.) 
	 :true ? yyyy
	 : _infinity), 100.));
	 }

double zeller/*11*/ (double d, double m
	 , double y, double c){
 return
 neg__to__pos(remainder((d + y + quotient(((26. * m) - 2.), 10.) + quotient(y, 4.) + quotient(c, 4.) + (2. * (- c))), 7.));
	 }

double neg__to__pos/*21*/ (double d){
 return
 (d < 0. ? (d + 7.) 
	 :true ? d
	 : _infinity);
	 }

double birthday/*26*/ (double dw){
 display("Aleksandr Semin was born on ");
(dw == 1. ? (display("Monday "),
	1.) 
	 :dw == 2. ? (display("Tuesday "),
	2.) 
	 :dw == 3. ? (display("Wednesday "),
	3.) 
	 :dw == 4. ? (display("Thursday "),
	4.) 
	 :dw == 5. ? (display("Friday "),
	5.) 
	 :dw == 6. ? (display("Saturday "),
	6.) 
	 :true ? (display("Sunday "),
	7.)
	 : _infinity);
display(dd);
display(".");
display(mm);
display(".");
return
 yyyy;
	 }

double dd/*41*/ ( 23. );
double mm/*42*/ ( 09. );
double yyyy/*43*/ ( 2002. );
int main(){
display(birthday(day__of__week()));
	 newline();
	 std::cin.get();
 return 0;
}

