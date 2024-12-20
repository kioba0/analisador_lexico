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

int verificarReservadas(char reservadas[][28] , char string[]){
    for(int i = 0 ; i < 28 ; i++){
        if(strcmp(reservadas[i] , string) == 0){
          return i;
        }
    }

    return -1;
}

TOKEN AnaLex(FILE *fd) { 

    char reservadas[28][28] = {"const", "pr", "init", "endp", "char", "int", "real",  "bool", "do", "while", "endw", "var", "from", "dt", "to", "by", "endv", "if", "elif", "else", "endi", "getint", "getout", "getchar", "getreal", "putint", "putchar", "putreal"};

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

                else if (c == '\n') { 

                    estado = 0; 

                    t.cat = FIM_EXPR;   // fim de linha (ou expressao) encontrado 

                    contLinha++; 

                    return t; 

                }

                else if(c == '\'') {
                    estado = 9; // Início de um char
                }
                
                else if (c == '\"') {
                    estado = 11; // Início de uma string(literal)
                }  
                
                else if(c == '>') estado = 44; 
                else if(c == '<') estado = 37; 
                else if (c == '&') estado = 47; 
                else if (c == '|') estado = 48;
                else if (c == '=') estado = 31;
                else if (c == '!') estado = 34;
                else if (c == '/') estado = 27; 

                else if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') {    // inicio de identificador - inicializa lexema 

                    estado = 1; 

                    lexema[tamL] = c;  

                    lexema[++tamL] = '\0'; 

                } 

                else if (c == '_'){     // inicio de identificador - inicializa lexema
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

                else if (c == ',') {    // virgula
                    estado = 21; 
                    t.cat = SN; 
                    t.codigo = VIRGULA; 
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

                else if (c == EOF) {    // fim do arquivo fonte encontrado 

                    t.cat = FIM_ARQ; 

                    return t; 

                } 

                else{
                    error("Caracter invalido na expressao!");   // sem transicao valida no AFD 

                }

                break; 

            case 1: // ID
            
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')|| (c >= '0' && c <= '9') || (c == '_')) {
                    estado = 1; 
                    lexema[tamL] = c;   // acumula caracteres lidos em lexema 
                    lexema[++tamL] = '\0'; 
                } 

                else {                  // transicao OUTRO* do estado 1 do AFD 

                    estado = 3;     // monta token identificador e retorna 
                    ungetc(c, fd); 
                    // verificar id ou pr
                    if((verificarReservadas(reservadas, lexema) != -1)){
                        t.cat = PVR;
                    } else{
                        t.cat = ID; 
                    }
                    
                    strcpy(t.lexema, lexema); 
                    return t; 

                } 

                break; 

            case 2: // ID

                if (c == '_') {
                    estado = 2;
                    lexema[tamL] = c;   // acumula caracteres lidos em lexema 
                    lexema[++tamL] = '\0'; 
                }

                else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
                    estado = 1;
                    lexema[tamL] = c;   // acumula caracteres lidos em lexema 
                    lexema[++tamL] = '\0'; 
                }

                else{
                    error("Caracter invalido na expressao!");   // sem transicao valida no AFD 

                }

                break;
   
            case 4: 

                if (c >= '0' && c <= '9') {    
                    estado = 7; 
                    digitos[tamD] = c;  
                    digitos[++tamD] = '\0';
                }
                
                else{
                    error("Caracter invalido na expressao!");   // sem transicao valida no AFD 

                }

                break;

            case 5:
                if (c >= '0' && c <= '9') {    
                    estado = 5; 
                    digitos[tamD] = c;  
                    digitos[++tamD] = '\0';
                }

                else if (c == '.'){
                    estado = 4; 
                    digitos[tamD] = c;  
                    digitos[++tamD] = '\0';
                }

                else {                 // transicao OUTRO* do estado 5 do AFD 

                    estado = 6;        // monta token constante inteira e retorna 
                    ungetc(c, fd); 
                    t.cat = CT_I; 
                    t.valor_I = atoi(digitos); 
                    return t; 

                }

                break;

            case 7:
                if (c >= '0' && c <= '9') {    
                    estado = 7; 
                    digitos[tamD] = c;  
                    digitos[++tamD] = '\0';
                }
                
                else {                 // transicao OUTRO* do estado 7 do AFD 

                    estado = 6;        // monta token constante inteira e retorna 
                    ungetc(c, fd); 
                    t.cat = CT_R; 
                    t.valor_R = atof(digitos); 
                    return t; 

                }

                break;

            case 9:
                if (c == '\\'){
                    estado = 43;
                    lexema[tamL] = c;   // acumula caracteres lidos em lexema 
                    lexema[++tamL] = '\0'; 
                }
                else if((isprint(c) != 0) && (c != '\"') && (c!= '\n')){
                    estado = 40;
                    lexema[tamL] = c;   // acumula caracteres lidos em lexema 
                    lexema[++tamL] = '\0'; 
                }

                else {
                    error("Caracter invalido na expressao!");
                }
                
                break;

            // case 9:
            //     if (c == '\n'){
            //         estado = 42;
            //         lexema[tamL] = c;   // acumula caracteres lidos em lexema 
            //         lexema[++tamL] = '\0'; 
            //     }
            //     else if (c == '\0'){
            //         estado = 41;
            //         lexema[tamL] = c;   // acumula caracteres lidos em lexema 
            //         lexema[++tamL] = '\0'; 
            //     }
            //     else if((isprint(c) != 0) && (c != '\"') && (c!= '\n')){
            //         estado = 40;
            //         lexema[tamL] = c;   // acumula caracteres lidos em lexema 
            //         lexema[++tamL] = '\0'; 
            //     }

            //     else {
            //         error("Caracter invalido na expressao!");
            //     }
                
            //     break;

            case 11:
                if (c == '\"'){
                    estado = 12;
                    
                }
                
                else if((isprint(c) != 0) && (c != '\"') && (c!= '\n')){             
                    estado = 11;   
                    lexema[tamL] = c;   // acumula caracteres lidos em lexema 
                    lexema[++tamL] = '\0'; 
                    if (tamL > TAM_LEXEMA){
                        error("Tamanho maior que o máximo.");
                    }
                    
                }
                
                else {
                    error("Caracter invalido na expressao!");
                }

                break;

            case 12:
                estado = 13;
                ungetc(c, fd);
                t.cat = LT;
                strcpy(t.lexema, lexema);
                return t;


            case 27:    //comentario ou divisão
                if (c == '/'){
                    estado = 29;
                    
                }

                else{               // transicao OUTRO* do estado 27 do AFD 
                    estado = 28;
                    ungetc(c, fd);
                    t.cat = SN; 
                    t.codigo = DIVISAO;
                    return t;
                }

                break;

            case 29:    //comentario
                if (c == '\n'){
                    estado = 30;
                    contLinha++;
                }

                else if(c == EOF){
                    t.cat = FIM_ARQ;
                    return t;
                }

                break;

            case 30:    //comentario volta pro 0        // transicao OUTRO* do estado 30 do AFD 
                estado = 0;
                ungetc(c, fd);
                
                break;

            case 31:    //atribuição ou comparação
                if (c == '='){
                    estado = 33;
                    t.cat = SN; 
                    t.codigo = COMPARACAO;
                    return t; 
                }

                else{               // transicao OUTRO* do estado 31 do AFD 
                    estado = 32;
                    ungetc(c, fd);
                    t.cat = SN; 
                    t.codigo = ATRIB;
                    return t;
                }

                break;
            case 34:    //diferente ou negação
                if (c == '='){
                    estado = 35;
                    t.cat = SN; 
                    t.codigo = DIFERENTE;
                    return t; 
                }

                else{               // transicao OUTRO* do estado 34 do AFD 
                    estado = 36;
                    ungetc(c, fd);
                    t.cat = SN; 
                    t.codigo = NEGACAO;
                    return t;
                }

                break;
            case 37:    //menor ou menor igual
                if (c == '='){
                    estado = 39;
                    t.cat = SN; 
                    t.codigo = MENORIGUAL;
                    return t; 
                }

                else{               // transicao OUTRO* do estado 37 do AFD 
                    estado = 38;
                    ungetc(c, fd);
                    t.cat = SN; 
                    t.codigo = MENOR;
                    return t;
                }

                break;

            //verificar charcon
            case 40:
                if (c == '\''){
                    estado = 10;
                    t.cat = CT_C;
                    t.c = lexema[--tamL];   
                    return t; 

                    // strcpy(t.c, lexema);
                    // return t;
                }

                else {
                    error("Caracter invalido na expressao!");
                }
                
                break;
            
            case 41:
                if (c == '\''){
                    estado = 10;
                    t.cat = CT_C;
                    t.c = '\0';

                    // t.c = lexema[--tamL];
                    // strcpy(t.c, lexema);
                    return t; 
                }

                else {
                    error("Caracter invalido na expressao!");
                }
                
                break;

            case 42:
                if (c == '\''){
                    estado = 10;
                    t.cat = CT_C;
                    t.c = '\n';
                    
                    // t.c = lexema[--tamL];
                    // strcpy(t.c, lexema);
                    return t; 
                }

                else {
                    error("Caracter invalido na expressao!");
                }
                
                break;
            case 43: 
                if (c == 'n'){
                    estado = 42;
                    lexema[tamL] = c;   // acumula caracteres lidos em lexema 
                    lexema[++tamL] = '\0'; 
                }
                else if(c == '0'){
                    estado = 41;
                    lexema[tamL] = c;   // acumula caracteres lidos em lexema 
                    lexema[++tamL] = '\0'; 
                }

                else {
                    error("Caracter invalido na expressao!");
                }

                break;

            case 44:    //maior ou maior igual
                if (c == '='){
                    estado = 46;
                    t.cat = SN; 
                    t.codigo = MAIORIGUAL;
                    return t; 
                }

                else{                   // transicao OUTRO* do estado 45 do AFD 
                    estado = 45;
                    ungetc(c, fd);
                    t.cat = SN; 
                    t.codigo = MAIOR;
                    return t;
                }

                break;
                
            case 47:    //endereço ou AND
                if (c == '&'){
                    estado = 51;
                    t.cat = SN; 
                    t.codigo = AND;
                    return t; 
                }

                else{                   // transicao OUTRO* do estado 47 do AFD 
                    estado = 50;
                    ungetc(c, fd);
                    t.cat = SN; 
                    t.codigo = ENDERECO;
                    return t;
                }

                break;

            case 48:    //OR
                if(c == '|'){
                    estado = 49;
                    t.cat = SN; 
                    t.codigo = OR;
                    return t; 
                }
        
                else {
                    error("Caracter invalido na expressao!");
                }

                break;

        }

    } 

} 

 

int main() { 

    FILE *fd; 

    TOKEN tk; 

 

    if ((fd=fopen("teste.txt", "r")) == NULL) 

        error("Erro ao abrir o arquivo"); 

  

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

                    case COMPARACAO: 
                        printf("<SN, COMPARACAO> "); 
                        break; 

                    case ABRE_CHAVE: 
                        printf("<SN, ABRE_CHAVE> "); 
                        break; 

                    case FECHA_CHAVE: 
                        printf("<SN, FECHA_CHAVE> "); 
                        break; 

                    case ABRE_COL: 
                        printf("<SN, ABRE_COL> "); 
                        break; 

                    case FECHA_COL: 
                        printf("<SN, FECHA_COL> "); 
                        break;

                    case NEGACAO: 
                        printf("<SN, NEGACAO> "); 
                        break;

                    case DIFERENTE:
                        printf("<SN, DIFERENTE> "); 
                        break;
                    
                    case VIRGULA:
                        printf("<SN, VIRGULA> "); 
                        break;

                    case MAIOR:
                        printf("<SN, MAIOR> "); 
                        break;
                    
                    case MENOR:
                        printf("<SN, MENOR> "); 
                        break;
                    
                    case MAIORIGUAL:
                        printf("<SN, MAIORIGUAL> "); 
                        break;
                    
                    case MENORIGUAL:
                        printf("<SN, MENORIGUAL> "); 
                        break;
                        
                    case AND:
                        printf("<SN, AND> "); 
                        break;

                    case OR:
                        printf("<SN, OR> "); 
                        break;

                    case ENDERECO:
                        printf("<SN, ENDERECO> "); 
                        break;
                } 

                break; 

            case CT_I: 
                printf("<CT_I, %d> " , tk.valor_I); 
                break; 

            case CT_R:
                printf("<CT_R, %f> ", tk.valor_R);
                break ;

            case CT_C:
                printf("<CT_C, %c> ", tk.c);
                break ;

            case LT:
                printf("<LT, %s> ", tk.lexema);
                break ;

            case PVR:
                printf("<PVR, %s> ", tk.lexema);
                break;

            case FIM_EXPR: printf("<FIM_EXPR, %d>\n", 0); 

                printf("\nLINHA %d: ", contLinha); 

                break;

            case FIM_ARQ: printf("<Fim do arquivo encontrado>\n "); 
                break;

        } 

        if (tk.cat == FIM_ARQ) break; 

    } 

 

    fclose(fd); 

    return 0; 

} 