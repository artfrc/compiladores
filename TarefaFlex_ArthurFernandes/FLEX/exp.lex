%option noyywrap
%option nodefault
%option outfile="lexer.c"
%option header-file="lexer.h"

%{
#include "exp.h"
#include <stdlib.h>
#include <string.h>

Token *token(int tipo, int valor);
int adiciona_lexema(const char *lex);
%}


NUM     [0-9]+
DIGITO  [0-9]
LETRA   [a-zA-Z]

FLOAT   {NUM}"."{NUM}
ID      {LETRA}({LETRA}|{DIGITO}|_)*
RELOP       (<=|>=|==|!=|<|>|=)

%%

[[:space:]]    { }

"if"           { return token(TOK_IF, 0); }
"while"        { return token(TOK_WHILE, 0); }
"then"         { return token(TOK_THEN, 0); }
"else"         { return token(TOK_ELSE, 0); }
"repeat"       { return token(TOK_REPEAT, 0); }
"until"        { return token(TOK_UNTIL, 0); }

{FLOAT}        { return token(TOK_NUM_FLOAT, adiciona_lexema(yytext)); }

{NUM}          { return token(TOK_NUM, atoi(yytext)); }

\+             { return token(TOK_OP, SOMA); }
-              { return token(TOK_OP, SUB); }
\*             { return token(TOK_OP, MULT); }
\/             { return token(TOK_OP, DIV); }


{ID}           { return token(TOK_ID, adiciona_lexema(yytext)); }
{RELOP}        { return token(TOK_RELOP, adiciona_lexema(yytext)); }


\(             { return token(TOK_PONT, PARESQ); }
\)             { return token(TOK_PONT, PARDIR); }

<<EOF>>        { return token(TOK_EOF, 0); }

.              { return token(TOK_ERRO, 0); }

%%

Token tok;

Token *token(int tipo, int valor) {
    tok.tipo = tipo;
    tok.atributo = valor;
    return &tok;
}