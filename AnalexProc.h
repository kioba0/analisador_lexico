#ifndef ANALEX

#define ANALEX 

#define TAM_MAX_LEXEMA 31 

enum TOKEN_CAT 
{
    ID=1, 
    SN,
    PR, 
    CT_I, 
    CT_R,
    CT_C,
    LT,
    FIM_EXPR,  //verificar
    FIM_ARQ
}; 

/* Onde: ID: Identificador, SN: Sinal; CT_I: Constante numérica inteira; CT_R: Constante numérica real; CT_C: ; LT: ; */ 

enum PAL_RESERV
{
    CONST, 
    // PR, 
    INIT, 
    ENDP, 
    CHAR, 
    INT, 
    REAL, 
    BOOL, 
    DO, 
    WHILE,
    ENDW, 
    VAR, 
    FROM, 
    TO,
    BY,
    ENDV,
    IF,
    ELIF,
    ELSE,
    ENDI,
    GETINT,
    GETCHAR,
    GETREAL,
    PUTINT,
    PUTCHAR,
    PUTREAL
};

enum SINAIS 
{
    ATRIB = 1, 
    ADICAO, 
    SUBTRACAO, 
    MULTIPLIC, 
    DIVISAO, 
    COMPARACAO,
    ABRE_PAR,
    FECHA_PAR,
    ABRE_CHAVE,
    FECHA_CHAVE,
    ABRE_COL,
    FECHA_COL,
    NEGACAO,
    DIFERENTE,
    VIRGULA,
    MAIOR,
    MENOR,
    MAIORIGUAL,
    MENORIGUAL,
    AND,
    OR,
    ENDERECO
}; // Sinais válidos da linguagem 

 

typedef 

    struct {  

        enum TOKEN_CAT cat;     // deve receber uma das constantes de enum TOKEN_CAT 

        union {     // parte variável do registro 

            int codigo;  // para tokens das categorias SN 

            char lexema[TAM_MAX_LEXEMA];  // cadeia de caracteres que corresponde ao nome do token da cat. ID 

            int valor_I; // valor da constante inteira em tokens da cat. CT_I 

            float valor_R; // valor da constante real em tokens da cat. CT_I 

            char c;

        }; 

    } TOKEN;    // Tipo TOKEN 

 

#endif 

/* Contador de linhas do código fonte */ 

int contLinha = 1; 