#include "biblioteca.h"
#include "Biblioteca_auxiliar.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void apagar_cliente(int *tam, Cliente *clientes) {
    int indice;

    printf("Insira o indice do cliente que deseja apagar: "); scanf("%d", &indice);

    if (indice >= 1 && indice <= *tam)
    {
        for (int i = indice - 1; i < *tam - 1; i++) {
            clientes[i] = clientes[i + 1];
        }
        (*tam)--;
        printf("Cliente no indice %d apagado com sucesso.\n", indice);
    }
    else
    {
        printf("Índice invalido. Insira um indice valido.\n");
    }
}

void novo_cliente(int tamanho, Cliente *clientes) {
    char temp[20];
    int aux, valido = 0;
    printf("Insira as informacoes a seguir.\n");

    printf("Nome: "); scanf("%100[^\n]s", clientes[tamanho].nome);
    limpa();

    do{
        printf("CPF: "); scanf("%20[^\n]s",temp);
        aux = verificaCPF(tamanho, clientes, temp);
        limpa();

        if(aux != -1){
            printf("CPF ja existente, por favor insira um CPF diferente\n");
        }
        else{
            strcpy(clientes[tamanho].cpf, temp);
            valido = 1;
        }
    } while(!valido);

    valido = 0;

    do {
        printf("Tipo de Conta: (comum ou plus)"); scanf("%10s", temp);
        limpa();

        arruma_tipo(tamanho, temp, clientes);

        if (!strcmp(clientes[tamanho].tipo, "comum") || !strcmp(clientes[tamanho].tipo, "plus"))
        {
            valido = 1;
        }
        else
        {
            printf("Tipo de conta invalido: %s. Digite 'comum' ou 'plus'.\n", clientes[tamanho].tipo);
        }
    } while (!valido);

    printf("Saldo inicial: ");
    scanf("%lf", &clientes[tamanho].saldo);
    limpa();

    printf("Senha: ");
    scanf("%10[^\n]s", clientes[tamanho].senha);
    limpa();
}

void listar_clientes(int tam, Cliente *clientes){
    for(int i = 0; i < tam; i++){
        printf("Cliente %d\n", i+1);
        printf("Nome: %s\n",clientes[i].nome);
        printf("Saldo: %.2lf\n",clientes[i].saldo);
        printf("Senha: %s\n",clientes[i].senha);
        printf("CPF: %s\n",clientes[i].cpf);
        printf("Tipo de Conta: %s\n\n", clientes[i].tipo);
    }
}

void debito(int tam, Cliente *clientes) {
    limpa();
    char cpf[20], senha[20];
    int aux = 0, aux2, valor, confirma = 0, indice;

    printf("Insira o seu CPF: "); scanf("%20[^\n]s", cpf);

    indice = verificaCPF(tam, clientes, cpf);

    if (indice == -1)
    {
        printf("CPF nao encontrado.\n");
    }
    else
    {
        do
        {
            limpa();

            printf("Insira a sua senha: "); scanf("%20[^\n]s", senha);
            aux = verificaSenha(tam, clientes, senha);

            if (aux)
            {
                confirma = 1;
                aux2 = 1;
            }
            else
            {
                printf("Senha invalida. (1 - sair / 0 - tentar novamente)\n");
                scanf("%d", &confirma);
            }
        } while (confirma != 1);

        if (aux2)
        {

            printf("Insira o valor que deseja debitar da sua conta: ");
            scanf("%d", &valor);

            if (valor > 0)
            {
                double taxa = 0.0;
                int tipo;

                taxa = aplica_taxa(indice, clientes, valor);
                tipo = confere_tipo(indice, clientes);

                if(tipo)
                {
                    if (clientes[indice].saldo - valor - taxa >= -5000.0)
                    {
                        clientes[indice].saldo -= (valor + taxa);
                        printf("Debito de %d realizado com sucesso. Taxa cobrada de: %.2lf. Novo saldo: %.2lf\n", valor, taxa, clientes[indice].saldo);
                    }
                    else
                    {
                        printf("Saldo insuficiente ou limite de saldo negativo excedido.\n");
                    }
                }
                else
                {
                    if (clientes[indice].saldo - valor - taxa >= -1000.0)
                    {
                        clientes[indice].saldo -= (valor + taxa);
                        printf("Debito de %d realizado com sucesso. Taxa cobrada de: %.2lf. Novo saldo: %.2lf\n", valor, taxa, clientes[indice].saldo);
                    }
                    else
                    {
                        printf("Saldo insuficiente ou limite de saldo negativo excedido.\n");
                    }
                }

            }
            else
            {
                printf("O valor do debito deve ser maior que zero.\n");
            }
        }
    }
}

void deposito(int tam, Cliente *clientes) {
    char cpf[20], senha[20];
    int indice = 0, aux, valor, confirma = 0;

    limpa();
    printf("Insira o seu CPF: ");scanf("%20[^\n]s", cpf);

    indice = verificaCPF(tam, clientes, cpf);

    if (indice == -1)
    {
        printf("CPF não encontrado.\n");
    }
    else
    {
        do
        {
            limpa();

            printf("Insira a sua senha: "); scanf("%20[^\n]s", senha);
            aux = verificaSenha(tam, clientes, senha);

            if (aux)
            {
                confirma = 1;
            }
            else
            {
                printf("Senha inválida. (1 - sair / 0 - tentar novamente)\n");
                scanf("%d", &confirma);
            }
        } while (confirma != 1);

        printf("Insira o valor que deseja depositar na sua conta: "); scanf("%d", &valor);

        if (valor > 0)
        {
            clientes[indice].saldo += valor;
            printf("Deposito de %d realizado com sucesso. Novo saldo: %.2lf\n", valor, clientes[indice].saldo);
        }
        else
        {
            printf("O valor do deposito deve ser maior que zero.\n");
        }
    }
}

void transferencia(int tam, Cliente *clientes) {
    char cpf_origem[20], cpf_destino[20];
    int indice_origem, indice_destino, valor;

    limpa();
    printf("Insira o CPF da conta de origem: ");
    scanf("%20[^\n]s", cpf_origem);

    limpa();
    printf("Insira o CPF da conta de destino: ");
    scanf("%20[^\n]s", cpf_destino);

    for (int i = 0; i < tam; i++) {
        if (strcmp(clientes[i].cpf, cpf_origem) == 0) {
            indice_origem = i;
        }
        if (strcmp(clientes[i].cpf, cpf_destino) == 0) {
            indice_destino = i;
        }
    }

    if (indice_origem >= 0 && indice_destino >= 0) {
        printf("Insira o valor que deseja transferir: ");
        scanf("%d", &valor);

        if (valor > 0 && clientes[indice_origem].saldo >= valor) {
            clientes[indice_origem].saldo -= valor;
            clientes[indice_destino].saldo += valor;
            printf("Transferencia de %d realizada com sucesso.\n", valor);
        } else {
            printf("Valor invalido ou saldo insuficiente na conta de origem.\n");
        }
    } else {
        printf("Conta de origem ou conta de destino nao encontrada.\n");
    }
}

void imprimirExtrato(Cliente cliente) {
    printf("Extrato do Cliente:\n");
    printf("Nome: %s\n", cliente.nome);
    printf("CPF: %s\n", cliente.cpf);
    printf("Tipo de Conta: %s\n", cliente.tipo);
    printf("Saldo Atual: %.2lf\n", cliente.saldo);

    printf("Transacoes:\n");
    for (int j = 0; j < 100; j++) {
        if (cliente.historico[j].valor != 0) {
            printf("Transacao %d:\n", j + 1);
            printf("Descricao: %s\n", cliente.historico[j].descricao);
            printf("Valor: %.2lf\n", cliente.historico[j].valor);
            if (strcmp(cliente.historico[j].descricao, "Debito") == 0) {
                printf("Taxa do Debito: %.2lf\n", cliente.historico[j].taxa);
            }
        }
    }
}
void extrato(int tam, Cliente *clientes) {
    char cpf[20];
    char senha[20];
    int aux = 0, aux2;

    limpa();
    printf("Insira o seu CPF: ");
    scanf("%20[^\n]s", cpf);

    aux = verificaCPF(tam, clientes, cpf);

    if (aux == -1) {
        printf("CPF nao encontrado.\n");
    } else {
        do {
            limpa();
            printf("Insira a sua senha: ");
            scanf("%20[^\n]s", senha);
            aux = verificaSenha(tam, clientes, senha);
            if (aux) {
                aux2 = 1;
            } else {
                printf("Senha invalida. (1 - sair)\n");
                scanf("%d", &aux2);
            }
        } while (aux2 != 1);

        int indice = -1;

        for (int i = 0; i < tam; i++) {
            aux = strncmp(cpf, clientes[i].cpf, 11);
            if (aux == 0) {
                indice = i;
                break;
            }
        }

        if (indice != -1) {
            imprimirExtrato(clientes[indice]);
        } else {
            printf("Erro interno: cliente nao encontrado.\n");
        }
    }
}