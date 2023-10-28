#include "biblioteca.h" //Duas bibliotecas de funções incluidas
#include "Biblioteca_auxiliar.h"
#include <stdio.h>
#include <string.h>

lista_de_clientes usuarios;

void apagar_cliente(int *tam, Cliente *clientes) { //Função de apagar clientes
    int indice;

    printf("Insira o numero do cliente que deseja apagar: "); scanf("%d", &indice);
    //É pedido o número do cliente

    if (indice >= 1 && indice <= *tam)//Verifica se o numero do cliente existe
    {
        for (int i = indice - 1; i < *tam - 1; i++) {
            usuarios.clientes[i] = usuarios.clientes[i + 1];//Alteração de todos os clientes para posição anterior
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

        printf("CPF: "); scanf("%15[^\n]s",temp);
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
        char temp2[20];
        printf("Tipo de Conta: (comum ou plus)"); scanf("%10s", temp2);//É pedido o tipo de conta
        limpa();

        arruma_tipo(tamanho, temp2, clientes);//Função que arruma o tipo de conta (Explicada naestrutura da função)

        if (!strcmp(temp2, "comum") || !strcmp(temp2, "plus"))// verifica caso o tipo não tenha sido bem inserido
        {
            strcpy(clientes[tamanho].tipo, temp2);
            valido = 1; //condição de para do 'do while' ativa
        }
        else
        {
            //Condição caso o tipo da conta não tenha sido bem inserida
            printf("Tipo de conta invalido: %s. Digite 'comum' ou 'plus'.\n", temp2);
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
                        strcpy(clientes[indice].historico[clientes[indice].num_transacoes].descricao, "Debito");// Aqui estamos a descricao "Débtio" para a categoria descricao da struct de extrato
                        clientes[indice].historico[clientes[indice].num_transacoes].valor = valor;//Registrado o valor da transacao
                        clientes[indice].historico[clientes[indice].num_transacoes].taxa = taxa; //Registrado a taxa da transacao
                        clientes[indice].num_transacoes++; //Numero de transações alterado

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
                        strcpy(clientes[indice].historico[clientes[indice].num_transacoes].descricao, "Debito");// Aqui estamos a descricao "Débtio" para a categoria descricao da struct de extrato
                        clientes[indice].historico[clientes[indice].num_transacoes].valor = valor;// Registrado o valor da transacao
                        clientes[indice].historico[clientes[indice].num_transacoes].taxa = taxa; //Registrado a taxa da transacao
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

    printf("Insira o seu CPF: ");
    scanf("%20[^\n]s", cpf);//Recebemos o cpf e guaradamos em um vetor temporário

    indice = verificaCPF(tam, clientes, cpf); //Recebemos a posicao de onde está o titular do cpf

    if (indice == -1)
    {
        printf("CPF não encontrado.\n"); //Caso o cpf n seja encontrado
    }
    else
    {
        do
        {// 'Do while' para pedir a senha do usuário
            limpa();

            printf("Insira a sua senha: "); scanf("%20[^\n]s", senha);
            aux = verificaSenha(tam, clientes, senha);//verifica a senha

            if (aux)
            {
                confirma = 1; //condicao para encerrar o 'do while'
            }
            else
            {
                printf("Senha inválida. (1 - sair / 0 - tentar novamente)\n");
                scanf("%d", &confirma);
            }
        } while (confirma != 1);

        printf("Insira o valor que deseja depositar na sua conta: ");
        scanf("%d", &valor);//Recebemos o valor a ser depositado

        if (valor > 0)
        {
            double taxa;

            taxa = aplica_taxa(indice, clientes, valor);//Aplica a taxa
            clientes[indice].saldo += valor; //Deposita o valor

            strcpy(clientes[indice].historico[clientes[indice].num_transacoes].descricao, "Depósito");//Registra a descricao
            clientes[indice].historico[clientes[indice].num_transacoes].valor = valor;//Registra o valor
            clientes[indice].historico[clientes[indice].num_transacoes].taxa = taxa;//Registra a taxa
            clientes[indice].num_transacoes++;

            printf("Deposito de %d realizado com sucesso. Novo saldo: %.2lf\n", valor, clientes[indice].saldo);
        }

        else
        {
            printf("O valor do deposito deve ser maior que zero.\n");//Print caso o valor seja invalido
        }
    }
}

void transferencia(int tam, Cliente *clientes) {
    char cpf_origem[20], cpf_destino[20];
    int indice_origem, indice_destino, tipo;
    double taxa, valor;

    printf("Insira o CPF da conta de origem: ");scanf("%20[^\n]s", cpf_origem); //Recebe o cpf de origem
    indice_origem = verificaCPF(tam, clientes, cpf_origem);//Recebe o indice do cpf de origem
    limpa();

    printf("Insira o CPF da conta de destino: ");scanf("%20[^\n]s", cpf_destino); //Recebe o cpf de destino
    indice_destino = verificaCPF(tam, clientes, cpf_destino);// Recebe o indice do cpf de destino
    limpa();


    printf("\nNome da conta de origem: %s\n", clientes[indice_origem].nome);//Confirma os dados das contas envolvidas
    printf("Nome da conta de destino: %s\n\n", clientes[indice_destino].nome);

    if (indice_origem != -1 && -1 != indice_destino)
    {
        printf("Insira o valor que deseja transferir: "); scanf("%lf", &valor);//Recebe o valor a ser transferido
        limpa();

        if(valor > 0)
        {
            tipo = confere_tipo(indice_origem, clientes);//Recebemos o tipo de conta
            taxa = aplica_taxa(indice_origem, clientes, valor);//Recebemos a taxa a ser aplicada
            double valorOrigem = valor + taxa; //Juntamos o valor total a ser descontada da conta de origem

            if(tipo && (clientes[indice_origem].saldo - valor) >= -5000)//Caso a conta seja plus e ainda tenha saldo para a transferencia
            {
                clientes[indice_origem].saldo -= valorOrigem;//desconto do valor
                clientes[indice_destino].saldo += valor;//acréscimo do valor

                strcpy(clientes[indice_origem].historico[clientes[indice_origem].num_transacoes].descricao, "Transferência (envio)");//Registra a descricao
                clientes[indice_origem].historico[clientes[indice_origem].num_transacoes].valor = -valor; //Registra o valor descontado
                clientes[indice_origem].historico[clientes[indice_origem].num_transacoes].taxa = taxa; //Registra a taxa aplicada
                clientes[indice_origem].num_transacoes++;

                strcpy(clientes[indice_destino].historico[clientes[indice_destino].num_transacoes].descricao, "Transferência (recebimento)");//Registra a descricao
                clientes[indice_destino].historico[clientes[indice_destino].num_transacoes].valor = valor; //Registra o valor recebido
                clientes[indice_destino].historico[clientes[indice_destino].num_transacoes].taxa = 0; // Não há taxa para o destinatário
                clientes[indice_destino].num_transacoes++;

                printf("Transferencia de %.2lf realizada com sucesso.\n", valor);
            }
            else if(!tipo && (clientes[indice_origem].saldo - valor) >= -1000)//Caso a conta seja comum e tenha saldo suficiente para a transferência
            {
                clientes[indice_origem].saldo -= valorOrigem;//desconto do valor
                clientes[indice_destino].saldo += valor;//acrescimo do valor

                strcpy(clientes[indice_origem].historico[clientes[indice_origem].num_transacoes].descricao, "Transferência (envio)");//Registra a descricao
                clientes[indice_origem].historico[clientes[indice_origem].num_transacoes].valor = -valor;//Registra o valor descontado
                clientes[indice_origem].historico[clientes[indice_origem].num_transacoes].taxa = taxa;//Registra a taxa
                clientes[indice_origem].num_transacoes++;

                strcpy(clientes[indice_destino].historico[clientes[indice_destino].num_transacoes].descricao, "Transferência (recebimento)");//Registra a descricao
                clientes[indice_destino].historico[clientes[indice_destino].num_transacoes].valor = valor;//Registra o valor recebido
                clientes[indice_destino].historico[clientes[indice_destino].num_transacoes].taxa = 0; // Não há taxa para o destinatário
                clientes[indice_destino].num_transacoes++;
                printf("Transferencia de %.2lf realizada com sucesso.\n", valor);
            }
            else
            {
                printf("Valor insuficiente para transferencia.\n");//Caso o cliente não tenha saldo para fazer a transferencia
            }
        }
        else
        {
            printf("Valor invalido para transferencia\n");//Caso o valor não seja aceito
        }
    }
    else
    {
        printf("Conta de origem ou conta de destino nao encontrada.\n");//Caso uma das contas não exista
    }
}

void imprimirExtrato(Cliente cliente) {

    // Imprime as informações do cliente
    printf("Extrato do Cliente:\n");
    printf("Nome: %s\n", cliente.nome); // Imprime o nome do cliente
    printf("CPF: %s\n", cliente.cpf); // Imprime o CPF do cliente
    printf("Tipo de Conta: %s\n", cliente.tipo); // Imprime o tipo de conta do cliente
    printf("Saldo Atual: %.2lf\n", cliente.saldo); // Imprime o saldo atual do cliente

    printf("Transacoes:\n"); // Inicia a impressão das transações
    char arquivo[50];
    sprintf(arquivo, "%s.txt", cliente.cpf); // Formata o nome do arquivo com o CPF do cliente

    FILE *f = fopen(arquivo, "w"); // Abre o arquivo para escrita

    if(f== NULL){
        printf("ERRO AO ABRIR"); // Se houver um erro ao abrir o arquivo, imprime uma mensagem de erro
    }
    for (int j = 0; j < cliente.num_transacoes; j++) { // Loop através das transações do cliente
        if (cliente.historico[j].valor != 0) { // Se a transação tiver um valor diferente de zero

            fprintf(f, "-------------------------\n"); // Imprime uma linha divisória no arquivo
            printf("-------------------------\n"); // Imprime uma linha divisória na saída padrão

            fprintf(f, "Transacao %d:\n", j + 1); // Imprime o número da transação no arquivo
            printf("Transacao %d:\n", j + 1); // Imprime o número da transação na saída padrão

            fprintf(f, "Descricao: %s\n", cliente.historico[j].descricao); // Imprime a descrição da transação no arquivo
            printf("Descricao: %s\n", cliente.historico[j].descricao); // Imprime a descrição da transação na saída padrão

            fprintf(f, "Valor: %.2lf\n", cliente.historico[j].valor); // Imprime o valor da transação no arquivo
            printf("Valor: %.2lf\n", cliente.historico[j].valor); // Imprime o valor da transação na saída padrão

            if (strcmp(cliente.historico[j].descricao, "Debito") == 0) {
                fprintf(f, "Taxa do Debito: %.2lf\n", cliente.historico[j].taxa); // Se a descrição for "Debito", imprime a taxa de débito no arquivo
                printf("Taxa do Debito: %.2lf\n", cliente.historico[j].taxa); // Se a descrição for "Debito", imprime a taxa de débito na saída padrão
            }
            else if (strcmp(cliente.historico[j].descricao, "Transferência (envio)") == 0) {
                fprintf(f, "Taxa da Transferencia: %.2lf\n", cliente.historico[j].taxa); // Se a descrição for "Transferência (envio)", imprime a taxa de transferência no arquivo
                printf("Taxa da Transferencia: %.2lf\n", cliente.historico[j].taxa); // Se a descrição for "Transferência (envio)", imprime a taxa de transferência na saída padrão
            }
        }

        fclose(f); // Fecha o arquivo após escrever todas as transações
    }
    printf("\n"); // Imprime uma linha em branco para separar as saídas
}

void extrato(int tam, Cliente *clientes) {
    char cpf[20]; // Variável para armazenar o CPF do cliente
    char senha[20]; // Variável para armazenar a senha do cliente
    int aux, aux2; // Variáveis auxiliares

    printf("Insira o seu CPF: "); // Solicita ao usuário que insira o CPF

    scanf("%20[^\n]s", cpf); // Lê o CPF inserido pelo usuário

    aux = verificaCPF(tam, clientes, cpf); // Verifica se o CPF inserido existe na lista de clientes

    if (aux == -1) { // Se o CPF não for encontrado
        printf("CPF nao encontrado.\n"); // Imprime uma mensagem de erro
    } else {
        do {
            limpa(); // Limpa a tela
            printf("Insira a sua senha: "); // Solicita ao usuário que insira a senha
            scanf("%15[^\n]s", senha);
            aux = verificaSenha(tam, clientes, senha); // Verifica se a senha inserida é correta
            if (aux) { // Se a senha for correta
                aux2 = 1;
            } else {
                printf("Senha invalida. (1 - sair / 0 - Tentar novamente)\n");
                scanf("%d", &aux2);
            }
        } while (aux2 != 1);

        int indice = -1; // Variável para armazenar o índice do cliente na lista de clientes

        for (int i = 0; i < tam; i++) { // Loop através da lista de clientes
            aux = strncmp(cpf, clientes[i].cpf, 11); // Compara o CPF inserido com o CPF do cliente atual na lista
            if (aux == 0) {
                indice = i; // Armazena o índice do cliente
                break;
            }
        }

        if (indice != -1)
        { // Se um cliente com o CPF inserido foi encontrado

            imprimirExtrato(clientes[indice]); // Imprime o extrato do cliente
        }
        else
        {

            printf("Erro interno: cliente nao encontrado.\n"); // Imprime uma mensagem de erro
        }
    }
}
