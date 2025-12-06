#define TRUE 1 
#define FALSE 0 

#define TOK_NUM 0 
#define TOK_OP 1 
#define TOK_PONT 2 
#define TOK_ERRO 3
#define TOK_ID 4
#define TOK_RELOP 5
#define TOK_NUM_FLOAT 6
#define TOK_IF 7
#define TOK_WHILE 8
#define TOK_THEN 9
#define TOK_ELSE 10
#define TOK_REPEAT 11
#define TOK_UNTIL 12
#define TOK_EOF 13

#define SOMA 0 
#define SUB 1 
#define MULT 2 
#define DIV 3

#define PARESQ 0 
#define PARDIR 1 

typedef struct { 
   int tipo; 
   int atributo; 
} Token; 

extern Token *token(); 
extern Token *yylex();