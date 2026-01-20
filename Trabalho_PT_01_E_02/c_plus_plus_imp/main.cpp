#include <bits/stdc++.h>
using namespace std;

#define NAME_FILE "file.txt"

string buffer, lexeme = "";
int startLexeme = 0, nextPosLexeme = 0;
int fileLine = 1, fileColumn = 1;

class Token {
public:
    
    string name;
    string lexeme;
    string type;
    int startLine;
    int startColumn;
    int posSymbolTable;

    Token() {}

    
    Token(string n, string l, int sl, int sc) : 
        name(n), 
        lexeme(l), 
        startLine(sl), 
        startColumn(sc) {}

    // ID
    Token(string n, string l, string t, int sl, int sc, int pst) : 
        name(n), 
        lexeme(l), 
        type(t), 
        startLine(sl), 
        startColumn(sc), 
        posSymbolTable(pst) {}

    // KEYWORD
    Token(string n, string l, int pst) : 
        name(n),
        lexeme(l),
        type(""),
        startLine(-1),
        startColumn(-1),
        posSymbolTable(pst) {}

    bool operator==(const Token& other) const {
        return lexeme == other.lexeme;
    }

};

optional<Token> token;
vector<Token> tokensList;
unordered_map<string, Token> symbolTable;

string getContentFile() {
            
    try {
        ifstream file(NAME_FILE);

        if (!file.is_open()) {
            throw runtime_error("Nao foi possivel abrir o arquivo: "  NAME_FILE);
        }

        stringstream buffer;
        buffer << file.rdbuf();

        return buffer.str();
    }
    catch (const exception& e) {
        cerr << "Erro: " << e.what() << endl;
        throw;
    }
}

bool insertSymbolInTable(Token token) {

    
    if(symbolTable.find(token.lexeme) == symbolTable.end()) { // nao achou na tabela, entao insere
        symbolTable[token.lexeme] = token;
        return true;
    } else { // achou, entao nao insere, so retorna 'dizendo" que ja exixte
        return false;
    }
}

string printOrDash(const string& s) {
    return s.empty() ? "-" : s;
}

void printSymbolTable() {

    cout << "\n============================== TABELA DE SIMBOLOS ==============================\n";
    cout << left
         << setw(5)  << "POS"
         << setw(15) << "LEXEMA"
         << setw(15) << "NOME"
         << setw(15) << "TIPO"
         << setw(10) << "LINHA"
         << setw(10) << "COLUNA"
         << endl;

    cout << string(70, '-') << endl;

    for(const auto& [lex, tok] : symbolTable) {
        cout << left
             << setw(5)  << tok.posSymbolTable
             << setw(15) << printOrDash(tok.lexeme)
             << setw(15) << printOrDash(tok.name)
             << setw(15) << printOrDash(tok.type)
             << setw(10) << tok.startLine
             << setw(10) << tok.startColumn
             << endl;
    }

    cout << "================================================================================\n";
}

char getNext() {
    if(nextPosLexeme >= buffer.length()) {
        return '\0';
    }

    while(true) {
        char next = buffer[nextPosLexeme];

        // ==== IGNORAR COMENTÁRIOS % ... % ===
        if(next == '%') {
            nextPosLexeme++; // pula o primeiro %

            while(nextPosLexeme < buffer.length()) {
                char c = buffer[nextPosLexeme];

                if(c == '%') { // fim do comentário
                    nextPosLexeme++;
                    break;
                }

                // controle de linha/coluna
                if(c == '\n') {
                    fileLine++;
                    fileColumn = 1;
                } else {
                    fileColumn++;
                }

                nextPosLexeme++;
            }
            continue; // volta a procurar token
        }

        if(isspace(next)) {
            if(next == '\n') {
                if(lexeme != "") {
                    return '\0';
                } else {
                    nextPosLexeme++;
                    fileLine++;
                    fileColumn = 1;
                }
            } 
            else if(next == ' ') {
                if(lexeme != "") {
                    nextPosLexeme++;
                    return next;
                } else {
                    nextPosLexeme++;
                }
            } 
            else {
                nextPosLexeme++;
            }
        } 
        else {
            if(lexeme == "") {
                startLexeme = nextPosLexeme;
                fileColumn = startLexeme + 1;
            }
            nextPosLexeme++;
            return next;
        }   
    }
}


char getNextForLookahead() {
    if(nextPosLexeme >= buffer.length()) {
        return '\0';
    }
    return buffer[nextPosLexeme];
}

optional<Token> getIntLexeme() {

    lexeme = "";

    int state = 1;
    char c;

    while(true) {
        switch (state) {
        case 1:
            c = getNext();
            if(c == 'i'){
                lexeme += c;
                state = 2;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'n'){
                lexeme += c;
                state = 3;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 3:
            c = getNext();
            if(c == 't'){
                lexeme += c;
                state = 4;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 4:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                return nullopt;
            } else { // é INT mesmo
                Token token("INT", "INT", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                return token;

            }
        
        default:
            break;
        }
    }
}

optional<Token> getCharLexeme() {

    lexeme = "";

    int state = 1;
    char c;

    while(true) {
        switch (state) {
        case 1:
            c = getNext();
            if(c == 'c'){
                lexeme += c;
                state = 2;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'h'){
                lexeme += c;
                state = 3;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 3:
            c = getNext();
            if(c == 'a'){
                lexeme += c;
                state = 4;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 4:
            c = getNext();
            if(c == 'r'){
                lexeme += c;
                state = 5;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 5:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                return nullopt;
            } else { // é CHAR mesmo
                Token token("CHAR", "CHAR", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                return token;

            }
        
        default:
            break;
        }
    }
}

optional<Token> getFloatLexeme() {

    lexeme = "";

    int state = 1;
    char c;

    while(true) {
        switch (state) {
        case 1:
            c = getNext();
            if(c == 'f'){
                lexeme += c;
                state = 2;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'l'){
                lexeme += c;
                state = 3;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 3:
            c = getNext();
            if(c == 'o'){
                lexeme += c;
                state = 4;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 4:
            c = getNext();
            if(c == 'a'){
                lexeme += c;
                state = 5;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 5:
            c = getNext();
            if(c == 't'){
                lexeme += c;
                state = 6;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 6:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                return nullopt;
            } else { // é FLOAT mesmo
                Token token("FLOAT", "FLOAT", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                return token;

            }
        
        default:
            break;
        }
    }
}

optional<Token> getVoidLexeme() {

    lexeme = "";

    int state = 1;
    char c;

    while(true) {
        switch (state) {
        case 1:
            c = getNext();
            if(c == 'v'){
                lexeme += c;
                state = 2;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'o'){
                lexeme += c;
                state = 3;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 3:
            c = getNext();
            if(c == 'i'){
                lexeme += c;
                state = 4;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 4:
            c = getNext();
            if(c == 'd'){
                lexeme += c;
                state = 5;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 5:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                return nullopt;
            } else { // é VOID mesmo
                Token token("VOID", "VOID", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                return token;

            }
        
        default:
            break;
        }
    }
}

optional<Token> getMainLexeme() {

    lexeme = "";

    int state = 1;
    char c;

    while(true) {
        switch (state) {
        case 1:
            c = getNext();
            if(c == 'm'){
                lexeme += c;
                state = 2;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'a'){
                lexeme += c;
                state = 3;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 3:
            c = getNext();
            if(c == 'i'){
                lexeme += c;
                state = 4;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 4:
            c = getNext();
            if(c == 'n'){
                lexeme += c;
                state = 5;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 5:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                return nullopt;
            } else { // é VOID mesmo
                Token token("MAIN", "MAIN", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                return token;

            }
        
        default:
            break;
        }
    }
}


optional<Token> getIfLexeme() {

    lexeme = "";

    int state = 1;
    char c;

    while(true) {
        switch (state) {
        case 1:
            c = getNext();
            if(c == 'i'){
                lexeme += c;
                state = 2;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'f'){
                lexeme += c;
                state = 3;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 3:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                return nullopt;
            } else { // é IF mesmo
                Token token("IF", "IF", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                return token;

            }
        
        default:
            break;
        }
    }
}

optional<Token> getThenLexeme() {

    lexeme = "";

    int state = 1;
    char c;

    while(true) {
        switch (state) {
        case 1:
            c = getNext();
            if(c == 't'){
                lexeme += c;
                state = 2;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'h'){
                lexeme += c;
                state = 3;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 3:
            c = getNext();
            if(c == 'e'){
                lexeme += c;
                state = 4;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 4:
            c = getNext();
            if(c == 'n'){
                lexeme += c;
                state = 5;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 5:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                return nullopt;
            } else { // é THEN mesmo
                Token token("THEN", "THEN", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                return token;

            }
        
        default:
            break;
        }
    }
}

optional<Token> getElsifLexeme() {

    lexeme = "";

    int state = 1;
    char c;

    while(true) {
        switch (state) {
        case 1:
            c = getNext();
            if(c == 'e'){
                lexeme += c;
                state = 2;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'l'){
                lexeme += c;
                state = 3;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 3:
            c = getNext();
            if(c == 's'){
                lexeme += c;
                state = 4;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 4:
            c = getNext();
            if(c == 'i'){
                lexeme += c;
                state = 5;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 5:
            c = getNext();
            if(c == 'f'){
                lexeme += c;
                state = 6;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 6:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                return nullopt;
            } else { // é ELSIF mesmo
                Token token("ELSIF", "ELSIF", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                return token;

            }
        
        default:
            break;
        }
    }
}

optional<Token> getElseLexeme() {

    lexeme = "";

    int state = 1;
    char c;

    while(true) {
        switch (state) {
        case 1:
            c = getNext();
            if(c == 'e'){
                lexeme += c;
                state = 2;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'l'){
                lexeme += c;
                state = 3;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 3:
            c = getNext();
            if(c == 's'){
                lexeme += c;
                state = 4;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 4:
            c = getNext();
            if(c == 'e'){
                lexeme += c;
                state = 5;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 5:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                return nullopt;
            } else { // é ELSE mesmo
                Token token("ELSE", "ELSE", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                return token;

            }
        
        default:
            break;
        }
    }
}

optional<Token> getForLexeme() {

    lexeme = "";

    int state = 1;
    char c;

    while(true) {
        switch (state) {
        case 1:
            c = getNext();
            if(c == 'f'){
                lexeme += c;
                state = 2;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'o'){
                lexeme += c;
                state = 3;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 3:
            c = getNext();
            if(c == 'r'){
                lexeme += c;
                state = 4;
            } else {
                nextPosLexeme = startLexeme;
                lexeme = "";
                return nullopt;
            }
            break;
        case 4:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                return nullopt;
            } else { // é FOR mesmo
                Token token("FOR", "FOR", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                return token;

            }
        
        default:
            break;
        }
    }
}

optional<Token> getIdLexeme() {

    lexeme = "";
    int state = 1;
    char c;

    while(true) {
        switch (state) {
        case 1:
            c = getNext();
            if(isalpha(c)){
                lexeme += toupper(c);
                state = 2;
            } else {
                nextPosLexeme = startLexeme;
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if((isalpha(c) or isdigit(c))){
                lexeme += toupper(c);
                state = 2;
            } else {
                state = 3;
            }
            break;
        case 3: {
            Token token("ID", lexeme, "ID", fileLine, startLexeme, symbolTable.size());
            tokensList.push_back(token);
            if(insertSymbolInTable(token)) {
                return token;
            }
            break;
        }

        default:
            break;
        }
    }
}

void lexer() {

    while(nextPosLexeme < buffer.size()) {
        token = getIfLexeme();
        if(token == nullopt) {
             token = getVoidLexeme();
             if(token == nullopt) {
                  token = getIntLexeme();
              } if(token == nullopt) {
                  token = getCharLexeme();
              } if(token == nullopt) {
                 token = getFloatLexeme();
              } if(token == nullopt) {
                 token = getMainLexeme();
              } if(token == nullopt) {
                token = getThenLexeme();
             } if(token == nullopt) {
                token = getElsifLexeme();
             } if(token == nullopt) {
                token = getElseLexeme();
             } if(token == nullopt) {
                token = getForLexeme();
             } if(token == nullopt) {
                token = getIdLexeme();
             } 
        }         
    }

}

int main() {

    buffer = getContentFile();
    lexer();
    printSymbolTable();
    cout << "\nTOKENS:" << endl;

    for(Token t : tokensList) {
        cout << t.lexeme << endl;
    }

}