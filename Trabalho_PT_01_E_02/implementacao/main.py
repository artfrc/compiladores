from enum import Enum
from tabulate import tabulate
 
class TokenType(Enum):
    IDENTIFIER = "IDENTIFIER"
    NUMBER = "NUMBER"
    KEYWORD = "KEYWORD"
    OPERATOR = "OPERATOR"
    SEPARATOR = "SEPARATOR"
    COMMENT = "COMMENT"
    STRING = "STRING"
    EOF = "EOF"
    ERROR = "ERROR"
 
class Token:
    def __init__(self, token_type, lexeme, line, column, attribute=None):
        self.token_type = token_type
        self.lexeme = lexeme
        self.line = line
        self.column = column
        self.attribute = attribute
 
class SymbolTable:
    def __init__(self):
        self.table = {}
 
    def add(self, lexeme, token_type, data_type=None):
        if lexeme not in self.table:
            self.table[lexeme] = {
                "token": token_type,
                "lexeme": lexeme,
                "data_type": data_type
            }
 
    def to_list(self):
        return [[v["token"].value, v["lexeme"], v["data_type"]] for v in self.table.values()]
 
class Lexer:
    def __init__(self, source_code):
        self.source = source_code
        self.position = 0
        self.line = 1
        self.column = 1
        self.symbol_table = SymbolTable()
 
        self.keywords = {"if", "else", "while", "return"}
        self.operators = {"+", "-", "*", "/", "=", "=="}
        self.separators = {"(", ")", "{", "}", ";", ","}
 
    def next_char(self):
        if self.position >= len(self.source):
            return None
        ch = self.source[self.position]
        self.position += 1
        if ch == "\n":
            self.line += 1
            self.column = 1
        else:
            self.column += 1
        return ch
 
    def peek_char(self):
        if self.position >= len(self.source):
            return None
        return self.source[self.position]
 
    def get_token(self):
        ch = self.next_char()
        if ch is None:
            return Token(TokenType.EOF, "EOF", self.line, self.column)
 
        if ch.isspace():
            return self.get_token()
 
        if ch == "/" and self.peek_char() == "/":
            while ch not in {None, "\n"}:
                ch = self.next_char()
            return Token(TokenType.COMMENT, "//", self.line, self.column)
 
        if ch == "/" and self.peek_char() == "*":
            self.next_char()
            while True:
                ch = self.next_char()
                if ch is None:
                    return Token(TokenType.ERROR, "/*", self.line, self.column, attribute="Comentário não fechado")
                if ch == "*" and self.peek_char() == "/":
                    self.next_char()
                    break
            return Token(TokenType.COMMENT, "/*...*/", self.line, self.column)
 
        if ch == '"':
            start_col = self.column
            lexeme = ""
            while True:
                ch = self.next_char()
                if ch is None or ch == "\n":
                    return Token(TokenType.ERROR, lexeme, self.line, start_col, attribute="String não fechada")
                if ch == '"':
                    break
                lexeme += ch
            self.symbol_table.add(lexeme, TokenType.STRING, "string")
            return Token(TokenType.STRING, lexeme, self.line, start_col)
 
        if ch.isalpha() or ch == "_":
            start_col = self.column
            lexeme = ch
            while self.peek_char() and (self.peek_char().isalnum() or self.peek_char() == "_"):
                lexeme += self.next_char()
            if lexeme in self.keywords:
                return Token(TokenType.KEYWORD, lexeme, self.line, start_col)
            else:
                self.symbol_table.add(lexeme, TokenType.IDENTIFIER)
                return Token(TokenType.IDENTIFIER, lexeme, self.line, start_col)
 
        if ch.isdigit():
            start_col = self.column
            lexeme = ch
            while self.peek_char() and self.peek_char().isdigit():
                lexeme += self.next_char()
            self.symbol_table.add(lexeme, TokenType.NUMBER, "int")
            return Token(TokenType.NUMBER, lexeme, self.line, start_col, attribute=int(lexeme))
 
        if ch in self.operators:
            start_col = self.column
            lexeme = ch
            if ch == "=" and self.peek_char() == "=":
                lexeme += self.next_char()
            return Token(TokenType.OPERATOR, lexeme, self.line, start_col)
 
        if ch in self.separators:
            return Token(TokenType.SEPARATOR, ch, self.line, self.column)
 
        return Token(TokenType.ERROR, ch, self.line, self.column, attribute="Caractere inválido")
 
if __name__ == "__main__":
    with open("file.txt", "r", encoding="utf-8") as f:
        code = f.read()
 
    lexer = Lexer(code)
 
    tokens = []
    while True:
        token = lexer.get_token()
        tokens.append([
            token.token_type.value,
            token.lexeme,
            token.line,
            token.column,
            token.attribute
        ])
        if token.token_type == TokenType.EOF:
            break
 
    headers = ["Token", "Lexema", "Linha", "Coluna", "Atributo"]
    print(tabulate(tokens, headers=headers, tablefmt="grid"))
 
    print("\nTabela de Símbolos:")
    print(tabulate(lexer.symbol_table.to_list(), headers=["Token", "Lexema", "Tipo"], tablefmt="grid"))