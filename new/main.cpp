#include <bits/stdc++.h>
using namespace std;

string fileInStringGlobal, buffer;
int currentPosFileGlobal = 0, lineFileGlobal = 1, columnFileGlobal = 1;
int initLexemeLine, initLexemeColumn;

class Token {
   string name;
   string lexeme;
   string type;
};

class ReturnToken {
public:
    string type;
    string attributeValue;
    int line;
    int column;

    ReturnToken(string t, string aV, int l, int c) :
        type(t), attributeValue(aV), line(l), column(c) {}
};

/* ===================
| FUNCOES DO ANALISADOR
|  ==================== */

ostream& operator<<(ostream& os, const ReturnToken& t);
string PassFileToString();
void lexer();
optional<ReturnToken> diagramVoidWithIdentifier();

/* ===================
| FUNCOES DO ANALISADOR
|  ==================== */

ostream& operator<<(ostream& os, const ReturnToken& t) {
    os << left
       << setw(10) << t.type
       << setw(15) << t.attributeValue
       << setw(6)  << t.line
       << setw(6)  << t.column;
    return os;
}


string PassFileToString() {
    ifstream file("file.txt");
    if (!file) throw runtime_error("Error to open file");
    
    string fileInString(
        (istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>()
    );
    return fileInString;
}

char getNextChar() {
    char c = fileInStringGlobal[currentPosFileGlobal];
    while(isspace(c)) {
        if(c == '\n') {
           lineFileGlobal++;
           columnFileGlobal = 1;
        }
         currentPosFileGlobal++;
         columnFileGlobal++;
         getNextChar();
    }
    currentPosFileGlobal++;
    columnFileGlobal++;
    return c;
}

optional<ReturnToken> diagramVoidWithIdentifier() {
    int state = 0, countLookahead = 0;
    string lexeme = "";
    char c;
    initLexemeLine = lineFileGlobal;
    initLexemeColumn = columnFileGlobal;

    while (true) {
        
        switch (state) {
            case 0:
                c = getNextChar();
                if (c == 'v') {
                    lexeme += c;
                    state = 1;
                } else return nullopt;
                break;

            case 1:
                c = getNextChar();
                if (c == 'o') {
                    lexeme += c;
                    state = 2;
                } else return nullopt;
                break;

            case 2:
                c = getNextChar();
                if (c == 'i') {
                    lexeme += c;
                    state = 3;
                } else return nullopt;
                break;

            case 3:
                c = getNextChar();
                if (c == 'd') {
                    lexeme += c;
                    state = 4;
                } else return nullopt;
                break;

            case 4:
                c = getNextChar();
                if (isalpha(c) || isdigit(c)) {
                    countLookahead++;
                    lexeme += c;
                    
                } else {
                    if(countLookahead == 0) return ReturnToken("VOID", "-", initLexemeLine, initLexemeColumn);
                    else return ReturnToken("ID", lexeme, initLexemeLine, initLexemeColumn);
                }
                break;
        }
    }
}

void lexer() {

    while(currentPosFileGlobal < fileInStringGlobal.length()) {
        optional<ReturnToken> token = diagramVoidWithIdentifier();
        if (token.has_value()) {
            cout << *token << endl;
        }

    }

}

int main () {
    try{
        fileInStringGlobal = PassFileToString();
    } catch(const exception e) {
        cout << e.what() << endl;
    }

    lexer();

}
