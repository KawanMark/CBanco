#include "biblioteca.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char nome_do_arquivo[20] = "lista_de_clientes";

void limpa(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int verificaCPF(int tam, Cliente *clientes, char *compara){

    int aux = 0;
    int ret;
    for (int i = 0; i < tam; i++) {
        ret = strncmp(compara, clientes[i].cpf, 11);
        if(ret == 0){
            aux = 1;
            return aux;
        }
    }
    return aux;
}

int cpfExiste(int tam, Cliente *clientes, const char *cpf) {
    for (int i = 0; i < tam; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0) {
            return 1; // CPF encontrado
        }
    }
    return 0; // CPF não encontrado
}

int verificaSenha(int tam, Cliente *clientes, char *compara){

    int aux = 0;
    int ret;
    for (int i = 0; i < tam; i++) {
        ret = strncmp(compara, clientes[i].senha, 15);
        if(ret == 0){
            aux = 1;
            return aux;
        }
    }
    return aux;
}

int tam(Cliente *clientes) {
    FILE *arquivo = fopen(nome_do_arquivo, "rb");
    int cont = 0;
    if (arquivo) {
        while (fread(&clientes[cont], sizeof(Cliente), 1, arquivo) == 1) {
            cont++;
        }
        fclose(arquivo);
    }
    return cont;
}


void novo_cliente(Cliente *clientes) {
    limpa();
    printf("Insira as informacoes a seguir.\n");
    printf("Nome: ");
    scanf("%100[^\n]s", clientes->nome);
    limpa();

    int valido = 0;

    do{
        printf("CPF: ");
        scanf("%20[^\n]s", clientes->cpf);
        limpa();

        if(verificaCPF(tam(clientes), clientes, clientes->cpf)){
            printf("CPF ja existente, por favor insira um CPF diferente\n");
        }
        else{
            valido = 1;
        }
    } while(!valido);

     valido = 0;

    do {
        printf("Tipo de Conta: (comum ou plus) " );
        scanf("%10s", clientes->tipo);

        for (int i = 0; clientes->tipo[i]; i++) {
            clientes->tipo[i] = tolower(clientes->tipo[i]);
        }

        if (strcmp(clientes->tipo, "comum") == 0 || strcmp(clientes->tipo, "plus") == 0) {
            printf("Tipo de conta:  %s\n", clientes->tipo);
            valido = 1;
        } else {
            printf("Tipo de conta invalido: %s. Digite 'comum' ou 'plus'.\n", clientes->tipo);
        }
    } while (!valido);

    limpa();

    printf("Saldo inicial: ");
    scanf("%lf", &clientes->saldo);
    limpa();

    printf("Senha: ");
    scanf("%10[^\n]s", clientes->senha);
    limpa();

}

void listar_clientes(int tam, Cliente *clientes){
    for(int i = 0; i < tam; i++){
        printf("Cliente %d\n", i+1);
        printf("Nome: %s\n",clientes[i].nome);
        printf("Saldo: %.2lf\n",clientes[i].saldo);
        printf("Senha: %s\n",clientes[i].senha);
        printf("CPF: %s\n",clientes[i].cpf);
        printf("Tipo de Conta: %s\n\n", clientes->tipo);
    }
}

void escreve(int tam,  Cliente *clientes) {
    FILE *arquivo= fopen(nome_do_arquivo, "wb");

    if (arquivo) {
        fwrite(clientes, sizeof(Cliente), tam, arquivo);
        fclose(arquivo);
    }
}

void debito(int tam, Cliente *clientes){
    char cpf[20];
    char senha[20];
    int aux = 0, aux2, valor, confirma = 0;

    limpa();
    printf("Insira o seu CPF: ");
    scanf("%20[^\n]s", cpf);

    aux = verificaCPF(tam, clientes, cpf);

    if(!aux) printf("CPF nao encontrado.\n");

    else{
        do {
            limpa();
            printf("Insira a sua senha: ");
            scanf("%20[^\n]s", senha);
            aux = verificaSenha(tam, clientes, senha);
            if(aux){
                confirma = 1;
                aux2 = 1;
            } else{
                printf("Senha invalida. (1 - sair / 0 - tentar novamente)\n");
                scanf("%d", &confirma);
            }
        } while (confirma != 1);

        if(aux2){
            for (int i = 0; i < tam; i++) {
                aux = strncmp(cpf, clientes[i].cpf, 11);
                if(aux == 0){
                    printf("Insira o valor que deseja ser debitado da sua conta: ");
                    scanf("%d", &valor);
                    clientes[i].saldo -= valor;
                }
            }
        }
    }
}

void deposito(int tam, Cliente *clientes) {
    char cpf[20];
    char senha[20];
    int aux = 0, aux2, valor, confirma = 0;

    limpa();
    printf("Insira o seu CPF: ");
    scanf("%20[^\n]s", cpf);

    aux = verificaCPF(tam, clientes, cpf);

    if (!aux)
        printf("CPF não encontrado.\n");
    else {
        do {
            limpa();
            printf("Insira a sua senha: ");
            scanf("%20[^\n]s", senha);
            aux = verificaSenha(tam, clientes, senha);
            if (aux) {
                confirma = 1;
                aux2 = 1;
            } else {
                printf("Senha inválida. (1 - sair / 0 - tentar novamente)\n");
                scanf("%d", &confirma);
            }
        } while (confirma != 1);

        if (aux2) {
            for (int i = 0; i < tam; i++) {
                aux = strncmp(cpf, clientes[i].cpf, 11);
                if (aux == 0) {
                    printf("Insira o valor que deseja depositar na sua conta: ");
                    scanf("%d", &valor);
                    if (valor > 0) {
                        clientes[i].saldo += valor;
                        printf("Deposito de %d realizado com sucesso. Novo saldo: %.2lf\n", valor, clientes[i].saldo);
                    } else {
                        printf("O valor do deposito deve ser maior que zero.\n");
                    }
                }
            }
        }
    }
}
