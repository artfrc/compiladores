#include <bits/stdc++.h>
#include "main.h"
using namespace std;

#define MAX_CHAR_READ_CODE 2

string code, buffer = "";
int ini = 0, prox = 0, line = 1, column = 1;

string PassFileToString() {
    ifstream file("file.txt");
    if (!file) throw runtime_error("Error to open file");
    
    string fileInString(
        (istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>()
    );
    return fileInString;
}

string getToBuffer() {
    return code.substr(ini, MAX_CHAR_READ_CODE);
}

bool NotLetterUnderlineOrDigit(char c) {
    return (!isalpha(c) || !isdigit(c) || c != '_');
}

char getNextChar() {
    char c;
 
        c = buffer[ini++];
 
        if (c == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
  
    return c;
}

optional<ReturnToken> diagramVoidWithIdentifier() {
    int state = 0;
    string lexeme = "";
    char c;

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
                } else if (NotLetterUnderlineOrDigit(c)) {

                    
                } else return nullopt;
                break;

            case 4:
                c = getNextChar();
                if (isalpha(c) || isdigit(c)) {
                    lexeme += c;
                    
                } else {
                }
                break;
        }
    }
}

optional<ReturnToken> diagramIdentifierAndKeyword() {

    int state = 0;
    string lexeme = "";
    char c;

    while (true) {
        
        switch (state) {
            case 0:
                c = getNextChar();
                if (c == 'm') {
                    lexeme += c;
                    state = 1;
                } else if (NotLetterUnderlineOrDigit(c)) {
                    state = 11;
                } else return nullopt;
                break;

            case 1:
                c = getNextChar();
                if (c == 'a') {
                    lexeme += c;
                    state = 2;
                } else if (NotLetterUnderlineOrDigit(c)) {
                    state = 11;
                } else return nullopt;
                break;

            case 2:
                c = getNextChar();
                if (c == 'i') {
                    lexeme += c;
                    state = 3;
                }else if (NotLetterUnderlineOrDigit(c)) {
                    state = 11;
                } else return nullopt;
                break;

            case 3:
                c = getNextChar();
                if (c == 'n') {
                    lexeme += c;
                    state = 4;
                } else if (NotLetterUnderlineOrDigit(c)) {
                    state = 11; // diagrama ID
                }  else return nullopt;
                break;

            case 4: // d5
                c = getNextChar();
                if (NotLetterUnderlineOrDigit(c)) {
                    state = 5; 
                    
                } else {
                    lexeme += c;
                    state = 10; // diagrama 
                }
                break;
            
                case 5:
        }
    }
}

void lexer() {

    while(ini < buffer.length()) {
        buffer = getToBuffer();
        optional<ReturnToken> token = diagramIdentifierAndKeyword();
        if (token.has_value()) {
            cout << *token << endl;
        }

    }

}

int main() {
    try {
        code = PassFileToString();
        lexer();
    } catch (const exception& e) {
        cout << e.what() << endl;
    }

}
