#include <bits/stdc++.h>
using namespace std;

#define NAME_FILE "file.txt"

string buffer, lexeme = "";
int startLexeme = 0, nextPosLexeme = 0;
int fileLine = 1, fileColumn = 1;
int tokenStartLine = 1, tokenStartColumn = 1;

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

void FailedLexeme() {

    nextPosLexeme = startLexeme;
    fileColumn = tokenStartColumn;
    lexeme = "";

}

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

        if(next == '%') {
            nextPosLexeme++;
            fileColumn++;

            while(nextPosLexeme < buffer.length()) {
                char c = buffer[nextPosLexeme];

                if(c == '%') {
                    nextPosLexeme++;
                    fileColumn++;
                    break;
                }

                if(c == '\n') {
                    fileLine++;
                    fileColumn = 1;
                } else {
                    fileColumn++;
                }

                nextPosLexeme++;
            }
            continue;
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
                    return '\0';
                } else {
                    nextPosLexeme++;
                    fileColumn++;
                }
            } 
            else {
                nextPosLexeme++;
                if(next == '\t') {
                    fileColumn++;
                } else {
                    fileColumn++;
                }
            }
        } 
        else {
            if(lexeme == "") {
                startLexeme = nextPosLexeme;
                tokenStartLine = fileLine;
                tokenStartColumn = fileColumn;
            }
            lexeme += next;
            nextPosLexeme++;
            fileColumn++;
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
                state = 2;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'n'){
                state = 3;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 3:
            c = getNext();
            if(c == 't'){
                state = 4;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 4:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                fileColumn = tokenStartColumn;
                return nullopt;
            } else { // é INT mesmo
                Token token("INT", "INT", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                lexeme = "";
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
                state = 2;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'h'){
                state = 3;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 3:
            c = getNext();
            if(c == 'a'){
                state = 4;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 4:
            c = getNext();
            if(c == 'r'){
                state = 5;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 5:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                fileColumn = tokenStartColumn;
                return nullopt;
            } else { // é CHAR mesmo
                Token token("CHAR", "CHAR", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                lexeme = "";
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
                state = 2;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'l'){
                state = 3;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 3:
            c = getNext();
            if(c == 'o'){
                state = 4;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 4:
            c = getNext();
            if(c == 'a'){
                state = 5;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 5:
            c = getNext();
            if(c == 't'){
                state = 6;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 6:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                fileColumn = tokenStartColumn;
                return nullopt;
            } else { // é FLOAT mesmo
                Token token("FLOAT", "FLOAT", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                lexeme = "";
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
                state = 2;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'o'){
                state = 3;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 3:
            c = getNext();
            if(c == 'i'){
                state = 4;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 4:
            c = getNext();
            if(c == 'd'){
                state = 5;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 5:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                fileColumn = tokenStartColumn;
                return nullopt;
            } else { // é VOID mesmo
                Token token("VOID", "VOID", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                lexeme = "";
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
                state = 2;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'a'){
                state = 3;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 3:
            c = getNext();
            if(c == 'i'){
                state = 4;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 4:
            c = getNext();
            if(c == 'n'){
                state = 5;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 5:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                fileColumn = tokenStartColumn;
                return nullopt;
            } else { // é VOID mesmo
                Token token("MAIN", "MAIN", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                lexeme = "";
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
                state = 2;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'f'){
                state = 3;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 3:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                fileColumn = tokenStartColumn;
                return nullopt;
            } else { // é IF mesmo
                Token token("IF", "IF", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                lexeme = "";
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
                state = 2;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'h'){
                state = 3;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 3:
            c = getNext();
            if(c == 'e'){
                state = 4;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 4:
            c = getNext();
            if(c == 'n'){
                state = 5;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 5:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                fileColumn = tokenStartColumn;
                return nullopt;
            } else { // é THEN mesmo
                Token token("THEN", "THEN", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                lexeme = "";
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
                state = 2;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'l'){
                state = 3;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 3:
            c = getNext();
            if(c == 's'){
                state = 4;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 4:
            c = getNext();
            if(c == 'i'){
                state = 5;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 5:
            c = getNext();
            if(c == 'f'){
                state = 6;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 6:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                fileColumn = tokenStartColumn;
                return nullopt;
            } else { // é ELSIF mesmo
                Token token("ELSIF", "ELSIF", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                lexeme = "";
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
                state = 2;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'l'){
                state = 3;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 3:
            c = getNext();
            if(c == 's'){
                state = 4;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 4:
            c = getNext();
            if(c == 'e'){
                state = 5;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 5:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                fileColumn = tokenStartColumn;
                return nullopt;
            } else { // é ELSE mesmo
                Token token("ELSE", "ELSE", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                lexeme = "";
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
                state = 2;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == 'o'){
                state = 3;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 3:
            c = getNext();
            if(c == 'r'){
                state = 4;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 4:
            c = getNextForLookahead();
            if(isalpha(c) || isdigit(c) || c == '_') { // é um ID!
                nextPosLexeme = startLexeme;
                fileColumn = tokenStartColumn;
                return nullopt;
            } else { // é FOR mesmo
                Token token("FOR", "FOR", symbolTable.size());
                insertSymbolInTable(token);
                tokensList.push_back(token);
                lexeme = "";
                return token;

            }
        
        default:
            break;
        }
    }
}

optional<Token> getRelopLexeme() {
    lexeme = "";

    int state = 1;
    char c;
    
    // Guarda a posição inicial para possível rollback
    int savedNextPos = nextPosLexeme;
    int savedFileColumn = fileColumn;
    int savedFileLine = fileLine;

    while(true) {
        switch (state) {
            case 1:
                c = getNext();
                if(c == '<') {
                    state = 2;
                } else if(c == '=') {
                    state = 6;
                } else if(c == '>') {
                    state = 7;
                } else if(c == '!') {
                    state = 10; // Novo estado para '!'
                } else {
                    // Rollback
                    nextPosLexeme = startLexeme;
                    fileColumn = tokenStartColumn;
                    fileLine = tokenStartLine;
                    lexeme = "";
                    return nullopt;
                }
                break;
                
            case 2:
                c = getNextForLookahead();
                if(c == '=') {
                    getNext(); // Consome o '='
                    state = 3;
                } else if(c == '>') {
                    getNext(); // Consome o '>'
                    state = 4;
                } else {
                    state = 5;
                }
                break;
                
            case 3: {
                // <=
                string type = "LE";
                Token token("RELOP", lexeme, type, tokenStartLine, tokenStartColumn, symbolTable.size());
                tokensList.push_back(token);
                insertSymbolInTable(token);
                lexeme = "";
                return token;
            }
                
            case 4: {
                // <>
                string type = "NE";
                Token token("RELOP", lexeme, type, tokenStartLine, tokenStartColumn, symbolTable.size());
                tokensList.push_back(token);
                insertSymbolInTable(token);
                lexeme = "";
                return token;
            }
                
            case 5: {
                // < (sozinho)
                string type = "LT";
                Token token("RELOP", lexeme, type, tokenStartLine, tokenStartColumn, symbolTable.size());
                tokensList.push_back(token);
                insertSymbolInTable(token);
                lexeme = "";
                return token;
            }
                
            case 6:
                c = getNextForLookahead();
                if(c == '=') {
                    getNext(); // Consome o segundo '='
                    state = 11; // '=='
                } else {
                    state = 12; // '=' sozinho
                }
                break;
                
            case 7:
                c = getNextForLookahead();
                if(c == '=') {
                    getNext(); // Consome o '='
                    state = 8;
                } else {
                    state = 9;
                }
                break;
                
            case 8: {
                // >=
                string type = "GE";
                Token token("RELOP", lexeme, type, tokenStartLine, tokenStartColumn, symbolTable.size());
                tokensList.push_back(token);
                insertSymbolInTable(token);
                lexeme = "";
                return token;
            }

            case 9: {
                // > (sozinho)
                string type = "GT";
                Token token("RELOP", lexeme, type, tokenStartLine, tokenStartColumn, symbolTable.size());
                tokensList.push_back(token);
                insertSymbolInTable(token);
                lexeme = "";
                return token;
            }
            
            case 10: // Estado para '!'
                c = getNextForLookahead();
                if(c == '=') {
                    getNext(); // Consome o '='
                    state = 13; // '!='
                } else {
                    // '!' sozinho não é um RELOP válido
                    nextPosLexeme = startLexeme;
                    FailedLexeme();
                    return nullopt;
                }
                break;
                
            case 11: { // '=='
                string type = "EE"; // Double Equal (ou "EQ" se preferir manter consistência)
                Token token("RELOP", lexeme, type, tokenStartLine, tokenStartColumn, symbolTable.size());
                tokensList.push_back(token);
                insertSymbolInTable(token);
                lexeme = "";
                return token;
            }
                
            case 12: { // '=' sozinho
                string type = "EQ";
                Token token("RELOP", lexeme, type, tokenStartLine, tokenStartColumn, symbolTable.size());
                tokensList.push_back(token);
                insertSymbolInTable(token);
                lexeme = "";
                return token;
            }
                
            case 13: { // '!='
                string type = "NE";
                Token token("RELOP", lexeme, type, tokenStartLine, tokenStartColumn, symbolTable.size());
                tokensList.push_back(token);
                insertSymbolInTable(token);
                lexeme = "";
                return token;
            }

            default:
                // Rollback em caso de erro
                nextPosLexeme = savedNextPos;
                fileColumn = savedFileColumn;
                fileLine = savedFileLine;
                lexeme = "";
                return nullopt;
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
                state = 2;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 2:
            c = getNextForLookahead();  // USA LOOKAHEAD AQUI
            if((isalpha(c) || isdigit(c))){
                getNext();  // SÓ CONSOME SE FOR VÁLIDO
                state = 2;
            } else {
                state = 3;
            }
            break;
        case 3: {
            string lexUpper = "";
            for(char ch : lexeme) {
                lexUpper += toupper(ch);
            }
            Token token("ID", lexUpper, "ID", tokenStartLine, tokenStartColumn, symbolTable.size());
            tokensList.push_back(token);
            insertSymbolInTable(token);
            lexeme = "";
            return token;
        }

        default:
            break;
        }
    }
}

optional <Token> getOpenParenthesis() {
    lexeme = "";
    int state = 1;
    char c;

    while(true) {
        switch (state) {
        case 1:
            c = getNext();
            if(c == '('){
                state = 2;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 2:
            Token token("LPAREN", lexeme, "LPAREN", tokenStartLine, tokenStartColumn, symbolTable.size());
            tokensList.push_back(token);
            insertSymbolInTable(token);
            lexeme = "";
            return token;

            break;
        }
    }

}

optional <Token> getCloseParenthesis() {
    lexeme = "";
    int state = 1;
    char c;

    while(true) {
        switch (state) {
        case 1:
            c = getNext();
            if(c == ')'){
                state = 2;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 2:
            Token token("LPAREN", lexeme, "LPAREN", tokenStartLine, tokenStartColumn, symbolTable.size());
            tokensList.push_back(token);
            insertSymbolInTable(token);
            lexeme = "";
            return token;

            break;
        }
    }

}

optional <Token> getCommaLexeme() {
    lexeme = "";
    int state = 1;
    char c;

    while(true) {
        switch (state) {
        case 1:
            c = getNext();
            if(c == ';'){
                state = 2;
            } else if( c == ',') {
                state = 3;
             } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 2: {
            Token token("COMMA", lexeme, "COMMA", tokenStartLine, tokenStartColumn, symbolTable.size());
            tokensList.push_back(token);
            insertSymbolInTable(token);
            lexeme = "";
            return token;
            break;
        }
        case 3: {
            Token token("COMMA", lexeme, "COMMA", tokenStartLine, tokenStartColumn, symbolTable.size());
            tokensList.push_back(token);
            insertSymbolInTable(token);
            lexeme = "";
            return token;
            break;
        }
        }
    }

}

optional <Token> getassignmentLexeme() {
    lexeme = "";
    int state = 1;
    char c;

    while(true) {
        switch (state) {
        case 1:
            c = getNext();
            if(c == ':'){
                state = 2;
            } else {
                FailedLexeme();
                return nullopt;
            }
            break;
        case 2:
            c = getNext();
            if(c == '=') {
                state = 3;
            } else {
                FailedLexeme();
                return nullopt;
            }
        case 3:
            Token token("ATRIB", lexeme, "ATRIB", tokenStartLine, tokenStartColumn, symbolTable.size());
            tokensList.push_back(token);
            insertSymbolInTable(token);
            lexeme = "";
            return token;

            break;
        }
    }

}

optional<Token> getMathOperatorLexeme() {
    lexeme = "";
    int state = 1;
    char c;

    while(true) {
        switch (state) {
            case 1:
                c = getNext();
                if(c == '*') {
                    state = 2;
                } else if(c == '+') {
                    state = 3;
                } else if(c == '-') {
                    state = 4;
                } else if(c == '/') {
                    state = 5;
                } else {
                    FailedLexeme();
                    return nullopt;
                }
                break;
                
            case 2:
                c = getNextForLookahead();
                if(c == '*') {
                    getNext(); // Consome o segundo '*'
                    Token token("EXP", lexeme, "EXP", tokenStartLine, tokenStartColumn, symbolTable.size());
                    tokensList.push_back(token);
                    insertSymbolInTable(token);
                    lexeme = "";
                    return token;
                } else {
                    // Apenas '*' (multiplicação)
                    Token token("MULT", lexeme, "MULT", tokenStartLine, tokenStartColumn, symbolTable.size());
                    tokensList.push_back(token);
                    insertSymbolInTable(token);
                    lexeme = "";
                    return token;
                }
                break;
                
            case 3: {
                // '+' (adição)
                Token token("PLUS", lexeme, "PLUS", tokenStartLine, tokenStartColumn, symbolTable.size());
                tokensList.push_back(token);
                insertSymbolInTable(token);
                lexeme = "";
                return token;
                break;
            }
                
            case 4: {
                Token token("MINUS", lexeme, "MINUS", tokenStartLine, tokenStartColumn, symbolTable.size());
                tokensList.push_back(token);
                insertSymbolInTable(token);
                lexeme = "";
                return token;
                break;
            }
                
            case 5: {
                Token token("DIV", lexeme, "DIV", tokenStartLine, tokenStartColumn, symbolTable.size());
                tokensList.push_back(token);
                insertSymbolInTable(token);
                lexeme = "";
                return token;
                break;
            }
        }
    }
}

optional<Token> getNumberLexeme() {
    lexeme = "";
    int state = 1;
    char c;
    
    // Guarda posições para possível rollback
    int savedStart = startLexeme;
    int savedNext = nextPosLexeme;
    int savedLine = fileLine;
    int savedCol = fileColumn;
    int savedTokenLine = tokenStartLine;
    int savedTokenCol = tokenStartColumn;

    while(true) {
        switch(state) {
            case 1: // Estado inicial - primeiro dígito
                c = getNext();
                if(isdigit(c)) {
                    if(c == '0') {
                        state = 2; // Zero como primeiro dígito
                    } else {
                        state = 3; // Dígito de 1-9
                    }
                } else if(c == '.') {
                    state = 4; // Ponto decimal sem dígito antes
                } else {
                    // Rollback
                    nextPosLexeme = savedNext;
                    fileLine = savedLine;
                    fileColumn = savedCol;
                    startLexeme = savedStart;
                    tokenStartLine = savedTokenLine;
                    tokenStartColumn = savedTokenCol;
                    lexeme = "";
                    return nullopt;
                }
                break;
                
            case 2: // Zero como primeiro dígito
                c = getNextForLookahead();
                if(c == '.') {
                    getNext(); // Consome o ponto
                    state = 6; // Vai para parte fracionária
                } else if(c == 'E' || c == 'e') {
                    getNext(); // Consome o E
                    state = 8; // Vai para expoente
                } else if(isdigit(c)) {
                    // Zero seguido de dígito - não é válido (exceto em ponto flutuante)
                    nextPosLexeme = savedNext;
                    fileLine = savedLine;
                    fileColumn = savedCol;
                    startLexeme = savedStart;
                    tokenStartLine = savedTokenLine;
                    tokenStartColumn = savedTokenCol;
                    lexeme = "";
                    return nullopt;
                } else {
                    state = 10; // Apenas "0" - inteiro válido
                }
                break;
                
            case 3: // Dígitos inteiros (1-9 seguido de mais dígitos)
                c = getNextForLookahead();
                if(isdigit(c)) {
                    getNext();
                    // Continua no estado 3
                } else if(c == '.') {
                    getNext();
                    state = 6; // Vai para parte fracionária
                } else if(c == 'E' || c == 'e') {
                    getNext();
                    state = 8; // Vai para expoente
                } else {
                    state = 10; // Fim do inteiro
                }
                break;
                
            case 4: // Ponto decimal sem dígito antes (ex: .5)
                c = getNextForLookahead();
                if(isdigit(c)) {
                    getNext();
                    state = 5; // Dígitos após ponto inicial
                } else {
                    // Ponto sozinho não é número válido
                    nextPosLexeme = savedNext;
                    fileLine = savedLine;
                    fileColumn = savedCol;
                    startLexeme = savedStart;
                    tokenStartLine = savedTokenLine;
                    tokenStartColumn = savedTokenCol;
                    lexeme = "";
                    return nullopt;
                }
                break;
                
            case 5: // Parte fracionária (após ponto inicial)
                c = getNextForLookahead();
                if(isdigit(c)) {
                    getNext();
                    // Continua no estado 5
                } else if(c == 'E' || c == 'e') {
                    getNext();
                    state = 8; // Vai para expoente
                } else {
                    state = 11; // Fim do float (ex: .123)
                }
                break;
                
            case 6: // Após ponto decimal em número com parte inteira (ex: 123.)
                c = getNextForLookahead();
                if(isdigit(c)) {
                    getNext();
                    state = 7; // Dígitos após ponto
                } else {
                    state = 11; // Fim do float (ex: 123.)
                }
                break;
                
            case 7: // Dígitos após ponto decimal (ex: 123.45)
                c = getNextForLookahead();
                if(isdigit(c)) {
                    getNext();
                    // Continua no estado 7
                } else if(c == 'E' || c == 'e') {
                    getNext();
                    state = 8; // Vai para expoente
                } else {
                    state = 11; // Fim do float sem expoente
                }
                break;
                
            case 8: // Após 'E' ou 'e' (início do expoente)
                c = getNextForLookahead();
                if(c == '+' || c == '-') {
                    getNext(); // Consome sinal
                    state = 9; // Vai para dígitos do expoente
                } else if(isdigit(c)) {
                    getNext();
                    state = 9; // Dígitos do expoente sem sinal
                } else {
                    // 'E' ou 'e' sem dígitos após - inválido
                    nextPosLexeme = savedNext;
                    fileLine = savedLine;
                    fileColumn = savedCol;
                    startLexeme = savedStart;
                    tokenStartLine = savedTokenLine;
                    tokenStartColumn = savedTokenCol;
                    lexeme = "";
                    return nullopt;
                }
                break;
                
            case 9: // Dígitos do expoente
                c = getNextForLookahead();
                if(isdigit(c)) {
                    getNext();
                    // Continua no estado 9
                } else {
                    state = 11; // Fim do float com notação científica
                }
                break;
                
            case 10: { // Estado final para INT
                // Verifica se o valor está no intervalo permitido (0-32767)
                try {
                    int intValue = stoi(lexeme);
                    if(intValue < 0 || intValue > 32767) {
                        cerr << "Erro: inteiro fora do intervalo permitido (0-32767): " << lexeme 
                             << " na linha " << tokenStartLine << ", coluna " << tokenStartColumn << endl;
                        nextPosLexeme = savedNext;
                        fileLine = savedLine;
                        fileColumn = savedCol;
                        startLexeme = savedStart;
                        tokenStartLine = savedTokenLine;
                        tokenStartColumn = savedTokenCol;
                        lexeme = "";
                        return nullopt;
                    }
                    
                    Token token("INT_CONST", lexeme, "INT", tokenStartLine, tokenStartColumn, symbolTable.size());
                    tokensList.push_back(token);
                    insertSymbolInTable(token);
                    lexeme = "";
                    return token;
                } catch(const exception& e) {
                    cerr << "Erro ao converter inteiro: " << lexeme << endl;
                    nextPosLexeme = savedNext;
                    fileLine = savedLine;
                    fileColumn = savedCol;
                    startLexeme = savedStart;
                    tokenStartLine = savedTokenLine;
                    tokenStartColumn = savedTokenCol;
                    lexeme = "";
                    return nullopt;
                }
                break;
            }
                
            case 11: { // Estado final para FLOAT
                // Verifica se é um float válido
                try {
                    // Tenta converter para float para validar
                    float floatValue = stof(lexeme);
                    
                    Token token("FLOAT_CONST", lexeme, "FLOAT", tokenStartLine, tokenStartColumn, symbolTable.size());
                    tokensList.push_back(token);
                    insertSymbolInTable(token);
                    lexeme = "";
                    return token;
                } catch(const exception& e) {
                    cerr << "Erro ao converter float: " << lexeme 
                         << " na linha " << tokenStartLine << ", coluna " << tokenStartColumn << endl;
                    nextPosLexeme = savedNext;
                    fileLine = savedLine;
                    fileColumn = savedCol;
                    startLexeme = savedStart;
                    tokenStartLine = savedTokenLine;
                    tokenStartColumn = savedTokenCol;
                    lexeme = "";
                    return nullopt;
                }
                break;
            }
                
            default:
                nextPosLexeme = savedNext;
                fileLine = savedLine;
                fileColumn = savedCol;
                startLexeme = savedStart;
                tokenStartLine = savedTokenLine;
                tokenStartColumn = savedTokenCol;
                lexeme = "";
                return nullopt;
        }
    }
}

optional<Token> getCharConstLexeme() {
    lexeme = "";
    int state = 1;
    char c;
    
    // Guarda posições para possível rollback
    int savedNext = nextPosLexeme;
    int savedLine = fileLine;
    int savedCol = fileColumn;
    int savedStart = startLexeme;
    int savedTokenLine = tokenStartLine;
    int savedTokenCol = tokenStartColumn;

    while(true) {
        switch(state) {
            case 1: // Espera a primeira aspas
                c = getNext();
                if(c == '\'') {
                    state = 2;
                } else {
                    // Rollback
                    nextPosLexeme = savedNext;
                    fileLine = savedLine;
                    fileColumn = savedCol;
                    startLexeme = savedStart;
                    tokenStartLine = savedTokenLine;
                    tokenStartColumn = savedTokenCol;
                    lexeme = "";
                    return nullopt;
                }
                break;
                
            case 2: // Dentro das aspas - espera um caractere
                c = getNext();
                if(c == '\'') {
                    // Aspas vazias '' - inválido
                    nextPosLexeme = savedNext;
                    fileLine = savedLine;
                    fileColumn = savedCol;
                    startLexeme = savedStart;
                    tokenStartLine = savedTokenLine;
                    tokenStartColumn = savedTokenCol;
                    lexeme = "";
                    return nullopt;
                } else if(c == '\\') {
                    // Caractere de escape
                    state = 3;
                } else if(c == '\n' || c == '\0') {
                    // Fim de linha ou arquivo sem fechar aspas - erro
                    nextPosLexeme = savedNext;
                    fileLine = savedLine;
                    fileColumn = savedCol;
                    startLexeme = savedStart;
                    tokenStartLine = savedTokenLine;
                    tokenStartColumn = savedTokenCol;
                    lexeme = "";
                    return nullopt;
                } else {
                    // Caractere normal
                    state = 4;
                }
                break;
                
            case 3: // Após barra invertida - caractere de escape
                c = getNext();
                if(c == 'n' || c == 't' || c == 'r' || c == '\\' || c == '\'' || c == '"') {
                    // Sequências de escape válidas: \n, \t, \r, \\, \', \"
                    state = 4;
                } else {
                    // Sequência de escape inválida
                    nextPosLexeme = savedNext;
                    fileLine = savedLine;
                    fileColumn = savedCol;
                    startLexeme = savedStart;
                    tokenStartLine = savedTokenLine;
                    tokenStartColumn = savedTokenCol;
                    lexeme = "";
                    return nullopt;
                }
                break;
                
            case 4: // Espera a aspas de fechamento
                c = getNext();
                if(c == '\'') {
                    state = 5; // Aspas de fechamento encontrada
                } else {
                    // Mais de um caractere dentro das aspas - inválido para char
                    nextPosLexeme = savedNext;
                    fileLine = savedLine;
                    fileColumn = savedCol;
                    startLexeme = savedStart;
                    tokenStartLine = savedTokenLine;
                    tokenStartColumn = savedTokenCol;
                    lexeme = "";
                    return nullopt;
                }
                break;
                
            case 5: { // Estado final - char válido
                // O lexeme completo inclui as aspas: ex: 'A' ou '\n'
                // Mas podemos extrair apenas o valor interno para a tabela de símbolos
                string value = lexeme.substr(1, lexeme.length() - 2); // Remove as aspas
                
                Token token("CHAR_CONST", lexeme, "CHAR", tokenStartLine, tokenStartColumn, symbolTable.size());
                tokensList.push_back(token);
                insertSymbolInTable(token);
                lexeme = "";
                return token;
                break;
            }
                
            default:
                // Rollback em caso de erro
                nextPosLexeme = savedNext;
                fileLine = savedLine;
                fileColumn = savedCol;
                startLexeme = savedStart;
                tokenStartLine = savedTokenLine;
                tokenStartColumn = savedTokenCol;
                lexeme = "";
                return nullopt;
        }
    }
}

void skipWhitespaceAndComments() {
    while (nextPosLexeme < buffer.length()) {
        char c = buffer[nextPosLexeme];

        if (isspace(c)) {
            if (c == '\n') {
                fileLine++;
                fileColumn = 1;
            } else {
                fileColumn++;
            }
            nextPosLexeme++;
            continue;
        }

        if (c == '%') {
            nextPosLexeme++;  // consome o %
            while (nextPosLexeme < buffer.length()) {
                c = buffer[nextPosLexeme];
                if (c == '%') {
                    nextPosLexeme++;  // consome o % final
                    break;
                }
                if (c == '\n') {
                    fileLine++;
                    fileColumn = 1;
                } else {
                    fileColumn++;
                }
                nextPosLexeme++;
            }
            continue;
        }

        // não é espaço nem comentário → sai
        break;
    }
}

void lexer() {

    while(nextPosLexeme < buffer.size()) {
        skipWhitespaceAndComments();
        if(nextPosLexeme >= buffer.size()) break;
        
        startLexeme = nextPosLexeme;
        tokenStartLine = fileLine;
        tokenStartColumn = fileColumn;
        
        token = getIfLexeme();
        if(!token.has_value()) token = getVoidLexeme();
        if(!token.has_value()) token = getIntLexeme();
        if(!token.has_value()) token = getCharLexeme();
        if(!token.has_value()) token = getFloatLexeme();
        if(!token.has_value()) token = getMainLexeme();
        if(!token.has_value()) token = getThenLexeme();
        if(!token.has_value()) token = getElsifLexeme();
        if(!token.has_value()) token = getElseLexeme();
        if(!token.has_value()) token = getForLexeme();
        if(!token.has_value()) token = getOpenParenthesis();
        if(!token.has_value()) token = getCloseParenthesis();
        if(!token.has_value()) token = getCommaLexeme();
        if(!token.has_value()) token = getassignmentLexeme();
        if(!token.has_value()) token = getMathOperatorLexeme();
        if(!token.has_value()) token = getNumberLexeme();
        if(!token.has_value()) token = getCharConstLexeme();
        if(!token.has_value()) token = getRelopLexeme();
        
        if(!token.has_value()) token = getIdLexeme();
        
        if(!token.has_value()) {
            cerr << "Erro léxico na linha " << fileLine 
                 << ", coluna " << fileColumn 
                 << ": caractere inválido '" 
                 << buffer[nextPosLexeme] << "'" << endl;
            nextPosLexeme++;
            fileColumn++;
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

