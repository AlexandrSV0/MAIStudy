/*  SAV   */
#include "mlisp.h"
double __SAV__try/*2*/ (double x);
//________________ 
double __SAV__try/*2*/ (double x){
 x = sin(x)
return
 x;
	 }
int main(){
display(__SAV__try(2));
	 newline();
	 std::cin.get();
 return 0;
}

