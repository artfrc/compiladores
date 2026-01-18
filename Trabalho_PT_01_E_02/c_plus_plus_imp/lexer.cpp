#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cctype>
using namespace std;

// ============================================================
// ESTRUTURAS DE DADOS
// ============================================================
enum TokenType {
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    DELIMITER,
    END_OF_INPUT,
    UNKNOWN
};

struct Token {
    TokenType type;
    string lexeme;
    int line;
    int column;
};

// ============================================================
// CLASSE ANALISADOR LÉXICO - ABORDAGEM 2
// ============================================================
class LexerTabelaReservadas {
private:
    string input;
    size_t pos;
    int line;
    int column;
    unordered_set<string> keywords; // Tabela de palavras reservadas
    
    char peek() {
        if (pos >= input.length()) return '\0';
        return input[pos];
    }
    
    char advance() {
        if (pos >= input.length()) return '\0';
        char ch = input[pos++];
        if (ch == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        return ch;
    }
    
    void skipWhitespace() {
        while (isspace(peek())) {
            advance();
        }
    }
    
    // DIAGRAMA ÚNICO: Identificador ou Keyword
    Token scanIdentifierOrKeyword() {
        string lexeme = "";
        int col_start = column;
        
        // q0 -> q1 (lê letra ou _)
        if (isalpha(peek()) || peek() == '_') {
            lexeme += advance();
        } else {
            return {UNKNOWN, "", line, column};
        }
        
        // q1: loop (maximal munch - consome tudo que for alfanumérico)
        while (isalnum(peek()) || peek() == '_') {
            lexeme += advance();
        }
        
        // Ao sair do loop: lexema completo extraído
        // Consulta tabela de palavras reservadas
        if (keywords.find(lexeme) != keywords.end()) {
            return {KEYWORD, lexeme, line, col_start};
        } else {
            return {IDENTIFIER, lexeme, line, col_start};
        }
    }
    
    // DIAGRAMA: Número
    Token scanNumber() {
        string lexeme = "";
        int col_start = column;
        
        while (isdigit(peek())) {
            lexeme += advance();
        }
        
        return {NUMBER, lexeme, line, col_start};
    }
    
    // DIAGRAMA: Operadores
    Token scanOperator() {
        string lexeme(1, advance());
        return {OPERATOR, lexeme, line, column - 1};
    }
    
    // DIAGRAMA: Delimitadores
    Token scanDelimiter() {
        string lexeme(1, advance());
        return {DELIMITER, lexeme, line, column - 1};
    }
    
public:
    LexerTabelaReservadas(const string& source) 
        : input(source), pos(0), line(1), column(1) {
        // Inicializa tabela de palavras reservadas
        keywords.insert("if");
        keywords.insert("else");
        keywords.insert("while");
        keywords.insert("for");
        keywords.insert("do");
        keywords.insert("return");
        keywords.insert("int");
        keywords.insert("float");
        keywords.insert("void");
        keywords.insert("char");
    }
    
    Token nextToken() {
        skipWhitespace();
        
        if (peek() == '\0') {
            return {END_OF_INPUT, "", line, column};
        }
        
        // Identificador ou Keyword (diagrama único)
        if (isalpha(peek()) || peek() == '_') {
            return scanIdentifierOrKeyword();
        }
        
        // Número
        if (isdigit(peek())) {
            return scanNumber();
        }
        
        // Operadores
        if (peek() == '+' || peek() == '-' || peek() == '*' || 
            peek() == '/' || peek() == '=' || peek() == '<' || peek() == '>') {
            return scanOperator();
        }
        
        // Delimitadores
        if (peek() == '(' || peek() == ')' || peek() == '{' || 
            peek() == '}' || peek() == ';' || peek() == ',') {
            return scanDelimiter();
        }
        
        return {UNKNOWN, string(1, advance()), line, column - 1};
    }
    
    vector<Token> tokenize() {
        vector<Token> tokens;
        Token token;
        
        do {
            token = nextToken();
            tokens.push_back(token);
        } while (token.type != END_OF_INPUT);
        
        return tokens;
    }
    
    void printKeywordsTable() {
        cout << "\n┌─────────────────────────────────────────────────────────────┐" << endl;
        cout << "│  TABELA DE PALAVRAS RESERVADAS                              │" << endl;
        cout << "└─────────────────────────────────────────────────────────────┘" << endl;
        cout << "Hash Table (lookup em O(1)):\n";
        for (const string& kw : keywords) {
            cout << "  • " << kw << endl;
        }
    }
};

// ============================================================
// FUNÇÕES UTILITÁRIAS
// ============================================================
string tokenTypeToString(TokenType type) {
    switch(type) {
        case KEYWORD: return "KEYWORD    ";
        case IDENTIFIER: return "IDENTIFIER ";
        case NUMBER: return "NUMBER     ";
        case OPERATOR: return "OPERATOR   ";
        case DELIMITER: return "DELIMITER  ";
        case END_OF_INPUT: return "END        ";
        default: return "UNKNOWN    ";
    }
}

string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir arquivo: " << filename << endl;
        return "";
    }
    
    string content((istreambuf_iterator<char>(file)),
                   istreambuf_iterator<char>());
    file.close();
    return content;
}

void printTokens(const vector<Token>& tokens) {
    cout << "\n┌─────────────────────────────────────────────────────────────┐" << endl;
    cout << "│  TOKENS RECONHECIDOS                                        │" << endl;
    cout << "└─────────────────────────────────────────────────────────────┘" << endl;
    cout << "Linha:Col  | Tipo         | Lexema" << endl;
    cout << "─────────────────────────────────────────────────────────────" << endl;
    
    for (const Token& token : tokens) {
        if (token.type != END_OF_INPUT) {
            printf("%4d:%-4d | %s | \"%s\"\n", 
                   token.line, token.column, 
                   tokenTypeToString(token.type).c_str(), 
                   token.lexeme.c_str());
        }
    }
}

void createSampleFile(const string& filename) {
    ofstream file(filename);
    file << "if (x > 10) {\n";
    file << "    y = x + 5;\n";
    file << "} else {\n";
    file << "    while (z < 20) {\n";
    file << "        z = z + 1;\n";
    file << "    }\n";
    file << "}\n";
    file << "ifx = 100;\n";
    file << "elseif = 200;\n";
    file << "int main() {\n";
    file << "    for (i = 0; i < 10; i++) {\n";
    file << "        return i;\n";
    file << "    }\n";
    file << "}\n";
    file.close();
}

// ============================================================
// FUNÇÃO PRINCIPAL
// ============================================================
int main(int argc, char* argv[]) {
    cout << "╔═══════════════════════════════════════════════════════════╗" << endl;
    cout << "║  ANALISADOR LÉXICO - ABORDAGEM 2: TABELA DE RESERVADAS   ║" << endl;
    cout << "╚═══════════════════════════════════════════════════════════╝" << endl;
    
    string filename;
    
    if (argc > 1) {
        // Arquivo passado como argumento
        filename = argv[1];
    } else {
        // Cria arquivo de exemplo
        filename = "codigo_exemplo2.txt";
        cout << "\nNenhum arquivo especificado. Criando arquivo de exemplo..." << endl;
        createSampleFile(filename);
        cout << "Arquivo criado: " << filename << endl;
    }
    
    // Lê o arquivo
    cout << "\nLendo arquivo: " << filename << endl;
    string sourceCode = readFile(filename);
    
    if (sourceCode.empty()) {
        cout << "\nArquivo vazio ou não encontrado!" << endl;
        cout << "\nUso: " << argv[0] << " [arquivo.txt]" << endl;
        return 1;
    }
    
    // Mostra o código fonte
    cout << "\n┌─────────────────────────────────────────────────────────────┐" << endl;
    cout << "│  CÓDIGO FONTE                                               │" << endl;
    cout << "└─────────────────────────────────────────────────────────────┘" << endl;
    cout << sourceCode << endl;
    
    // Cria o lexer e mostra tabela de keywords
    LexerTabelaReservadas lexer(sourceCode);
    lexer.printKeywordsTable();
    
    // Executa análise léxica
    cout << "\nExecutando análise léxica..." << endl;
    vector<Token> tokens = lexer.tokenize();
    
    // Mostra tokens
    printTokens(tokens);
    
    // Estatísticas
    int keywordCount = 0, idCount = 0, numCount = 0;
    for (const Token& t : tokens) {
        if (t.type == KEYWORD) keywordCount++;
        else if (t.type == IDENTIFIER) idCount++;
        else if (t.type == NUMBER) numCount++;
    }
    
    cout << "\n┌─────────────────────────────────────────────────────────────┐" << endl;
    cout << "│  ESTATÍSTICAS                                               │" << endl;
    cout << "└─────────────────────────────────────────────────────────────┘" << endl;
    cout << "Total de tokens: " << tokens.size() - 1 << endl;
    cout << "Keywords: " << keywordCount << endl;
    cout << "Identificadores: " << idCount << endl;
    cout << "Números: " << numCount << endl;
    
    cout << "\n┌─────────────────────────────────────────────────────────────┐" << endl;
    cout << "│  FUNCIONAMENTO                                              │" << endl;
    cout << "└─────────────────────────────────────────────────────────────┘" << endl;
    cout << "• Diagrama ÚNICO para identificadores e keywords" << endl;
    cout << "• Maximal munch: consome o máximo de caracteres possível" << endl;
    cout << "• Após extrair lexema completo, consulta tabela hash" << endl;
    cout << "• 'ifx' → extraído completo → NÃO está na tabela → IDENTIFIER" << endl;
    cout << "• 'if' → extraído completo → ESTÁ na tabela → KEYWORD" << endl;
    cout << "• Sem lookahead, sem backtracking, mais eficiente!" << endl;
    
    return 0;
}