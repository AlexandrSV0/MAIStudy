/*  SAV   */
#include "mlisp.h"
double root/*2*/ (double a, double b);
double half__interval/*14*/ (double a, double b
	 , double fa, double fb);
double __SAV__try/*26*/ (double neg__point, double pos__point);
bool close__enough_Q/*40*/ (double x, double y);
double average/*42*/ (double x, double y);
extern double tolerance/*44*/;
extern double total__iterations/*45*/;
double f/*46*/ (double z);
//________________ 
double root/*2*/ (double a, double b){
 double temp/*3*/ ( 0. );
 temp = half__interval(a, b, f(a), f(b));
display("Total number of iteranions=");
display(total__iterations);
newline();
display("[");
display(a);
display(" , ");
display(b);
display("]");
return
 temp;
	 }

double half__interval/*14*/ (double a, double b
	 , double fa, double fb){
 double root/*15*/ ( 0. );
 total__iterations = 0.;
 root = (!((!(fa < 0.) || !(0. < fb))) ? __SAV__try(a, b) 
	 :!((!(0. < fa) || !(fb < 0.))) ? __SAV__try(b, a) 
	 :true ? (b + 1.)
	 : _infinity);
newline();
return
 root;
	 }

double __SAV__try/*26*/ (double neg__point, double pos__point){
 double midpoint/*27*/ ( 0. );
double test__value/*28*/ ( 0. );
 midpoint = average(neg__point, pos__point);
display("+");
 total__iterations = (total__iterations + 1.);
return
 (close__enough_Q(neg__point,  pos__point) ? midpoint 
	 :true ? ( test__value = f(midpoint),
	(0. < test__value ? __SAV__try(neg__point, midpoint) 
	 :test__value < 0. ? __SAV__try(midpoint, pos__point) 
	 :true ? midpoint
	 : _infinity))
	 : _infinity);
	 }

bool close__enough_Q/*40*/ (double x, double y){
 return
 abs((x - y)) < tolerance;
	 }
double average/*42*/ (double x, double y){
 return
 ((x + y) * (1. / 2.));
	 }

double tolerance/*44*/ ( 1e-3 );
double total__iterations/*45*/ ( 0. );
double f/*46*/ (double z){
 return
 (z - expt((z - 2.), 3.) - atan(z) - 8e-1);
	 }

int main(){
display((e < 0. ? 1. 
	 :e == 0. ? 2. 
	 :0. < e ? 3.
	 : _infinity));
	 newline();
	 display("Variant 206-21\n");
	 display(root(1., 151e-2));
	 newline();
	 display("(c) Aleksandr Semin 2022\n");
	 std::cin.get();
 return 0;
}

