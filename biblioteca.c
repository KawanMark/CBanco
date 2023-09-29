#include "biblioteca.h"
#include <stdio.h>
#include <string.h>
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

    printf("CPF: ");
    scanf("%20[^\n]s", clientes->cpf);
    limpa();

    printf("Tipo de Conta: ");
    scanf("%10[^\n]s", clientes->tipo);
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
        printf("Cliente %n\n", i+1);
        printf("Nome: %s\n",clientes[i].nome);
        printf("Saldo: %s\n",clientes[i].saldo);
        printf("CPF: %s\n\n",clientes[i].cpf);
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
    char temp[20];
    int aux = 0, valor;

    limpa();
    printf("Insira o seu CPF: ");
    scanf("%20[^\n]s", temp);

    aux = verificaCPF(tam, clientes, temp);

    if(!aux) printf("CPF nao encontrado.\n");

    else{
        for (int i = 0; i < tam; i++) {
            aux = strncmp(temp, clientes[i].cpf, 11);

            if(aux == 0){
                printf("Insira o valor que deseja ser debitado da sua conta: ");
                scanf("%d", &valor);
                clientes[i].saldo -= valor;
            }
        }
    }
}

