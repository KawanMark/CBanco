#ifndef UNTITLED_BIBLIOTECA_AUXILIAR_H
#define UNTITLED_BIBLIOTECA_AUXILIAR_H
#include "biblioteca.h"


void escreve(int tam,  Cliente *cliente);
int tam(Cliente *clientes);
void limpa();
int verificaSenha(int tam, Cliente *clientes, char *compara);
int verificaCPF(int tam, Cliente *clientes, char *compara);
void arruma_tipo(int tamanho, char *tipo, Cliente *clientes);
double aplica_taxa(int indice,  Cliente *clientes, double valor);
int confere_tipo(int indice, Cliente *clientes);

#endif
