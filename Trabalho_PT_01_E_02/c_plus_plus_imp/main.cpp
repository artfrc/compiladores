#include <bits/stdc++.h>
using namespace std;

#define LEN_BUFFER 20  
#define HALF_BUFFER 10 

string code;
int globalIndex = 0;
int currentLine = 1;
int lineStartIndex = 0;

struct Symbol {
   string token;
   string lexeme;
   string type;
};

unordered_map<string, Symbol> symbolTable;

struct BufferState {
    int globalIdx;
    int bufferPosition;
    bool usingBuffer1;
    int currentLineNum;
    int lineStart;
};

string PassFileToString();
void fillBuffer(char* buffer, int startPos);
char getNextChar(char* bufferOne, char* bufferTwo, int& bufferPos, bool& usingBufferOne);
char peekChar(char* bufferOne, char* bufferTwo, int bufferPos, bool usingBufferOne);
BufferState saveState(int bufferPos, bool usingBufferOne);
void restoreState(const BufferState& state, int& bufferPos, bool& usingBufferOne);

bool diagramWhile(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1);
bool diagramMain(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1);
bool diagramOpenBrace(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1);
bool diagramCloseBrace(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1);
bool diagramOpenBracket(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1);
bool diagramCloseBracket(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1);
bool diagramOpenParen(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1);
bool diagramCloseParen(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1);
bool diagramVoid(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1);
bool diagramInt(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1);
bool diagramChar(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1);
bool diagramFloat(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1);
bool diagramComma(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1);
bool diagramSemicolon(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1);
bool diagramAssignment(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1);
bool diagramGreater(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1);
bool diagramNumber(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1, string& lexeme);
bool diagramID(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1, string& lexeme);
bool diagramCharLiteral(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1, string& lexeme);

void insertSymbol(const string& token, const string& lexeme, const string& dataType);
void printSymbolTable();

BufferState saveState(int bufferPos, bool usingBufferOne) {
    return {globalIndex, bufferPos, usingBufferOne, currentLine, lineStartIndex};
}

void restoreState(const BufferState& state, int& bufferPos, bool& usingBufferOne) {
    globalIndex = state.globalIdx;
    bufferPos = state.bufferPosition;
    usingBufferOne = state.usingBuffer1;
    currentLine = state.currentLineNum;
    lineStartIndex = state.lineStart;
}

string PassFileToString() {
    ifstream file("file.txt");
    if (!file) throw runtime_error("Erro ao abrir o arquivo");
    
    string fileInString(
        (istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>()
    );
    return fileInString;
}

void insertSymbol(const string& token, const string& lexeme, const string& dataType) {
    if (symbolTable.find(lexeme) == symbolTable.end()) {
        symbolTable[lexeme] = { token, lexeme, dataType };
    }
}

void printSymbolTable() {
    cout << "\n---- TABELA DE SÍMBOLOS ----\n";
    cout << "TOKEN\t\tLEXEMA\t\tTIPO\n";
    
    if (symbolTable.empty()) {
        cout << "(vazia)\n";
        return;
    }
    
    for (const auto& [lexeme, symbol] : symbolTable) {
        cout << symbol.token << "\t\t"
             << symbol.lexeme << "\t\t"
             << symbol.type << "\n";
    }
}

void fillBuffer(char* buffer, int startPos) {
    for (int i = 0; i < LEN_BUFFER; i++) {
        if (startPos + i < code.length()) {
            buffer[i] = code[startPos + i];
        } else {
            buffer[i] = '\0';
        }
    }
}

void fillHalfBuffer(char* buffer, int halfNumber, int startPos) {
    // halfNumber: 0 = primeira metade, 1 = segunda metade
    int startIndex = halfNumber * HALF_BUFFER;
    
    for (int i = 0; i < HALF_BUFFER; i++) {
        if (startPos + i < code.length()) {
            buffer[startIndex + i] = code[startPos + i];
        } else {
            buffer[startIndex + i] = '\0';
        }
    }
}

char getNextChar(char* bufferOne, char* bufferTwo, int& bufferPos, bool& usingBufferOne) {
    if (globalIndex >= code.length()) {
        return '\0';
    }
    
    char* currentBuffer = usingBufferOne ? bufferOne : bufferTwo;
    char currentChar = currentBuffer[bufferPos];
    
    if (currentChar == '\n') {
        currentLine++;
        lineStartIndex = globalIndex + 1;
    }
    
    bufferPos++;
    globalIndex++;
    
    if (bufferPos == HALF_BUFFER) {
        char* otherBuffer = usingBufferOne ? bufferTwo : bufferOne;
        fillHalfBuffer(otherBuffer, 1, globalIndex);
    }
    else if (bufferPos >= LEN_BUFFER) {
        char* otherBuffer = usingBufferOne ? bufferTwo : bufferOne;
        fillHalfBuffer(otherBuffer, 0, globalIndex);
        
        bufferPos = 0;
        usingBufferOne = !usingBufferOne;
    }
    
    return currentChar;
}

char peekChar(char* bufferOne, char* bufferTwo, int bufferPos, bool usingBufferOne) {
    if (globalIndex >= code.length()) {
        return '\0';
    }
    
    char* currentBuffer = usingBufferOne ? bufferOne : bufferTwo;
    return currentBuffer[bufferPos];
}

bool diagramWhile(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1) {
    int state = 0;
    BufferState savedState = saveState(bufferPos, usingBuffer1);
    
    while (true) {
        char currentChar = getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
        
        switch(state) {
            case 0:
                if (currentChar == 'w') state = 1;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
            case 1:
                if (currentChar == 'h') state = 2;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
            case 2:
                if (currentChar == 'i') state = 3;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
            case 3:
                if (currentChar == 'l') state = 4;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
            case 4:
                if (currentChar == 'e') return true;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
        }
    }
}

bool diagramMain(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1) {
    int state = 0;
    BufferState savedState = saveState(bufferPos, usingBuffer1);
    
    while (true) {
        char currentChar = getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
        
        switch(state) {
            case 0:
                if (currentChar == 'm') state = 1;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
            case 1:
                if (currentChar == 'a') state = 2;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
            case 2:
                if (currentChar == 'i') state = 3;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
            case 3:
                if (currentChar == 'n') return true;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
        }
    }
}

bool diagramVoid(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1) {
    int state = 0;
    BufferState savedState = saveState(bufferPos, usingBuffer1);
    
    while (true) {
        char currentChar = getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
        
        switch(state) {
            case 0:
                if (currentChar == 'v') state = 1;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
            case 1:
                if (currentChar == 'o') state = 2;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
            case 2:
                if (currentChar == 'i') state = 3;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
            case 3:
                if (currentChar == 'd') return true;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
        }
    }
}

bool diagramInt(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1) {
    int state = 0;
    BufferState savedState = saveState(bufferPos, usingBuffer1);
    
    while (true) {
        char currentChar = getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
        
        switch(state) {
            case 0:
                if (currentChar == 'i') state = 1;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
            case 1:
                if (currentChar == 'n') state = 2;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
            case 2:
                if (currentChar == 't') {
                    char nextChar = peekChar(buffer1, buffer2, bufferPos, usingBuffer1);
                    if (!isalnum(nextChar) && nextChar != '_') {
                        return true;
                    } else {
                        restoreState(savedState, bufferPos, usingBuffer1);
                        return false;
                    }
                } else {
                    restoreState(savedState, bufferPos, usingBuffer1);
                    return false;
                }
                break;
        }
    }
}

bool diagramChar(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1) {
    int state = 0;
    BufferState savedState = saveState(bufferPos, usingBuffer1);
    
    while (true) {
        char currentChar = getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
        
        switch(state) {
            case 0:
                if (currentChar == 'c') state = 1;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
            case 1:
                if (currentChar == 'h') state = 2;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
            case 2:
                if (currentChar == 'a') state = 3;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
            case 3:
                if (currentChar == 'r') {
                    char nextChar = peekChar(buffer1, buffer2, bufferPos, usingBuffer1);
                    if (!isalnum(nextChar) && nextChar != '_') {
                        return true;
                    } else {
                        restoreState(savedState, bufferPos, usingBuffer1);
                        return false;
                    }
                } else {
                    restoreState(savedState, bufferPos, usingBuffer1);
                    return false;
                }
                break;
        }
    }
}

bool diagramFloat(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1) {
    int state = 0;
    BufferState savedState = saveState(bufferPos, usingBuffer1);
    
    while (true) {
        char currentChar = getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
        
        switch(state) {
            case 0:
                if (currentChar == 'f') state = 1;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
            case 1:
                if (currentChar == 'l') state = 2;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
            case 2:
                if (currentChar == 'o') state = 3;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
            case 3:
                if (currentChar == 'a') state = 4;
                else { restoreState(savedState, bufferPos, usingBuffer1); return false; }
                break;
            case 4:
                if (currentChar == 't') {
                    char nextChar = peekChar(buffer1, buffer2, bufferPos, usingBuffer1);
                    if (!isalnum(nextChar) && nextChar != '_') {
                        return true;
                    } else {
                        restoreState(savedState, bufferPos, usingBuffer1);
                        return false;
                    }
                } else {
                    restoreState(savedState, bufferPos, usingBuffer1);
                    return false;
                }
                break;
        }
    }
}

bool diagramOpenParen(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1) {
    BufferState savedState = saveState(bufferPos, usingBuffer1);
    char currentChar = getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
    
    if (currentChar == '(') return true;
    
    restoreState(savedState, bufferPos, usingBuffer1);
    return false;
}

bool diagramCloseParen(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1) {
    BufferState savedState = saveState(bufferPos, usingBuffer1);
    char currentChar = getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
    
    if (currentChar == ')') return true;
    
    restoreState(savedState, bufferPos, usingBuffer1);
    return false;
}

bool diagramOpenBrace(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1) {
    BufferState savedState = saveState(bufferPos, usingBuffer1);
    char currentChar = getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
    
    if (currentChar == '{') return true;
    
    restoreState(savedState, bufferPos, usingBuffer1);
    return false;
}

bool diagramCloseBrace(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1) {
    BufferState savedState = saveState(bufferPos, usingBuffer1);
    char currentChar = getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
    
    if (currentChar == '}') return true;
    
    restoreState(savedState, bufferPos, usingBuffer1);
    return false;
}

bool diagramOpenBracket(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1) {
    BufferState savedState = saveState(bufferPos, usingBuffer1);
    char currentChar = getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
    
    if (currentChar == '[') return true;
    
    restoreState(savedState, bufferPos, usingBuffer1);
    return false;
}

bool diagramCloseBracket(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1) {
    BufferState savedState = saveState(bufferPos, usingBuffer1);
    char currentChar = getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
    
    if (currentChar == ']') return true;
    
    restoreState(savedState, bufferPos, usingBuffer1);
    return false;
}

bool diagramComma(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1) {
    BufferState savedState = saveState(bufferPos, usingBuffer1);
    char currentChar = getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
    
    if (currentChar == ',') return true;
    
    restoreState(savedState, bufferPos, usingBuffer1);
    return false;
}

bool diagramSemicolon(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1) {
    BufferState savedState = saveState(bufferPos, usingBuffer1);
    char currentChar = getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
    
    if (currentChar == ';') return true;
    
    restoreState(savedState, bufferPos, usingBuffer1);
    return false;
}

bool diagramAssignment(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1) {
    BufferState savedState = saveState(bufferPos, usingBuffer1);
    char currentChar = getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
    
    if (currentChar == '=') return true;
    
    restoreState(savedState, bufferPos, usingBuffer1);
    return false;
}

bool diagramGreater(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1) {
    BufferState savedState = saveState(bufferPos, usingBuffer1);
    char currentChar = getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
    
    if (currentChar == '>') return true;
    
    restoreState(savedState, bufferPos, usingBuffer1);
    return false;
}

bool diagramNumber(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1, string& lexeme) {
    int state = 0;
    BufferState savedState = saveState(bufferPos, usingBuffer1);
    lexeme = "";
    
    while (true) {
        char currentChar = peekChar(buffer1, buffer2, bufferPos, usingBuffer1);
        
        switch(state) {
            case 0:
                if (isdigit(currentChar)) {
                    lexeme += currentChar;
                    getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
                    state = 1;
                } else {
                    restoreState(savedState, bufferPos, usingBuffer1);
                    return false;
                }
                break;
            case 1:
                if (isdigit(currentChar)) {
                    lexeme += currentChar;
                    getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
                } else if (currentChar == '.') {
                    lexeme += currentChar;
                    getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
                    state = 2;
                } else {
                    return true;
                }
                break;
            case 2:
                if (isdigit(currentChar)) {
                    lexeme += currentChar;
                    getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
                    state = 3;
                } else {
                    restoreState(savedState, bufferPos, usingBuffer1);
                    return false;
                }
                break;
            case 3:
                if (isdigit(currentChar)) {
                    lexeme += currentChar;
                    getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
                } else {
                    return true;
                }
                break;
        }
    }
}

bool diagramID(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1, string& lexeme) {
    int state = 0;
    BufferState savedState = saveState(bufferPos, usingBuffer1);
    lexeme = "";
    
    while (true) {
        char currentChar = peekChar(buffer1, buffer2, bufferPos, usingBuffer1);
        
        switch(state) {
            case 0:
                if (isalpha(currentChar) || currentChar == '_') {
                    lexeme += currentChar;
                    getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
                    state = 1;
                } else {
                    restoreState(savedState, bufferPos, usingBuffer1);
                    return false;
                }
                break;
            case 1:
                if (isalnum(currentChar) || currentChar == '_') {
                    lexeme += currentChar;
                    getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
                } else {
                    return true;
                }
                break;
        }
    }
}

bool diagramCharLiteral(char* buffer1, char* buffer2, int& bufferPos, bool& usingBuffer1, string& lexeme) {
    int state = 0;
    BufferState savedState = saveState(bufferPos, usingBuffer1);
    lexeme = "";
    
    while (true) {
        char currentChar = getNextChar(buffer1, buffer2, bufferPos, usingBuffer1);
        
        switch(state) {
            case 0:
                if (currentChar == '\'') {
                    lexeme += currentChar;
                    state = 1;
                } else {
                    restoreState(savedState, bufferPos, usingBuffer1);
                    return false;
                }
                break;
            case 1:
                if (currentChar != '\0' && currentChar != '\'') {
                    lexeme += currentChar;
                    state = 2;
                } else {
                    restoreState(savedState, bufferPos, usingBuffer1);
                    return false;
                }
                break;
            case 2:
                if (currentChar == '\'') {
                    lexeme += currentChar;
                    return true;
                } else {
                    restoreState(savedState, bufferPos, usingBuffer1);
                    return false;
                }
                break;
        }
    }
}

char* initializeBuffer() {
    char* buffer = new char[LEN_BUFFER]();
    return buffer;
}

void lexer() {
   char* bufferOne = initializeBuffer();
   char* bufferTwo = initializeBuffer();
   
   fillBuffer(bufferOne, 0);
   fillHalfBuffer(bufferTwo, 0, LEN_BUFFER);
   
   globalIndex = 0;
   currentLine = 1;
   lineStartIndex = 0;
   int bufferPos = 0;
   bool usingBufferOne = true;
   
   while (globalIndex < code.length()) {
      char currentChar = peekChar(bufferOne, bufferTwo, bufferPos, usingBufferOne);
      if (isspace(currentChar)) {
         getNextChar(bufferOne, bufferTwo, bufferPos, usingBufferOne);
         continue;
      }
      
      if (currentChar == '\0') break;
      
      string lexeme;
      
      if (diagramWhile(bufferOne, bufferTwo, bufferPos, usingBufferOne)) {
         cout << "TOKEN: <KEYWORD, WHILE>" << endl;
      } 
      else if (diagramMain(bufferOne, bufferTwo, bufferPos, usingBufferOne)) {
         cout << "TOKEN: <KEYWORD, MAIN>" << endl;
      }
      else if (diagramVoid(bufferOne, bufferTwo, bufferPos, usingBufferOne)) {
         cout << "TOKEN: <TYPE, VOID>" << endl;
      }
      else if (diagramInt(bufferOne, bufferTwo, bufferPos, usingBufferOne)) {
         cout << "TOKEN: <TYPE, INT>" << endl;
      }
      else if (diagramChar(bufferOne, bufferTwo, bufferPos, usingBufferOne)) {
         cout << "TOKEN: <TYPE, CHAR>" << endl;
      }
      else if (diagramFloat(bufferOne, bufferTwo, bufferPos, usingBufferOne)) {
         cout << "TOKEN: <TYPE, FLOAT>" << endl;
      }
      else if (diagramOpenParen(bufferOne, bufferTwo, bufferPos, usingBufferOne)) {
         cout << "TOKEN: <SIMBOLO, (>" << endl;
      }
      else if (diagramCloseParen(bufferOne, bufferTwo, bufferPos, usingBufferOne)) {
         cout << "TOKEN: <SIMBOLO, )>" << endl;
      }
      else if (diagramOpenBrace(bufferOne, bufferTwo, bufferPos, usingBufferOne)) {
         cout << "TOKEN: <SIMBOLO, {>" << endl;
      }
      else if (diagramCloseBrace(bufferOne, bufferTwo, bufferPos, usingBufferOne)) {
         cout << "TOKEN: <SIMBOLO, }>" << endl;
      }
      else if (diagramOpenBracket(bufferOne, bufferTwo, bufferPos, usingBufferOne)) {
         cout << "TOKEN: <SIMBOLO, [>" << endl;
      }
      else if (diagramCloseBracket(bufferOne, bufferTwo, bufferPos, usingBufferOne)) {
         cout << "TOKEN: <SIMBOLO, ]>" << endl;
      }
      else if (diagramComma(bufferOne, bufferTwo, bufferPos, usingBufferOne)) {
         cout << "TOKEN: <SIMBOLO, ,>" << endl;
      }
      else if (diagramSemicolon(bufferOne, bufferTwo, bufferPos, usingBufferOne)) {
         cout << "TOKEN: <SIMBOLO, ;>" << endl;
      }
      else if (diagramAssignment(bufferOne, bufferTwo, bufferPos, usingBufferOne)) {
         cout << "TOKEN: <OPERADOR, =>" << endl;
      }
      else if (diagramGreater(bufferOne, bufferTwo, bufferPos, usingBufferOne)) {
         cout << "TOKEN: <OPERADOR, >>" << endl;
      }
      else if (diagramNumber(bufferOne, bufferTwo, bufferPos, usingBufferOne, lexeme)) {
         cout << "TOKEN: <NUMERO, " << lexeme << ">" << endl;
      }
      else if (diagramCharLiteral(bufferOne, bufferTwo, bufferPos, usingBufferOne, lexeme)) {
         cout << "TOKEN: <CHAR_LITERAL, " << lexeme << ">" << endl;
      }
      else if (diagramID(bufferOne, bufferTwo, bufferPos, usingBufferOne, lexeme)) {
         cout << "TOKEN: <ID, " << lexeme << ">" << endl;
         insertSymbol("ID", lexeme, "variavel");
      }
      else {
         int errorLine = currentLine;
         currentChar = getNextChar(bufferOne, bufferTwo, bufferPos, usingBufferOne);
         
         cout << "ERRO [Linha " << errorLine << "]: ";
         cout << "Caractere não reconhecido '" << currentChar << "'" << endl;
      }
   }
   
   delete[] bufferOne;
   delete[] bufferTwo;
}

int main() {
   try {
      code = PassFileToString();
      cout << "Codigo lido:\n" << code << endl << endl;
      lexer();
      printSymbolTable();
   } catch (const exception& e) {
      cout << e.what() << endl;
   }
   
   return 0;
}