#include "biblioteca.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char nome_do_arquivo[20] = "lista_de_clientes";

void limpa(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void apagar_cliente(int *tam, Cliente *clientes) {
    int indice;

    printf("Insira o indice do cliente que deseja apagar: ");
    scanf("%d", &indice);

    if (indice >= 1 && indice <= *tam) {
        for (int i = indice - 1; i < *tam - 1; i++) {
            clientes[i] = clientes[i + 1];
        }

        (*tam)--;
        printf("Cliente no indice %d apagado com sucesso.\n", indice);
    } else {
        printf("Índice invalido. Insira um indice valido.\n");
    }
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


void novo_cliente(int tamanho, Cliente *clientes) {
    char temp[20];
    limpa();
    printf("Insira as informacoes a seguir.\n");
    printf("Nome: ");
    scanf("%100[^\n]s", clientes->nome);
    limpa();
    int aux;
    int valido = 0;

    do{
        printf("CPF: ");
        scanf("%20[^\n]s",temp);
        aux = verificaCPF(tamanho, clientes, temp);
        limpa();

        if(aux){
            printf("CPF ja existente, por favor insira um CPF diferente\n");
        }
        else{
            strcpy(clientes->cpf, temp);
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
void debito(int tam, Cliente *clientes) {
    char cpf[20];
    char senha[20];
    int aux = 0, aux2, valor, confirma = 0;

    limpa();
    printf("Insira o seu CPF: ");
    scanf("%20[^\n]s", cpf);

    aux = verificaCPF(tam, clientes, cpf);

    if (!aux) {
        printf("CPF nao encontrado.\n");
    } else {
        do {
            limpa();
            printf("Insira a sua senha: ");
            scanf("%20[^\n]s", senha);
            aux = verificaSenha(tam, clientes, senha);
            if (aux) {
                confirma = 1;
                aux2 = 1;
            } else {
                printf("Senha invalida. (1 - sair / 0 - tentar novamente)\n");
                scanf("%d", &confirma);
            }
        } while (confirma != 1);

        if (aux2) {
            int indice = -1;

            for (int i = 0; i < tam; i++) {
                aux = strncmp(cpf, clientes[i].cpf, 11);
                if (aux == 0) {
                    indice = i;
                    break;
                }
            }

            if (indice != -1) {
                printf("Insira o valor que deseja debitar da sua conta: ");
                scanf("%d", &valor);

                if (valor > 0) {
                    double taxa = 0.0;

                    if (strcmp(clientes[indice].tipo, "comum") == 0) {
                        taxa = valor * 0.05;
                    } else if (strcmp(clientes[indice].tipo, "plus") == 0) {
                        taxa = valor * 0.03;
                    }

                    if (clientes[indice].saldo - valor - taxa >= -1000.0) {
                        clientes[indice].saldo -= (valor + taxa);
                        printf("Debito de %d realizado com sucesso. Taxa cobrada de: %.2lf. Novo saldo: %.2lf\n", valor, taxa, clientes[indice].saldo);
                    } else {
                        printf("Saldo insuficiente ou limite de saldo negativo excedido.\n");
                    }
                } else {
                    printf("O valor do debito deve ser maior que zero.\n");
                }
            } else {
                printf("Erro interno: cliente não encontrado.\n");
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

void extrato(int tam, Cliente *clientes) {
    char cpf[20];
    char senha[20];
    int aux = 0, aux2;

    limpa();
    printf("Insira o seu CPF: ");
    scanf("%20[^\n]s", cpf);

    aux = verificaCPF(tam, clientes, cpf);

    if (!aux) {
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
            printf("Extrato do Cliente:\n");
            printf("Nome: %s\n", clientes[indice].nome);
            printf("CPF: %s\n", clientes[indice].cpf);
            printf("Tipo de Conta: %s\n", clientes[indice].tipo);
            printf("Saldo Atual: %.2lf\n", clientes[indice].saldo);

            printf("Transacoes:\n");
            for (int i = 0; i < tam; i++) {
                for (int j = 0; j < 100; j++) {
                    if (clientes[i].historico[j].valor != 0) {
                        printf("Transacao %d: %.2lf\n", j + 1, clientes[i].historico[j].valor);
                        printf("Descricao: %s\n", clientes[i].historico[j].descricao);
                    }
                }
            }
        } else {
            printf("Erro interno: cliente nao encontrado.\n");
        }
    }
}




