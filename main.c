#include <stdio.h>
#include "biblioteca.h"
#include "Biblioteca_auxiliar.h"

void menu();

Cliente clientes[1000];
extern lista_de_clientes usuarios;



int main() {
    int opcao, aux = 0; //, tamanho = 0;

    
    tam(&usuarios);
    
    while (1){

        menu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("'Novo Cliente' selecionado.\n");
                limpa();
                novo_cliente(usuarios.tamanho, usuarios.clientes);
                usuarios.tamanho++;
                break;
            case 2:
                printf("'Apaga Cliente' selecionado.\n");
                limpa();
                apagar_cliente(&usuarios.tamanho, usuarios.clientes);
                break;
            case 3:
                printf("'Listar Cliente' selecionado.\n");
                limpa();
                listar_clientes(usuarios.tamanho, usuarios.clientes);
                break;
            case 4:
                printf("'Debito' selecionado.\n");
                limpa();
                debito(usuarios.tamanho, usuarios.clientes);
                break;
            case 5:
                printf("'Deposito' selecionado.\n");
                limpa();
                deposito(usuarios.tamanho, usuarios.clientes);
                break;
            case 6:
                printf("'Extrato' selecionado.\n");
                limpa();
                extrato(usuarios.tamanho, usuarios.clientes);
                break;
            case 7:
                printf("'Transferencia entre contas' selecionado.\n");
                limpa();
                transferencia(usuarios.tamanho, usuarios.clientes);
                break;
            case 0:
                printf("'Sair' selecionado.\n");
                limpa();
                escreve(&usuarios);
                aux = 1;
                break;
            default:
                limpa();
                printf("Escolha invalida.\n");
        }
        if(aux)break;
    }

    return 0;
}

void menu(){
    printf("Bem vindo ao banco Quem Poupa Tem. Qual operacao voce precisa hoje ?\n");
    printf("1. Novo Cliente\n");
    printf("2. Apagar Cliente\n");
    printf("3. Listar Cliente\n");
    printf("4. Debito\n");
    printf("5. Deposito\n");
    printf("6. Extrato\n");
    printf("7. Transferencia entre contas\n");
    printf("0. Sair\n");
}