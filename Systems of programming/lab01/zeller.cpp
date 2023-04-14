//zeller.cpp
#include "mlisp.h"
double day__of__week();
double zeller(double d, double m, double y, double c);
double neg__to__pos(double d);
double birthday(double dw);
extern double dd;
extern double mm;
extern double yyyy;

double day__of__week() {
    double month = 10. + (mm <= 2. ? mm : mm - 12.);
    double year = remainder(1. + (mm < 3. ? yyyy-2. : yyyy-1.), 100.);
    double cent = quotient(mm <= 2. ? yyyy - 1. : yyyy, 100.);
    return zeller(dd, month, year, cent);
}

double zeller(double d, double m, double y, double c) {
    return neg__to__pos(remainder(d + y + quotient(m * 26. - 2., 10.) + quotient(y, 4.) + quotient(c,4.) + 2.* (-c), 7.));
}

double neg__to__pos(double d) {
    return (d < 0. ? d + 7. : true ? d : _infinity);
}

double birthday(double dw) {
    display("Aleksandr Semin was born on ");
    display(dw == 1. ? "Monday "
        : dw == 2. ? "Tuesday "
        : dw == 3. ? "Wednesday "
        : dw == 4. ? "Thursday "
        : dw == 5. ? "Friday "
        : dw == 6. ? "Saturday "
        : "Sunday ");
    display(dd), display(".");
    display(mm), display(".");
 return  yyyy;
}

double dd = 23.;
double mm = 9.;
double yyyy = 2002.;

int main() {
    display(birthday (day__of__week()));
    newline();
    std::cin.get();
    return 0;
}
