/*  SAV   */
#include "mlisp.h"
double neg__to__pos/*1*/ (double d);
//________________ 
double neg__to__pos/*1*/ (double d){
 return
 (d < 0. ? (d + 7.) 
	 :true ? d
	 : _infinity);
	 }

int main(){
	 display("No calculations!");
	 newline();
	  std::cin.get();
	 return 0;
	 }

