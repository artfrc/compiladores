#include <bits/stdc++.h>
using namespace std;

#define NAME_FILE "file.txt"

string buffer;
int startLexeme = 0, nextPosLexeme = 0; 

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
        name(n), lexeme(l), startLine(sl), startColumn(sc) {}

    Token(string n, string l, string t, int sl, int sc, int pst) : 
        name(n), lexeme(l), type(t), startLine(sl), startColumn(sc), posSymbolTable(pst) {}

    bool operator==(const Token& other) const {
        return lexeme == other.lexeme;
    }

};

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

void printSymbolTable() {

    cout << "\n================ TABELA DE SIMBOLOS ================\n";
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
             << setw(15) << tok.lexeme
             << setw(15) << tok.name
             << setw(15) << tok.type
             << setw(10) << tok.startLine
             << setw(10) << tok.startColumn
             << endl;
    }

    cout << "====================================================\n";
}


int main() {

    buffer = getContentFile();

}