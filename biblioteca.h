#ifndef UNTITLED_BIBLIOTECA_H
#define UNTITLED_BIBLIOTECA_H

typedef struct {

    double valor;
    double taxa;
    char descricao[10];

}Operacoes;

typedef struct {
    char nome[100];
    double saldo;
    char tipo[10];
    char cpf[20];
    char senha[10];
    Operacoes historico[100];

}Cliente;

int tam(Cliente *clientes);
void listar_clientes(int tam, Cliente *cliente);
void novo_cliente(Cliente *cliente);
void escreve(int tam,  Cliente *cliente);

#endif
