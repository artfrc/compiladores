#include <bits/stdc++.h>
#include "main.h"
using namespace std;

string fileInStringGlobal;
optional<ReturnToken> token;

int currentPosFileGlobal = 0;
int lineFileGlobal = 1;
int columnFileGlobal = 1;

int initLexemeLine;
int initLexemeColumn;

ostream& operator<<(ostream& os, const ReturnToken& t) {
    os << left
       << setw(10) << t.type
       << setw(15) << t.attributeValue
       << setw(6)  << t.line
       << setw(6)  << t.column;
    return os;
}

static inline LexerState saveState() {
    return { currentPosFileGlobal, lineFileGlobal, columnFileGlobal };
}

static inline void restoreState(const LexerState& st) {
    currentPosFileGlobal = st.pos;
    lineFileGlobal = st.line;
    columnFileGlobal = st.col;
}

string PassFileToString() {
    ifstream file("file.txt");
    if (!file) throw runtime_error("Error to open file");

    return string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
}

void skipWhitespace() {
    while (currentPosFileGlobal < (int)fileInStringGlobal.size()) {

        char c = fileInStringGlobal[currentPosFileGlobal];

        // whitespace normal
        if (isspace((unsigned char)c)) {
            if (c == '\n') {
                lineFileGlobal++;
                columnFileGlobal = 1;
            } else {
                columnFileGlobal++;
            }
            currentPosFileGlobal++;
            continue;
        }

        // comentário de bloco /* ... */
        if (c == '/' &&
            currentPosFileGlobal + 1 < (int)fileInStringGlobal.size() &&
            fileInStringGlobal[currentPosFileGlobal + 1] == '*') {

            skipBlockComment();
            continue;
        }

        // não é whitespace nem comentário
        break;
    }
}

char getNextChar() {
    if (currentPosFileGlobal >= (int)fileInStringGlobal.size()) return '\0';

    char c = fileInStringGlobal[currentPosFileGlobal];

    if (c == '\n') {
        lineFileGlobal++;
        columnFileGlobal = 1;
    } else {
        columnFileGlobal++;
    }

    currentPosFileGlobal++;
    return c;
}

optional<ReturnToken> diagramIntWithIdentifier() {
    int state = 0;
    int countLookahead = 0;
    string lexeme;
    char c;

    LexerState start = saveState();
    initLexemeLine = lineFileGlobal;
    initLexemeColumn = columnFileGlobal;

    while (true) {
        switch (state) {
            case 0:
                c = getNextChar();
                if (c == 'i') { lexeme += c; state = 1; }
                else { restoreState(start); return nullopt; }
                break;

            case 1:
                c = getNextChar();
                if (c == 'n') { lexeme += c; state = 2; }
                else { restoreState(start); return nullopt; }
                break;

            case 2:
                c = getNextChar();
                if (c == 't') { lexeme += c; state = 3; }
                else { restoreState(start); return nullopt; }
                break;

            case 3:
                c = getNextChar();
                if (isalpha((unsigned char)c) || isdigit((unsigned char)c)) {
                    lexeme += c;
                    countLookahead++;
                } else {
                    if (countLookahead == 0)
                        return ReturnToken("KEYWORD", "int", initLexemeLine, initLexemeColumn);
                    else
                        return ReturnToken("ID", lexeme, initLexemeLine, initLexemeColumn);
                }
                break;
        }
    }
}

optional<ReturnToken> diagramMainWithIdentifier() {
    int state = 0;
    int countLookahead = 0;
    string lexeme;
    char c;

    LexerState start = saveState();
    initLexemeLine = lineFileGlobal;
    initLexemeColumn = columnFileGlobal;

    while (true) {
        switch (state) {
            case 0:
                c = getNextChar();
                if (c == 'm') { lexeme += c; state = 1; }
                else { restoreState(start); return nullopt; }
                break;

            case 1:
                c = getNextChar();
                if (c == 'a') { lexeme += c; state = 2; }
                else { restoreState(start); return nullopt; }
                break;

            case 2:
                c = getNextChar();
                if (c == 'i') { lexeme += c; state = 3; }
                else { restoreState(start); return nullopt; }
                break;

            case 3:
                c = getNextChar();
                if (c == 'n') { lexeme += c; state = 4; }
                else { restoreState(start); return nullopt; }
                break;

            case 4:
                c = getNextChar();
                if (isalpha((unsigned char)c) || isdigit((unsigned char)c)) {
                    lexeme += c;
                    countLookahead++;
                } else {
                    if (countLookahead == 0)
                        return ReturnToken("KEYWORD", "main", initLexemeLine, initLexemeColumn);
                    else
                        return ReturnToken("ID", lexeme, initLexemeLine, initLexemeColumn);
                }
                break;
        }
    }
}

optional<ReturnToken> diagramCharWithIdentifier() {
    int state = 0;
    int countLookahead = 0;
    string lexeme;
    char c;

    LexerState start = saveState();
    initLexemeLine = lineFileGlobal;
    initLexemeColumn = columnFileGlobal;

    while (true) {
        switch (state) {
            case 0:
                c = getNextChar();
                if (c == 'c') { lexeme += c; state = 1; }
                else { restoreState(start); return nullopt; }
                break;

            case 1:
                c = getNextChar();
                if (c == 'h') { lexeme += c; state = 2; }
                else { restoreState(start); return nullopt; }
                break;

            case 2:
                c = getNextChar();
                if (c == 'a') { lexeme += c; state = 3; }
                else { restoreState(start); return nullopt; }
                break;

            case 3:
                c = getNextChar();
                if (c == 'r') { lexeme += c; state = 4; }
                else { restoreState(start); return nullopt; }
                break;

            case 4:
                c = getNextChar();
                if (isalpha((unsigned char)c) || isdigit((unsigned char)c)) {
                    lexeme += c;
                    countLookahead++;
                } else {
                    if (countLookahead == 0)
                        return ReturnToken("KEYWORD", "char", initLexemeLine, initLexemeColumn);
                    else
                        return ReturnToken("ID", lexeme, initLexemeLine, initLexemeColumn);
                }
                break;
        }
    }
}

optional<ReturnToken> diagramFloatWithIdentifier() {
    int state = 0;
    int countLookahead = 0;
    string lexeme;
    char c;

    LexerState start = saveState();
    initLexemeLine = lineFileGlobal;
    initLexemeColumn = columnFileGlobal;

    while (true) {
        switch (state) {
            case 0:
                c = getNextChar();
                if (c == 'f') { lexeme += c; state = 1; }
                else { restoreState(start); return nullopt; }
                break;

            case 1:
                c = getNextChar();
                if (c == 'l') { lexeme += c; state = 2; }
                else { restoreState(start); return nullopt; }
                break;

            case 2:
                c = getNextChar();
                if (c == 'o') { lexeme += c; state = 3; }
                else { restoreState(start); return nullopt; }
                break;

            case 3:
                c = getNextChar();
                if (c == 'a') { lexeme += c; state = 4; }
                else { restoreState(start); return nullopt; }
                break;

            case 4:
                c = getNextChar();
                if (c == 't') { lexeme += c; state = 5; }
                else { restoreState(start); return nullopt; }
                break;

            case 5:
                c = getNextChar();
                if (isalpha((unsigned char)c) || isdigit((unsigned char)c)) {
                    lexeme += c;
                    countLookahead++;
                } else {
                    if (countLookahead == 0)
                        return ReturnToken("KEYWORD", "float", initLexemeLine, initLexemeColumn);
                    else
                        return ReturnToken("ID", lexeme, initLexemeLine, initLexemeColumn);
                }
                break;
        }
    }
}

optional<ReturnToken> nextToken() {
    skipWhitespace();

    if (currentPosFileGlobal >= (int)fileInStringGlobal.size())
        return nullopt;

    LexerState start = saveState();
    vector<Candidate> candidates;

    auto tryDiagram = [&](auto&& fn) {
        restoreState(start);
        auto t = fn();
        if (t.has_value()) {
            LexerState end = saveState();
            candidates.push_back(Candidate{
                t,
                end.pos - start.pos,
                end.pos,
                end.line,
                end.col
            });
        }
        // se falhou, o próprio diagrama restaura (ou a gente restaura no começo do próximo)
    };

    tryDiagram(diagramIntWithIdentifier);
    tryDiagram(diagramMainWithIdentifier);
    tryDiagram(diagramCharWithIdentifier);
    tryDiagram(diagramFloatWithIdentifier);

    if (candidates.empty()) {
        // Consome 1 char “desconhecido” pra não travar
        restoreState(start);
        getNextChar();
        return nullopt;
    }

    auto best = max_element(
        candidates.begin(),
        candidates.end(),
        [](const Candidate& a, const Candidate& b) {
            return a.lexemeSize < b.lexemeSize;
        }
    );

    currentPosFileGlobal = best->finalPos;
    lineFileGlobal = best->finalLine;
    columnFileGlobal = best->finalColumn;

    return best->token;
}

bool skipBlockComment() {
    // garante que é realmente /*
    if (currentPosFileGlobal + 1 >= (int)fileInStringGlobal.size())
        return false;

    if (fileInStringGlobal[currentPosFileGlobal] != '/' ||
        fileInStringGlobal[currentPosFileGlobal + 1] != '*')
        return false;

    // consome /*
    getNextChar(); // '/'
    getNextChar(); // '*'

    while (currentPosFileGlobal < (int)fileInStringGlobal.size()) {
        char c = getNextChar();

        // encontrou */
        if (c == '*' &&
            currentPosFileGlobal < (int)fileInStringGlobal.size() &&
            fileInStringGlobal[currentPosFileGlobal] == '/') {

            getNextChar(); // consome '/'
            return true;
        }
    }

    // chegou EOF sem fechar comentário
    throw runtime_error("Unterminated block comment");
}

void lexer() {
    cout << left
         << setw(10) << "TYPE"
         << setw(15) << "LEXEME"
         << setw(6)  << "LINE"
         << setw(6)  << "COL"
         << "\n";

    cout << string(37, '-') << "\n";

    while (currentPosFileGlobal < (int)fileInStringGlobal.size()) {
        token = nextToken();
        if (token.has_value()) {
            cout << *token << "\n";
        }
    }
}

int main() {
    try {
        fileInStringGlobal = PassFileToString();
        lexer();
    } catch (const exception& e) {
        cout << e.what() << "\n";
    }
}