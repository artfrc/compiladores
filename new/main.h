#include <bits/stdc++.h>
using namespace std;

class ReturnToken {
public:
    string type;
    string attributeValue;
    int line;
    int column;

    ReturnToken(string t, string aV, int l, int c)
        : type(t), attributeValue(aV), line(l), column(c) {}
};

struct Candidate {
    optional<ReturnToken> token;
    int lexemeSize;
    int finalPos;
    int finalLine;
    int finalColumn;
};

struct LexerState {
    int pos;
    int line;
    int col;
};

ostream& operator<<(ostream& os, const ReturnToken& t);
string PassFileToString();
void lexer();

optional<ReturnToken> nextToken();
optional<ReturnToken> diagramMainWithIdentifier();
optional<ReturnToken> diagramIntWithIdentifier();
optional<ReturnToken> diagramCharWithIdentifier();
optional<ReturnToken> diagramFloatWithIdentifier();
