#include "Biblioteca_auxiliar.h"
#include "biblioteca.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char nome_do_arquivo[20] = "lista_de_clientes";

void limpa(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int verificaCPF(int tam, Cliente *clientes, char *compara){

    int aux = -1;
    int ret;
    for (int i = 0; i < tam; i++) {
        ret = strncmp(compara, clientes[i].cpf, 11);
        if(ret == 0){
            aux = i;
            return aux;
        }
    }
    return aux;
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


void escreve(int tam,  Cliente *clientes) {
    FILE *arquivo= fopen(nome_do_arquivo, "wb");

    if (arquivo) {
        fwrite(clientes, sizeof(Cliente), tam, arquivo);
        fclose(arquivo);
    }
}

void arruma_tipo(int tamanho, char *tipo, Cliente *clientes){
    for (int i = 0; i < strlen(tipo); i++) {
        tipo[i] = tolower(tipo[i]);
    }
}

double aplica_taxa(int indice,  Cliente *clientes, double valor){
    double taxa;

    if (!strcmp(clientes[indice].tipo, "comum"))
    {
        taxa = valor * 0.05;
    }
    else
    {
        taxa = valor * 0.03;
    }

    return taxa;
}

int confere_tipo(int indice, Cliente *clientes){

    int tipo;
    tipo = strncmp("comum", clientes[indice].tipo, 6);

    if(tipo == 0)
    {
        tipo = 0;
    }
    else
    {
        tipo = 1;
    }

    return tipo;
}