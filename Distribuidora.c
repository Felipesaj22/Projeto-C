#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#define TAM 100

typedef struct{
    char nome[20];
    char codigo[10];
    float preco, valor, valor_global;
    int quantidade, ativo;
}Estrutura;

Estrutura cadastro[TAM], cad;

void menu();
void ler_arquivo();
void salvar_arquivo();
void cadastrar();
void vender();
void comprar();
void relatorio();
int verificar_codigo();
int verificar_codigo_venda();
int verificar_nome();
int verificar_preco();
int verificar_quantidade();
int verificar_op();

int main(){
    setlocale(LC_ALL, "Portuguese");

    ler_arquivo();
    menu();
    salvar_arquivo();
return 0;
}

void menu(){
    int op;

    do{
       system("cls");
       printf("================ MENU ================\n\n");
       printf(" 1 -> CADASTRAR BEBIDA:               \n");
       printf(" 2 -> VENDER BEBIDA:                  \n");
       printf(" 3 -> COMPRAR BEBIDA:                 \n");
       printf(" 4 -> GERAR RELAT�RIO:                \n");
       printf(" 5 -> SAIR:                         \n\n");
       printf("=========== OLIVIO'S DRINKS ==========\n\n");

       printf("Digite a op��o desejada:\n");
       scanf("%d", &op);
       switch(op){
           case 1:
               salvar_arquivo();
               cadastrar();
           break;
           case 2:
               salvar_arquivo();
               vender();
           break;
           case 3:
               salvar_arquivo();
               comprar();
           break;
           case 4: relatorio();
           break;
           case 5: system("cls");
           break;
           default: printf("D�gito inv�lido!\n");
                    system("pause");
           break;
       }
    }while(op!=5);
}

void ler_arquivo(){
    FILE *arquivo = fopen("Dados.bin", "r+b");
    if(arquivo!=NULL){
        fread(cadastro, sizeof(Estrutura), TAM, arquivo);
        fclose(arquivo);
    }
    else{
        printf("N�o foi poss�vel abrir o arquivo.\n");
        system("pause");
    }
}

void salvar_arquivo(){
    FILE *arquivo = fopen("Dados.bin", "wb");
    fwrite(cadastro, sizeof(Estrutura), TAM, arquivo);
    fclose(arquivo);
}

void cadastrar(){
    int i, aux;
    char op;
    do{
        system("cls");
        printf("=============== CADASTRO ===============\n\n");

        do{
            printf("C�digo: ");
            fflush(stdin);
            fgets(cad.codigo, 10, stdin);
            cad.codigo[strlen(cad.codigo)-1] = '\0';
            aux = verificar_codigo_venda(&aux) + verificar_codigo(&aux);
        }while(aux!=2);

        do{
            printf("Nome: ");
            fflush(stdin);
            fgets(cad.nome, 20, stdin);
            cad.nome[strlen(cad.nome)-1] = '\0';
            aux = verificar_nome(&aux);
        }while(aux!=1);

        do{
            printf("Pre�o: R$ ");
            scanf("%f", &cad.preco);
            aux = verificar_preco(&aux);
        }while(aux!=1);

        do{
            printf("Quantidade: ");
            fflush(stdin);
            scanf("%d", &cad.quantidade);
            aux = verificar_quantidade(&aux);
        }while(aux!=1);

        for(i=0; i<TAM; i++){
            if(cadastro[i].ativo!=1){
                strcpy(cadastro[i].nome, cad.nome);
                strcpy(cadastro[i].codigo, cad.codigo);
                cadastro[i].quantidade = cad.quantidade;
                cadastro[i].preco = cad.preco;
                cadastro[i].ativo = 1;

                cadastro[i].valor = cadastro[i].preco * cadastro[i].quantidade; //valor corrente de cada bebida;
                cadastro[1].valor_global = cadastro[1].valor_global + cadastro[i].valor; //valor global do estoque geral;
                break;
            }
        }

        do{
            fflush(stdin);
            printf("Digite 1 para continuar ou 0 para sair: ");
            scanf("%c", &op);
            aux = verificar_op(&aux, op);
        }while(aux!=1);

    }while(op=='1');
}

void vender(){
    int i, aux;
    char op;

    cad.valor_global = 0; //Zerar valor total da venda antes de iniciar os c�lculos;
    do{
        system("cls");
        printf("================= VENDA =================\n");
        printf("PRODUTO\t\tC�DIGO\t\tQUANTIDADE\n\n");

        for(i=0; i<TAM; i++){
            if(cadastro[i].ativo==1){
                printf("%s\t\t%s\t\t%d.und\n", cadastro[i].nome, cadastro[i].codigo, cadastro[i].quantidade);
            }
        }
        printf("=========================================\n\n");
        do{
            printf("Digite o c�digo do produto: ");
            fflush(stdin);
            fgets(cad.codigo, 10, stdin);
            cad.codigo[strlen(cad.codigo)-1] = '\0';
            aux = verificar_codigo_venda(&aux);
        }while(aux!=1);

        do{
            for(i=0; i<TAM; i++){
                if(strcmp(cadastro[i].codigo, cad.codigo)==0 && cadastro[i].ativo==1){
                    printf("Digite a quantidade que deseja vender: ");
                    scanf("%d", &cad.quantidade);
                    if(cad.quantidade<=cadastro[i].quantidade && cad.quantidade>0){
                        cad.valor = cadastro[i].preco * cad.quantidade; //valor da venda do produto;
                        cad.valor_global = cad.valor_global + cad.valor; //valor total da venda;
                        cadastro[i].quantidade = cadastro[i].quantidade - cad.quantidade; //diminui do estoque a quantidade vendida;
                        cadastro[i].valor = cadastro[i].valor - cad.valor; //diminui do estoque o valor corrente do produto;
                        cadastro[1].valor_global = cadastro[1].valor_global - cad.valor; //diminui do estoque o valor global do produto;
                        aux = 1;
                        break;
                    }
                    else if(cad.quantidade<=0){
                        printf("Quantidade inv�lida!\n");
                        aux = 0;
                        system("pause");
                    }
                    else if(cad.quantidade>cadastro[i].quantidade){
                        printf("Quantidade indispon�vel em estoque!\n");
                        aux = 0;
                        system("pause");
                    }
                }
            }

        }while(aux!=1);

        do{
            printf("Digite 1 para continuar ou 0 para finalizar venda: ");
            fflush(stdin);
            scanf("%c", &op);
            aux = verificar_op(&aux, op);
        }while(aux!=1);

        if(op=='0'){
            printf("Valor da venda: R$ %.2f\n", cad.valor_global);
            cad.valor_global = 0; //Zerar o valor global da venda;
            system("pause");
        }
    }while(op=='1');
}

void comprar(){
    int aux, i;
    char op;

    cad.valor_global = 0; //Zerar valor total da compra antes de iniciar os c�lculos;
    do{
        system("cls");
        printf("=================== COMPRA ===================\n");
        printf("PRODUTO\t\tC�DIGO\t\tQUANTIDADE\n\n");

        for(i=0; i<TAM; i++){
            if(cadastro[i].ativo==1){
                printf("%s\t\t%s\t\t%d.und\n", cadastro[i].nome, cadastro[i].codigo, cadastro[i].quantidade);
            }
        }
        printf("==============================================\n\n");

        do{
            printf("Digite o c�digo do produto: ");
            fflush(stdin);
            fgets(cad.codigo, 10, stdin);
            cad.codigo[strlen(cad.codigo)-1] = '\0';
            aux = verificar_codigo_venda(&aux);
        }while(aux!=1);

        do{
            for(i=0 ;i<TAM; i++){
                if(strcmp(cadastro[i].codigo, cad.codigo)==0 && cadastro[i].ativo==1){
                    printf("Digite a quantidade que deseja comprar: ");
                    scanf("%d", &cad.quantidade);
                    if(cad.quantidade>0){
                        cad.valor = cadastro[i].preco * cad.quantidade; //valor da compra do produto;
                        cad.valor_global = cad.valor_global + cad.valor; //valor total da compra;
                        cadastro[i].quantidade = cadastro[i].quantidade + cad.quantidade; //diminui do estoque a quantidade vendida;
                        cadastro[i].valor = cadastro[i].valor + cad.valor; //aumenta do estoque o valor corrente do produto;
                        cadastro[1].valor_global = cadastro[1].valor_global + cad.valor; //aumenta do estoque o valor global do produto;
                        aux = 1;
                        break;
                    }
                    else if(cad.quantidade<=0){
                        printf("Quantidade inv�lida!\n");
                        aux = 0;
                        system("pause");
                    }
                }
            }
        }while(aux!=1);

        do{
            printf("Digite 1 para continuar ou 0 para finalizar compra: ");
            fflush(stdin);
            scanf("%c", &op);
            aux = verificar_op(&aux, op);
        }while(aux!=1);

        if(op=='0'){
            printf("Valor da compra: R$ %.2f\n", cad.valor_global);
            cad.valor_global = 0; //Zerar valor total da compra;
            system("pause");
        }
    }while(op=='1');
}

void relatorio(){
    int i;

    system("cls");
    printf("============================== R E L A T � R I O =============================\n");
    printf("NOME\t\tC�DIGO\t\tPRE�O\t\tQUANT\t\tVALOR\n\n");
    for(i=0; i<TAM; i++){
        if(cadastro[i].ativo==1){
            printf("%s\t\t%s\t\tR$%.2f\t\t%d.und\t\tR$%.2f\n", cadastro[i].nome, cadastro[i].codigo, cadastro[i].preco, cadastro[i].quantidade, cadastro[i].valor);
        }
    }
    printf("\n==============================================================================\n");
    printf("VALOR GLOBAL DO ESTOQUE: R$ %.2f\n", cadastro[1].valor_global);
    printf("==============================================================================\n");
    system("pause");
}

int verificar_codigo(int *aux1, int i){ //Fun��o que verifica se o c�digo j� est� cadastrado;
    aux1 = 1;
    for(i=0; i<TAM; i++){
        if(strcmp(cadastro[i].codigo, cad.codigo)==0 && cadastro[i].ativo==1){
            printf("C�digo j� cadastrado!\n");
            aux1 = 0;
            break;
        }
    }
    return (aux1);
}

int verificar_codigo_venda(int *aux1){ //Fun��o que verifica se o c�digo da venda � v�lido;
    if(cad.codigo[0]=='\0'){
        printf("C�digo inv�lido!\n");
        aux1 = 0;
    }
    else{
        aux1 = 1;
    }
    return(aux1);
}

int verificar_nome(int *aux1, int i){ //Fun��o que verifica se o nome j� est� cadastrado e � v�lido;
    aux1 = 1;
    for(i=0; i<TAM; i++){
        if(strcmp(cadastro[i].nome, cad.nome)==0 && cadastro[i].ativo==1){
            printf("Nome j� cadastrado!\n");
            aux1 = 0;
            break;
        }
    }
    if(cad.nome[0]=='\0'){
        printf("Nome inv�lido!\n");
        aux1 = 0;
    }
    return (aux1);
}

int verificar_preco(int *aux1){ //Fun��o que verifica se o pre�o � v�lido;
    if(cad.preco<=0){
        printf("Pre�o inv�lido!\n");
        aux1 = 0;
    }
    else{
        aux1 = 1;
    }
    return (aux1);
}

int verificar_quantidade(int *aux1){ //Fun��o que verifica se a quantidade � v�lida;
    if(cad.quantidade<=0){
        printf("Quantidade inv�lida!\n");
        aux1 = 0;
    }
    else{
        aux1 = 1;
    }
    return (aux1);
}

int verificar_op(int *aux1, char *opcao){ //Fun��o que verifica se op � v�lida;
    if((opcao=='0') || (opcao=='1')){
        aux1 = 1;
    }
    else{
        printf("Op��o inv�lida!\n");
        aux1 = 0;
    }
    return (aux1);
}
