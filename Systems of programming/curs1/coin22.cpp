/*  SAV   */
#include "mlisp.h"
double largest/*2*/ (double coins__set);
double count__change/*11*/ (double amount);
bool Shaeffer_Q/*27*/ (bool x_Q, bool y_Q);
double cc/*30*/ (double amount, double coins__set);
double denomination__list/*39*/ (double coins__set);
extern double VARIANT/*49*/;
extern double COINS/*50*/;
//________________ 
double largest/*2*/ (double coins__set){
 return
 (coins__set == 1. ? 1. 
	 :coins__set == 2. ? 3. 
	 :coins__set == 3. ? 10. 
	 :coins__set == 4. ? 20. 
	 :coins__set == 5. ? 50. 
	 :true ? 0.
	 : _infinity);
	 }

double count__change/*11*/ (double amount){
 display("______\n amount: ");
display(amount);
newline();
display("COINS: ");
display(COINS);
newline();
return
 ((amount < 0. || amount == 0. || COINS < 1. || largest(COINS) == 0.) ? (display("Improper parameter value!\ncount-change= "),
	-1.) 
	 :true ? (display("List of coin denominations: "),
	(denomination__list(COINS),
	(display("count-change= "),
	cc(amount, COINS))))
	 : _infinity);
	 }

bool Shaeffer_Q/*27*/ (bool x_Q, bool y_Q){
 return
 !(!((!(x_Q) || !(y_Q))));
	 }
double cc/*30*/ (double amount, double coins__set){
 return
 (amount == 0. ? 1. 
	 :Shaeffer_Q((1. < amount || 1. == amount),  0. < coins__set) ? 0. 
	 :true ? (cc(amount, (coins__set - 1.)) + cc((amount - largest(coins__set)), coins__set))
	 : _infinity);
	 }

double denomination__list/*39*/ (double coins__set){
 return
 (coins__set == 0. ? (newline(),
	0.) 
	 :true ? (display(largest(coins__set)),
	(display(" "),
	denomination__list((coins__set - 1.))))
	 : _infinity);
	 }

double VARIANT/*49*/ ( 21. );
double COINS/*50*/ ( 5. );
int main(){
display("Variant ");
	 display(VARIANT);
	 newline();
	 display(count__change(100.));
	 newline();
	  COINS = 13.;
	 display(count__change(100.));
	 newline();
	 display("(c) Aleksandr Semin 2022\n");
	 std::cin.get();
 return 0;
}

