|=-=|$|{|}|if|(|)|then|else|while|ID|<|<=|>|>=|==|+|-|*|/|;|=|NUM|E|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|program|program -> compoundstmt ||||||||||||||||||||||
|compoundstmt|compoundstmt -> { stmts } ||||||||||||||||||||||
|stmt|stmt -> compoundstmt ||stmt -> ifstmt |||||stmt -> whilestmt |stmt -> assgstmt ||||||||||||||
|ifstmt|||ifstmt -> if ( boolexpr ) then stmt else stmt ||||||||||||||||||||
|whilestmt||||||||whilestmt -> while ( boolexpr ) stmt |||||||||||||||
|assgstmt|||||||||assgstmt -> ID = arithexpr ; ||||||||||||||
|stmts|stmts -> stmt stmts |stmts -> E |stmts -> stmt stmts |||||stmts -> stmt stmts |stmts -> stmt stmts |||||||||||||stmts -> E |
|boolexpr||||boolexpr -> arithexpr boolop arithexpr |||||boolexpr -> arithexpr boolop arithexpr ||||||||||||boolexpr -> arithexpr boolop arithexpr ||
|arithexpr||||arithexpr -> multexpr arithexprprime |||||arithexpr -> multexpr arithexprprime ||||||||||||arithexpr -> multexpr arithexprprime ||
|boolop||||||||||boolop -> < |boolop -> <= |boolop -> > |boolop -> >= |boolop -> == |||||||||
|arithexprprime|||||arithexprprime -> E |||||arithexprprime -> E |arithexprprime -> E |arithexprprime -> E |arithexprprime -> E |arithexprprime -> E |arithexprprime -> + multexpr arithexprprime |arithexprprime -> - multexpr arithexprprime |||arithexprprime -> E |||arithexprprime -> E |
|multexpr||||multexpr -> simpleexpr multexprprime |||||multexpr -> simpleexpr multexprprime ||||||||||||multexpr -> simpleexpr multexprprime ||
|simpleexpr||||simpleexpr -> ( arithexpr ) |||||simpleexpr -> ID ||||||||||||simpleexpr -> NUM ||
|multexprprime|||||multexprprime -> E |||||multexprprime -> E |multexprprime -> E |multexprprime -> E |multexprprime -> E |multexprprime -> E |multexprprime -> E |multexprprime -> E |multexprprime -> * simpleexpr multexprprime |multexprprime -> / simpleexpr multexprprime |multexprprime -> E |||multexprprime -> E |