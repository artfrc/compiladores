#include <bits/stdc++.h>
using namespace std;

class ReturnToken {
public:
    string type;
    string attributeValue;
    int line;
    int column;

    ReturnToken(string t, string aV, int l, int c) :
        type(t), attributeValue(aV), line(l), column(c) {}
};

ostream& operator<<(ostream& os, const ReturnToken& t);
bool LetterUnderlineOrDigit(char c); 
void lexer();
optional<ReturnToken> diagramVoidWithIdentifier();
optional<ReturnToken> diagramMainWithIdentifier();

enum TokenType {
  TOKEN_ID,
  TOKEN_NUMBER,
  TOKEN_OPERATOR,
  TOKEN_PUNCTUATION,
  TOKEN_KEYWORD,
  TOKEN_EOF,
  TOKEN_ERROR,
  TOKEN_RELOP,
};

enum class Relop {
    GT,
    LT,
    LE,
    EQ,
    NE,
    GE
};
 
enum class Operator {
    SUM,
    SUB,
    MULT,
    DIV,
    EXP,
    PARESQ,
    PARDIR
};
 
enum class Type {
    CHAR,
    INT,
    FLOAT
};
 
enum class Punctuation {
    ASSIGMENT,
    MUL_VARS,
    END_EXP,
    DECLARATION,
    APOSTROPHE,
    DOT
};
 
enum class Keyword {
    IF,
    THEN,
    ELSE,
    WHILE,
    DO,
    MAIN,
    BEGIN,
    END,
    REPEAT,
    UNTIL,
    VOID
};
 
enum class ErrorKind {
    UNDEFINED = -1,
    UNCLOSED_COMMENT,
    INVALID_TOKEN_AFTER_EXCLAMATION,
    FRACTION_ENDED_WITH_A_DOT,
    ENDED_WITH_E_EXPOENT,
    ENDED_AFTER_EXPOENT_SIGN,
    UNKNOWN_TOKEN
};

 