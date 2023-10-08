#ifndef UNTITLED_BIBLIOTECA_H
#define UNTITLED_BIBLIOTECA_H

typedef struct {

    double valor;
    double taxa;
    char descricao[50];

}Operacoes;

typedef struct {
    char nome[100];
    double saldo;
    char tipo[10];
    char cpf[20];
    char senha[10];
    Operacoes historico[100];

}Cliente;

void listar_clientes(int tam, Cliente *cliente);
void novo_cliente(int tamanho, Cliente *clientes);
void debito(int tam, Cliente *clientes);
void deposito(int tam, Cliente *clientes);
void apagar_cliente(int *tam, Cliente *clientes);
void transferencia(int tam, Cliente *clientes);
void extrato(int tam, Cliente *clientes);
#endif