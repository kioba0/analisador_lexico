#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <string.h> 
#include <stdbool.h> 

#include "AnalexProc.h" 

#define TAM_LEXEMA 50 

#define TAM_NUM 20 

void error(char msg[]) { 
    printf("%s\n", msg); 
    exit(1); 
} 

 

TOKEN AnaLex(FILE *fd) { 



    int estado; 

    char lexema[TAM_LEXEMA] = ""; 

    int tamL = 0; 

    char digitos[TAM_NUM] = ""; 

    int tamD = 0; 

    TOKEN t; 

    estado = 0; 

    while (true) { 

        char c = fgetc(fd); 

        switch (estado) { 

            case 0: 

                if (c == ' ' || c == '\t') {  //TAB ou vazio
                    estado = 0; 
                }

                //verificar:
                // else if(c == '\'') estado = 11; // Início de um char
                // else if (c == '\"') estado = 6; // Início de uma string(literal)  
                else if(c == '>') estado = 44; 
                else if(c == '<') estado = 37; 
                else if (c == '&') estado = 47; 
                else if (c == '|') estado = 48;
                // else if (c == ':') estado = 51;  //verificar
                else if (c == '=') estado = 31;
                else if (c == '!')estado = 34;
                else if (c == '/') estado = 27; 

                else if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') {    // inicio de identificador - inicializa lexema 

                    estado = 1; 

                    lexema[tamL] = c;  

                    lexema[++tamL] = '\0'; 

                } 

                else if (c == '_'){
                    estado = 2; 

                    lexema[tamL] = c;  

                    lexema[++tamL] = '\0'; 
                }

                else if (c >= '0' && c <= '9') {    // inicio de constante inteira - inicializa digitos 

                    estado = 5; 

                    digitos[tamD] = c;  

                    digitos[++tamD] = '\0';

                } 

                else if (c == '+') {    // sinal de adicao - monta e devolve token 

                    estado = 24; 

                    t.cat = SN; 

                    t.codigo = ADICAO; 

                    return t; 

                } 

                else if (c == '-') {    // sinal de subtracao - monta e devolve token 

                    estado = 25; 

                    t.cat = SN; 

                    t.codigo = SUBTRACAO; 

                    return t; 

                } 

                else if (c == '*') {    // sinal de multiplicacao - monta e devolve token 

                    estado = 26; 

                    t.cat = SN; 

                    t.codigo = MULTIPLIC; 

                    return t; 

                } 

                else if (c == '(') {    // sinal de abre parenteses - monta e devolve token 

                    estado = 14; 

                    t.cat = SN; 

                    t.codigo = ABRE_PAR; 

                    return t; 

                } 

                else if (c == ')') {    // sinal de fecha parenteses - monta e devolve token 

                    estado = 15; 

                    t.cat = SN; 

                    t.codigo = FECHA_PAR; 

                    return t; 

                } 

                else if(c == '{'){      // sinal de abre chave
                    estado = 16;
                    t.cat = SN; 
                    t.codigo = ABRE_CHAVE; 
                    return t;
                }

                else if(c == '}'){      // sinal de fecha chave
                    estado = 17;
                    t.cat = SN; 
                    t.codigo = FECHA_CHAVE; 
                    return t;
                }

                else if (c == '.') {    // sinal de ponto
                    estado = 20; 
                    t.cat = SN; 
                    t.codigo = PONTO; 
                    return t;
                } 

                else if (c == ',') {    // virgula
                    estado = 21; 
                    t.cat = SN; 
                    t.codigo = VIRGULA; 
                    return t;
                } 

                else if (c == ';') {    // ponto e virgula
                    estado = 23; 
                    t.cat = SN; 
                    t.codigo = PONTO_VIRG; 
                    return t;
                }

                else if (c == '[') {    // abre colchete
                    estado = 18; 
                    t.cat = SN; 
                    t.codigo = ABRE_COL; 
                    return t; 
                } 

                else if (c == ']') {    // fecha colchete 
                    estado = 19; 
                    t.cat = SN; 
                    t.codigo = FECHA_COL; 
                    return t; 
                } 


                // verificar se tem q por:
                else if (c == '\n') { 

                    estado = 0; 

                    t.cat = FIM_EXPR;   // fim de linha (ou expressao) encontrado 

                    contLinha++; 

                    return t; 

                } 

                else if (c == EOF) {    // fim do arquivo fonte encontrado 

                    t.cat = FIM_ARQ; 

                    return t; 

                } 

                 else

                    error("Caracter invalido na expressao!");   // sem transicao valida no AFD 

                break; 

            case 1: 
            
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')|| (c >= '0' && c <= '9') || (c == '_')) {
                    estado = 1; 
                    lexema[tamL] = c;   // acumula caracteres lidos em lexema 
                    lexema[++tamL] = '\0'; 
                } 

                else {                  // transicao OUTRO* do estado 1 do AFD 
                
                    estado = 3;     // monta token identificador e retorna 
                    ungetc(c, fd); 
                    t.cat = ID; 
                    strcpy(t.lexema, lexema); 
                    return t; 

                } 

                break; 

            case 10:
                if (c >= '0' && c <= '9') { 

                    estado = 10; 

                    digitos[tamD] = c;      // acumula digitos lidos na variaavel digitos 

                        digitos[++tamD] = '\0'; 

                } 

                else {                      // transicao OUTRO* do estado 10 do AFD 

                    estado = 11;        // monta token constante inteira e retorna 

                    ungetc(c, fd); 

                    t.cat = CT_I; 

                    t.valor_I = atoi(digitos); 

                    return t; 

                } 

        }

    } 

} 

 

int main() { 

 

    FILE *fd; 

    TOKEN tk; 

 

    if ((fd=fopen("expressao.dat", "r")) == NULL) 

        error("Arquivo de entrada da expressao nao encontrado!"); 

  

    printf("LINHA %d: ", contLinha); 



    while (1) {    // laço infinito para processar a expressão até o fim de arquivo 

        tk = AnaLex(fd); 

        switch (tk.cat) { 

            case ID: printf("<ID, %s> ", tk.lexema); 

                break; 

            case SN: 
            
                switch (tk.codigo) { 

                    case ADICAO: printf("<SN, ADICAO> "); 

                        break; 

                    case SUBTRACAO: printf("<SN, SUBTRACAO> "); 

                        break; 

                    case MULTIPLIC: printf("<SN, MULTIPLICACAO> "); 

                        break; 

                    case DIVISAO: printf("<SN, DIVISAO> "); 

                        break; 

                    case ATRIB: printf("<SN, ATRIBUICAO> "); 

                        break; 

                    case ABRE_PAR: printf("<SN, ABRE_PARENTESES> "); 

                        break; 

                    case FECHA_PAR: printf("<SN, FECHA_PARENTESES> "); 

                        break; 

                } 

                break; 

            case CT_I: printf("<CT_I, %d> ", tk.valor_I); 

                break; 

            case FIM_EXPR: printf("<FIM_EXPR, %d>\n", 0); 

                printf("LINHA %d: ", contLinha); 

                break;

            case FIM_ARQ: printf(" <Fim do arquivo encontrado>\n"); 

        } 

        if (tk.cat == FIM_ARQ) break; 

    } 

 

    fclose(fd); 

    return 0; 

} 