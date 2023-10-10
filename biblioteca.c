#include "biblioteca.h" //Duas bibliotecas de funções incluidas
#include "Biblioteca_auxiliar.h"
#include <stdio.h>
#include <string.h>

void apagar_cliente(int *tam, Cliente *clientes) { //Função de apagar clientes
    int indice;

    printf("Insira o numero do cliente que deseja apagar: "); scanf("%d", &indice);
    //É pedido o número do cliente

    if (indice >= 1 && indice <= *tam)//Verifica se o numero do cliente existe
    {
        for (int i = indice - 1; i < *tam - 1; i++) {
            clientes[i] = clientes[i + 1];//Alteração de todos os clientes para posição anterior
        }
        (*tam)--;//Reduz o numero de clientes
        printf("Cliente de numero %d apagado com sucesso.\n", indice);
    }
    else
    {   //Condição caso o numero do cliente não exista
        printf("Numero invalido.\n");
    }
}

void novo_cliente(int tamanho, Cliente *clientes) {//Função de cadastrar clientes
    char temp[20];
    int aux, valido = 0;
    printf("Insira as informacoes a seguir.\n");

    printf("Nome: "); scanf("%100[^\n]s", clientes[tamanho].nome);//Recebe o nome do cliente
    limpa();

    do{// Função do while para pedir CPF até que seja válido

        printf("CPF: "); scanf("%20[^\n]s",temp);
        aux = verificaCPF(tamanho, clientes, temp);//Verifica o CPF
        limpa();

        if(aux != -1){//Caso o CPF seja inválido
            printf("CPF ja existente, por favor insira um CPF diferente\n");
        }
        else{
            //Caso o CPF seja válido o cpf do cliente é inserido na struct a condição de parada do 'do while' é ativa
            strcpy(clientes[tamanho].cpf, temp);
            valido = 1;
        }
    } while(!valido);

    valido = 0;

    do {// Outro 'Do while' para o tipo de conta
        printf("Tipo de Conta: (comum ou plus)"); scanf("%10s", temp);//É pedido o tipo de conta
        limpa();

        arruma_tipo(tamanho, temp, clientes);//Função que arruma o tipo de conta (Explicada na estrutura da função)

        if (!strcmp(clientes[tamanho].tipo, "comum") || !strcmp(clientes[tamanho].tipo, "plus"))// verifica caso o tipo não tenha sido bem inserido
        {
            valido = 1; //condição de para do 'do while' ativa
        }
        else
        {
            //Condição caso o tipo da conta não tenha sido bem inserida
            printf("Tipo de conta invalido: %s. Digite 'comum' ou 'plus'.\n", clientes[tamanho].tipo);
        }
    } while (!valido);

    printf("Saldo inicial: ");//É pedido o saldo inicial da conta
    scanf("%lf", &clientes[tamanho].saldo);
    limpa();

    printf("Senha: ");// É pedido a senha da conta
    scanf("%10[^\n]s", clientes[tamanho].senha);
    limpa();
}

void listar_clientes(int tam, Cliente *clientes){//Função de listar clientes

    for(int i = 0; i < tam; i++)
    {
        // Lista cada informação de todos os clientes por ordem de criação de conta
        printf("Cliente %d\n", i+1);
        printf("Nome: %s\n",clientes[i].nome);
        printf("Saldo: %.2lf\n",clientes[i].saldo);
        printf("Senha: %s\n",clientes[i].senha);
        printf("CPF: %s\n",clientes[i].cpf);
        printf("Tipo de Conta: %s\n\n", clientes[i].tipo);
    }
}

void debito(int tam, Cliente *clientes) {//Função de débito
    char cpf[20], senha[20];
    int aux, aux2, valor, confirma = 0, indice;

    printf("Insira o seu CPF: "); scanf("%20[^\n]s", cpf);// É pedido o CPF

    indice = verificaCPF(tam, clientes, cpf);//Aqui verifico o CPF e recebo indice dele

    if (indice == -1)
    {
        //Condição caso o CPF não exista
        printf("CPF nao encontrado.\n");
    }
    else
    {
        do// Função 'Do while' para pedir a senha
        {
            limpa();

            printf("Insira a sua senha: "); scanf("%20[^\n]s", senha);//Peço a senha
            aux = verificaSenha(tam, clientes, senha);//verifico se a senha está correta

            if (aux)
            {
                //Caso seja correta o do while quebra
                confirma = 1;
                aux2 = 1;
            }
            else
            {
                //Caso seja inválida é perguntado se o cliente quer tentar novamente ou sair da função
                printf("Senha invalida. (1 - sair / 0 - tentar novamente)\n");
                scanf("%d", &confirma);
            }
        } while (confirma != 1);

        if (aux2)
        {
            //Caso a senha seja correta é requisitado o valor de débito
            printf("Insira o valor que deseja debitar da sua conta: ");
            scanf("%d", &valor);

            if (valor > 0)//Verifica se o valor é valido
            {
                double taxa;
                int tipo;

                taxa = aplica_taxa(indice, clientes, valor);//Aplica a taxa de acordo com o tipo da conta
                tipo = confere_tipo(indice, clientes);// Verifica o tipo da conta
                //Ambas as funções explicadas na estrutura da função

                if(tipo)//Caso o tipo seja da conta seja Plus
                {
                    if (clientes[indice].saldo - valor - taxa >= -5000.0)//Verifica o limite de saldo negativo
                    {
                        clientes[indice].saldo -= (valor + taxa);//Descontado o valor da conta
                        strcpy(clientes[indice].historico[clientes[indice].num_transacoes].descricao, "Debito");
                        clientes[indice].historico[clientes[indice].num_transacoes].valor = valor;
                        clientes[indice].historico[clientes[indice].num_transacoes].taxa = taxa;
                        clientes[indice].num_transacoes++;

                        printf("Debito de %d realizado com sucesso. Taxa cobrada de: %.2lf. Novo saldo: %.2lf\n", valor, taxa, clientes[indice].saldo);
                        //Confirmação do valor do valor removido, taxa cobrada e o saldo atual
                    }
                    else
                    {
                        //Caso o saldo seja insuficiente
                        printf("Saldo insuficiente ou limite de saldo negativo excedido.\n");
                    }
                }
                else//Caso o tipo da conta seja comum
                {
                    if (clientes[indice].saldo - valor - taxa >= -1000.0)//Verifica o limite de saldo negativo
                    {
                        clientes[indice].saldo -= (valor + taxa);//Descontado o valor da conta
                        strcpy(clientes[indice].historico[clientes[indice].num_transacoes].descricao, "Debito");
                        clientes[indice].historico[clientes[indice].num_transacoes].valor = valor;
                        clientes[indice].historico[clientes[indice].num_transacoes].taxa = taxa;
                        clientes[indice].num_transacoes++;

                        printf("Debito de %d realizado com sucesso. Taxa cobrada de: %.2lf. Novo saldo: %.2lf\n", valor, taxa, clientes[indice].saldo);
                        //Confirmação do valor removido, taxa cobrado e saldo atual
                    }
                    else
                    {
                        //Caso o saldo seja insuficiente
                        printf("Saldo insuficiente ou limite de saldo negativo excedido.\n");
                    }
                }
            }
            else
            {
                //Caso o valor inserido seja inválido
                printf("O valor do debito deve ser maior que zero.\n");
            }
        }
    }
}

void deposito(int tam, Cliente *clientes) {
    char cpf[20], senha[20];
    int indice, aux, valor, confirma = 0;

    //limpa();
    printf("Insira o seu CPF: ");
    scanf("%20[^\n]s", cpf);

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

        printf("Insira o valor que deseja depositar na sua conta: "); 
        scanf("%d", &valor);

        if (valor > 0)
        {
            double taxa;
            int tipo;

            taxa = aplica_taxa(indice, clientes, valor);
            tipo = confere_tipo(indice, clientes);
            clientes[indice].saldo += valor;

            strcpy(clientes[indice].historico[clientes[indice].num_transacoes].descricao, "Depósito");
            clientes[indice].historico[clientes[indice].num_transacoes].valor = valor;
            clientes[indice].historico[clientes[indice].num_transacoes].taxa = taxa;
            clientes[indice].num_transacoes++;

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
    int indice_origem, indice_destino, tipo;
    double taxa, valor;

    printf("Insira o CPF da conta de origem: ");scanf("%20[^\n]s", cpf_origem);
    indice_origem = verificaCPF(tam, clientes, cpf_origem);
    limpa();

    printf("Insira o CPF da conta de destino: ");scanf("%20[^\n]s", cpf_destino);
    indice_destino = verificaCPF(tam, clientes, cpf_destino);
    limpa();


    printf("Indice de origem: %d\n", indice_origem);
    printf("Indice de destino: %d\n", indice_destino);

    if (indice_origem != -1 && -1 != indice_destino)
    {
        printf("Insira o valor que deseja transferir: "); scanf("%lf", &valor);
        limpa();

        if(valor > 0)
        {
            tipo = confere_tipo(indice_origem, clientes);
            taxa = aplica_taxa(indice_origem, clientes, valor);
            valor += taxa;

            if(tipo && (clientes[indice_origem].saldo - valor) >= -5000)
            {
                clientes[indice_origem].saldo -= valor;
                clientes[indice_destino].saldo += valor;

                strcpy(clientes[indice_origem].historico[clientes[indice_origem].num_transacoes].descricao, "Transferência (envio)");
                clientes[indice_origem].historico[clientes[indice_origem].num_transacoes].valor = -valor;
                clientes[indice_origem].historico[clientes[indice_origem].num_transacoes].taxa = taxa;
                clientes[indice_origem].num_transacoes++;

                 strcpy(clientes[indice_destino].historico[clientes[indice_destino].num_transacoes].descricao, "Transferência (recebimento)");
                clientes[indice_destino].historico[clientes[indice_destino].num_transacoes].valor = valor;
                clientes[indice_destino].historico[clientes[indice_destino].num_transacoes].taxa = 0; // Não há taxa para o destinatário
                clientes[indice_destino].num_transacoes++;

                printf("Transferencia de %.2lf realizada com sucesso.\n", valor);
            }
            else if(!tipo && (clientes[indice_origem].saldo - valor) >= -1000)
            {
                clientes[indice_origem].saldo -= valor;
                clientes[indice_destino].saldo += valor;

                strcpy(clientes[indice_origem].historico[clientes[indice_origem].num_transacoes].descricao, "Transferência (envio)");
                clientes[indice_origem].historico[clientes[indice_origem].num_transacoes].valor = -valor;
                clientes[indice_origem].historico[clientes[indice_origem].num_transacoes].taxa = taxa;
                clientes[indice_origem].num_transacoes++;
        
                strcpy(clientes[indice_destino].historico[clientes[indice_destino].num_transacoes].descricao, "Transferência (recebimento)");
                clientes[indice_destino].historico[clientes[indice_destino].num_transacoes].valor = valor;
                clientes[indice_destino].historico[clientes[indice_destino].num_transacoes].taxa = 0; // Não há taxa para o destinatário
                clientes[indice_destino].num_transacoes++;
                printf("Transferencia de %.2lf realizada com sucesso.\n", valor);
            }
            else
            {
                printf("Valor insuficiente para transferencia.\n");
            }
        }
        else
        {
            printf("Valor invalido para transferencia\n");
        }
    }
    else
    {
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
            printf("-------------------------\n");
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
    int aux, aux2;

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