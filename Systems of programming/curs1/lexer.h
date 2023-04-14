//                 lexer.h 2021
#ifndef LEXER_H
#define LEXER_H
#include "baselexer.h"
class tLexer:public tBaseLexer{
public:
//конструктор
 tLexer():tBaseLexer(){
//создать автоматы:

//________________________________________

// вещественное
     addstr(Adec, 0, "+", 1);
     addstr(Adec, 0, "-", 1);
     addrange(Adec, 0, '0', '9', 1);
     addstr(Adec, 1, ".", 2);
     addstr(Adec, 1, "e", 3);
     addrange(Adec, 1, '0', '9', 1);
     addstr(Adec, 2, "e", 3);
     addstr(Adec, 2, "E", 3);
     addrange(Adec, 2, '0', '9', 2);
     addstr(Adec, 3, "+", 4);
     addstr(Adec, 3, "-", 4);
     addrange(Adec, 3, '0', '9', 4);
     addrange(Adec, 4, '0', '9', 4);
     Adec.final(4);
//________________________________________

// идентификатор
     addrange(Aid, 0, 'a', 'z', 1);
     addrange(Aid, 0, 'A', 'Z', 1);
     addstr(Aid, 0, "!", 1);

     addrange(Aid, 1, 'a', 'z', 1);
     addrange(Aid, 1, 'A', 'Z', 1);
     addrange(Aid, 1, '0', '9', 1);
     addstr(Aid, 1, "!-", 1);

     addstr(Aid, 1, "-", 3);
     addrange(Aid, 3, 'a', 'z', 1);
     addrange(Aid, 3, 'A', 'Z', 1);
     addrange(Aid, 3, '0', '9', 1);
     addstr(Aid, 3, "?", 2);
     addstr(Aid, 3, "-", 1);
     addstr(Aid, 3, "!", 4);

     addstr(Aid, 0, "?", 2);
     addstr(Aid, 1, "?", 2);
     addstr(Aid, 2, "?", 2);

     addrange(Aid, 2,'a', 'z', 1);
     addrange(Aid, 2,'A', 'Z', 1);
     addrange(Aid, 2,'0', '9', 1);
     addstr(Aid, 2,"!", 1);
     addstr(Aid, 2,"-", 3);
     Aid.final(1);
//________________________________________

// идентификатор предиката
     addrange(Aidq, 0, 'a', 'z', 2);
     addrange(Aidq, 0, 'A', 'Z', 2);
     addstr(Aidq, 0, "!", 2);

     addstr(Aidq, 0, "-", 3);
     addstr(Aidq, 3, "?", 4);
     addstr(Aidq, 4, "-", 5);

     addrange(Aidq, 3, 'A', 'Z', 2);
     addrange(Aidq, 3, 'a', 'z', 2);
     addrange(Aidq, 3, '0', '9', 2);
     addstr(Aidq, 3, "!-", 2);

     addrange(Aidq, 4, 'A', 'Z', 2);
     addrange(Aidq, 4, 'a', 'z', 2);
     addrange(Aidq, 4, '0', '9', 2);
     addstr(Aidq, 4, "!", 2);
     addstr(Aidq, 4, "?", 1);

     addrange(Aidq, 2, 'A', 'Z', 2);
     addrange(Aidq, 2, 'a', 'z', 2);
     addrange(Aidq, 2, '0', '9', 2);
     addstr(Aidq, 2, "!", 2);
     addstr(Aidq, 2, "-", 3);

     addstr(Aidq, 0, "?", 1);
     addstr(Aidq, 2, "?", 1);

     addstr(Aidq, 1, "?", 1);
     
     addrange(Aidq, 1, 'a', 'z', 2);
     addrange(Aidq, 1, 'A', 'Z', 2);
     addrange(Aidq, 1, '0', '9', 2);
     addstr(Aidq, 1, "!", 2);
     addstr(Aidq, 1, "-", 3);
     Aidq.final(1);
}
};
#endif

