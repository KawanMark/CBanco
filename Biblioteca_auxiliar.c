#include "Biblioteca_auxiliar.h"
#include "biblioteca.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char nome_do_arquivo[20] = "lista_de_clientes"; // Define o nome do arquivo

void limpa(){ // Função para limpar o buffer de entrada
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {} //
}

int verificaCPF(int tam, Cliente *clientes, char *compara){ // Função para verificar se um CPF existe na lista de clientes

    int aux = -1;
    int ret;
    for (int i = 0; i < tam; i++) { //
        ret = strncmp(compara, clientes[i].cpf, 11); // Compara o CPF inserido com o CPF do cliente atual na lista
        if(ret == 0){
            aux = i; // Armazena o índice do cliente na variável aux
            return aux;
        }
    }
    return aux; // Retorna aux (será -1 se o CPF não for encontrado)
}

int verificaSenha(int tam, Cliente *clientes, char *compara){ // Função para verificar se uma senha existe na lista de clientes

    int aux = 0;
    int ret;
    for (int i = 0; i < tam; i++) {
        ret = strncmp(compara, clientes[i].senha, 15); // Compara a senha inserida com a senha do cliente atual na lista
        if(ret == 0){
            aux = 1; // Define aux como 1 e retorna aux
            return aux;
        }
    }
    return aux; // Retorna aux (será 0 se a senha não for encontrada)
}

void tam(lista_de_clientes *usuarios) { // Função para ler a lista de clientes de um arquivo binário
    FILE *arquivo = fopen(nome_do_arquivo, "rb");

    if (arquivo) { // Se o arquivo foi aberto com sucesso
        fread(usuarios, sizeof(lista_de_clientes), 1, arquivo); // Lê a lista de clientes do arquivo
        fclose(arquivo); // Fecha o arquivo
    }
}

void escreve(lista_de_clientes *usuarios) { // Função para escrever a lista de clientes em um arquivo binário
    FILE *arquivo = fopen(nome_do_arquivo, "wb");

    if (arquivo) { // Se o arquivo foi aberto com sucesso
        fwrite(usuarios, sizeof(lista_de_clientes), 1, arquivo); // Escreve a lista de clientes no arquivo
        fclose(arquivo); // Fecha o arquivo
    }
}

void arruma_tipo(int tamanho, char *tipo, Cliente *clientes){// Função para converter todos os caracteres em uma string para minúsculas
    for (int i = 0; i < strlen(tipo); i++) {
        tipo[i] = tolower(tipo[i]);
    }
}

double aplica_taxa(int indice,  Cliente *clientes, double valor){// Função para calcular e retornar a taxa aplicada a uma transação com base no tipo de conta do cliente e no valor da transação
    double taxa;

    if (!strcmp(clientes[indice].tipo, "comum"))// Se o tipo de conta do cliente for "comum"
    {
        taxa = valor * 0.05;
    }
    else// Se o tipo de conta do cliente não for "comum"
    {
        taxa = valor * 0.03;
    }

    return taxa; // Retorna a taxa calculada
}

int confere_tipo(int indice, Cliente *clientes){// Função para verificar o tipo de conta de um cliente

    int tipo;
    tipo = strncmp("comum", clientes[indice].tipo, 6); // Compara o tipo de conta do cliente com a string "comum"

    if(tipo == 0)// Se o tipo de conta do cliente for "comum"
    {
        tipo = 0;
    }
    else// Se o tipo de conta do cliente não for "comum"
    {
        tipo = 1;
    }

    return tipo; // Retorna o valor de tipo (será 0 para contas "comum" e 1 para contas "plus")
}
