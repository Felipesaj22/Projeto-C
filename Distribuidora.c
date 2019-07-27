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
       printf(" 4 -> GERAR RELATÓRIO:                \n");
       printf(" 5 -> SAIR:                         \n\n");
       printf("=========== OLIVIO'S DRINKS ==========\n\n");

       printf("Digite a opção desejada:\n");
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
           default: printf("Dígito inválido!\n");
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
        printf("Não foi possível abrir o arquivo.\n");
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
            printf("Código: ");
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
            printf("Preço: R$ ");
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

    cad.valor_global = 0; //Zerar valor total da venda antes de iniciar os cálculos;
    do{
        system("cls");
        printf("================= VENDA =================\n");
        printf("PRODUTO\t\tCÓDIGO\t\tQUANTIDADE\n\n");

        for(i=0; i<TAM; i++){
            if(cadastro[i].ativo==1){
                printf("%s\t\t%s\t\t%d.und\n", cadastro[i].nome, cadastro[i].codigo, cadastro[i].quantidade);
            }
        }
        printf("=========================================\n\n");
        do{
            printf("Digite o código do produto: ");
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
                        printf("Quantidade inválida!\n");
                        aux = 0;
                        system("pause");
                    }
                    else if(cad.quantidade>cadastro[i].quantidade){
                        printf("Quantidade indisponível em estoque!\n");
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

    cad.valor_global = 0; //Zerar valor total da compra antes de iniciar os cálculos;
    do{
        system("cls");
        printf("=================== COMPRA ===================\n");
        printf("PRODUTO\t\tCÓDIGO\t\tQUANTIDADE\n\n");

        for(i=0; i<TAM; i++){
            if(cadastro[i].ativo==1){
                printf("%s\t\t%s\t\t%d.und\n", cadastro[i].nome, cadastro[i].codigo, cadastro[i].quantidade);
            }
        }
        printf("==============================================\n\n");

        do{
            printf("Digite o código do produto: ");
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
                        printf("Quantidade inválida!\n");
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
    printf("============================== R E L A T Ó R I O =============================\n");
    printf("NOME\t\tCÓDIGO\t\tPREÇO\t\tQUANT\t\tVALOR\n\n");
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

int verificar_codigo(int *aux1, int i){ //Função que verifica se o código já está cadastrado;
    aux1 = 1;
    for(i=0; i<TAM; i++){
        if(strcmp(cadastro[i].codigo, cad.codigo)==0 && cadastro[i].ativo==1){
            printf("Código já cadastrado!\n");
            aux1 = 0;
            break;
        }
    }
    return (aux1);
}

int verificar_codigo_venda(int *aux1){ //Função que verifica se o código da venda é válido;
    if(cad.codigo[0]=='\0'){
        printf("Código inválido!\n");
        aux1 = 0;
    }
    else{
        aux1 = 1;
    }
    return(aux1);
}

int verificar_nome(int *aux1, int i){ //Função que verifica se o nome já está cadastrado e é válido;
    aux1 = 1;
    for(i=0; i<TAM; i++){
        if(strcmp(cadastro[i].nome, cad.nome)==0 && cadastro[i].ativo==1){
            printf("Nome já cadastrado!\n");
            aux1 = 0;
            break;
        }
    }
    if(cad.nome[0]=='\0'){
        printf("Nome inválido!\n");
        aux1 = 0;
    }
    return (aux1);
}

int verificar_preco(int *aux1){ //Função que verifica se o preço é válido;
    if(cad.preco<=0){
        printf("Preço inválido!\n");
        aux1 = 0;
    }
    else{
        aux1 = 1;
    }
    return (aux1);
}

int verificar_quantidade(int *aux1){ //Função que verifica se a quantidade é válida;
    if(cad.quantidade<=0){
        printf("Quantidade inválida!\n");
        aux1 = 0;
    }
    else{
        aux1 = 1;
    }
    return (aux1);
}

int verificar_op(int *aux1, char *opcao){ //Função que verifica se op é válida;
    if((opcao=='0') || (opcao=='1')){
        aux1 = 1;
    }
    else{
        printf("Opção inválida!\n");
        aux1 = 0;
    }
    return (aux1);
}
