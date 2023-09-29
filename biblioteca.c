#include "biblioteca.h"
#include <stdio.h>

char nome_do_arquivo[20] = "lista_de_clientes";
void limpa(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
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
        printf("Cliente %d\n", i+1);
        printf("Nome: %s\n\n",clientes[i].nome);
    }
}

void escreve(int tam,  Cliente *clientes) {
    FILE *arquivo= fopen(nome_do_arquivo, "wb");

    if (arquivo) {
        fwrite(clientes, sizeof(Cliente), tam, arquivo);
        fclose(arquivo);
    }
}