#include "lexer.h" 
#include "exp.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_LEXEMAS 1000
#define TAM_LEXEMA 100

char tabela_lexemas[MAX_LEXEMAS][TAM_LEXEMA];
int total_lexemas = 0;

YY_BUFFER_STATE buffer; 

int adiciona_lexema(const char *lex) {
    if (total_lexemas < MAX_LEXEMAS) {
        strcpy(tabela_lexemas[total_lexemas], lex);
        return total_lexemas++;
    }
    return -1;
}

void inicializa_arquivo(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erro: Não foi possível abrir o arquivo '%s'\n", filename);
        exit(1);
    }
    yyin = file;
}

void inicializa_string(char *str) { 
   buffer = yy_scan_string(str); 
}

Token *proximo_token() { 
   return yylex(); 
}

void imprime_token(Token *tok) {
   if(!tok) return;

   switch (tok->tipo) {
        case TOK_NUM:
            printf("<NUM, %d>\n", tok->atributo);
            break;

        case TOK_OP:
            switch (tok->atributo) {
                case SOMA: printf("<OP, +>\n"); break;
                case SUB:  printf("<OP, ->\n"); break;
                case MULT: printf("<OP, *>\n"); break;
                case DIV:  printf("<OP, />\n"); break;
            }
            break;

        case TOK_PONT:
            switch (tok->atributo) {
                case PARESQ: printf("<PONT, (>\n"); break;
                case PARDIR: printf("<PONT, )>\n"); break;
            }
            break;

        case TOK_NUM_FLOAT:
            printf("<NUM_FLOAT, %s>\n", tabela_lexemas[tok->atributo]);
            break;

        case TOK_ID:
            printf("<ID, %s>\n", tabela_lexemas[tok->atributo]);
            break;

        case TOK_RELOP:
            printf("<RELOP, %s>\n", tabela_lexemas[tok->atributo]);
            break;

        case TOK_IF:
            printf("<IF>\n");
            break;

        case TOK_WHILE:
            printf("<WHILE>\n");
            break;

        case TOK_THEN:
            printf("<THEN>\n");
            break;

        case TOK_ELSE:
            printf("<ELSE>\n");
            break;

        case TOK_REPEAT:
            printf("<REPEAT>\n");
            break;

        case TOK_UNTIL:
            printf("<UNTIL>\n");
            break;

        case TOK_EOF:
            printf("<EOF>\n");
            break;

        case TOK_ERRO:
            printf("<ERRO>\n");
            break;
   }
}

void analisar_arquivo(const char *filename) {
    printf("\n>>> Analisando arquivo: %s\n", filename);
    printf(">>> Tokens encontrados:\n\n");
    
    inicializa_arquivo(filename);
    
    Token *tok = proximo_token();
    int count = 0;
    
    while (tok != NULL && tok->tipo != TOK_EOF) {
        printf("%3d: ", ++count);
        imprime_token(tok);
        tok = proximo_token();
    }
    
    if (tok && tok->tipo == TOK_EOF) {
        printf("%3d: ", ++count);
        imprime_token(tok);
    }
    
    fclose(yyin);
    printf("\n>>> Total de tokens: %d\n", count);
    printf(">>> Fim da análise léxica\n\n");
}

void analisar_string() {
    char entrada[1000];
    printf("\n>>> Digite a expressão (máx 1000 caracteres):\n");
    printf(">>> ");
    fgets(entrada, 1000, stdin);
    
    entrada[strcspn(entrada, "\n")] = 0;
    
    printf("\n>>> Tokens encontrados:\n\n");
    inicializa_string(entrada);
    
    Token *tok = proximo_token();
    int count = 0;
    
    while (tok != NULL && tok->tipo != TOK_EOF) {
        printf("%3d: ", ++count);
        imprime_token(tok);
        tok = proximo_token();
    }
    
    if (tok && tok->tipo == TOK_EOF) {
        printf("%3d: ", ++count);
        imprime_token(tok);
    }
    
    yy_delete_buffer(buffer);
    printf("\n>>> Total de tokens: %d\n", count);
    printf(">>> Fim da análise léxica\n\n");
}

int main(int argc, char **argv) { 
    int opcao;
    
    printf("\n\n>>> ANALISADOR LÉXICO - COMPILADORES\n");
    
    while (1) {
        printf("\nOpções:\n");
        printf("1 - Analisar arquivo\n");
        printf("2 - Analisar expressão digitada\n");
        printf("3 - Sair\n");
        printf("Escolha: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida!\n");
            while (getchar() != '\n'); 
            continue;
        }
        getchar(); 
        
        switch (opcao) {
            case 1: {
                char filename[100];
                printf("Digite o nome do arquivo: ");
                fgets(filename, 100, stdin);
                filename[strcspn(filename, "\n")] = 0; 
                
                analisar_arquivo(filename);
                break;
            }
            case 2:
                analisar_string();
                break;
            case 3:
                printf("Encerrando programa...\n");
                return 0;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    }
    
    return 0; 
}