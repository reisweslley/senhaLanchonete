#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX_NOME 100
#define TAM_MAX_PEDIDO 200

typedef struct {
    char letra;
    int numero;
    char nome[TAM_MAX_NOME];
    char pedido[TAM_MAX_PEDIDO];
} Senha;

int numeroSenhas = 0;
int contadorSenha = 1;
int totalAtendidas = 0; // <-- Contador de atendidos na sessão
Senha *senhas = NULL;

void adicionarSenha(const char *nome, const char *pedido) {
    senhas = realloc(senhas, (numeroSenhas + 1) * sizeof(Senha));
    if (senhas == NULL) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }
    senhas[numeroSenhas].letra = 'A';
    senhas[numeroSenhas].numero = contadorSenha++;
    strncpy(senhas[numeroSenhas].nome, nome, TAM_MAX_NOME);
    strncpy(senhas[numeroSenhas].pedido, pedido, TAM_MAX_PEDIDO);
    numeroSenhas++;
}

void imprimirSenhas() {
    if (numeroSenhas == 0) {
        printf("Nenhum pedido pendente.\n");
        return;
    }
    for (int i = 0; i < numeroSenhas; i++) {
        printf("Senha %d: %c%d | Cliente: %s | Pedido: %s\n", 
            i + 1, senhas[i].letra, senhas[i].numero, senhas[i].nome, senhas[i].pedido);
    }
    printf("Total de pedidos pendentes: %d\n", numeroSenhas);
}

void arquivarSenha(int indice) {
    FILE *arquivo = fopen("senhas_atendidas.txt", "a");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir arquivo para arquivar senhas.\n");
        return;
    }
    fprintf(arquivo, "Senha atendida: %c%d | Cliente: %s | Pedido: %s\n",
            senhas[indice].letra, senhas[indice].numero,
            senhas[indice].nome, senhas[indice].pedido);
    fclose(arquivo);
}

void atenderProximo() {
    if (numeroSenhas == 0) {
        printf("Nenhuma senha para atender.\n");
        return;
    }
    printf("Atendendo senha: %c%d | Cliente: %s | Pedido: %s\n",
           senhas[0].letra, senhas[0].numero, senhas[0].nome, senhas[0].pedido);
    arquivarSenha(0);

    for (int i = 1; i < numeroSenhas; i++) {
        senhas[i - 1] = senhas[i];
    }
    numeroSenhas--;
    totalAtendidas++;  // <-- Incrementa atendidos

    if (numeroSenhas == 0) {
        free(senhas);
        senhas = NULL;
    } else {
        senhas = realloc(senhas, numeroSenhas * sizeof(Senha));
    }
}

void mostrarEstatisticas() {
    printf("\n--- Estatísticas do Sistema ---\n");
    printf("Pedidos pendentes: %d\n", numeroSenhas);
    printf("Pedidos atendidos nesta sessão: %d\n", totalAtendidas);
    printf("--------------------------------\n");
}

int main() {
    int opcao;
    char nome[TAM_MAX_NOME];
    char pedido[TAM_MAX_PEDIDO];

    while (1) {
        printf("\nMenu:\n");
        printf("1. Gerar novo pedido\n");
        printf("2. Atender próximo pedido\n");
        printf("3. Listar pedidos\n");
        printf("4. Ver estatísticas\n"); // <-- Adicionada no menu
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // limpar buffer do teclado

        switch (opcao) {
            case 1:
                printf("Digite o nome do cliente: ");
                fgets(nome, TAM_MAX_NOME, stdin);
                nome[strcspn(nome, "\n")] = '\0'; // remover \n
                printf("Digite o pedido: ");
                fgets(pedido, TAM_MAX_PEDIDO, stdin);
                pedido[strcspn(pedido, "\n")] = '\0'; // remover \n
                adicionarSenha(nome, pedido);
                printf("Pedido gerado com sucesso!\n");
                break;

            case 2:
                atenderProximo();
                break;

            case 3:
                imprimirSenhas();
                break;

            case 4:
                mostrarEstatisticas();
                break;

            case 0:
                printf("Encerrando programa.\n");
                free(senhas);
                return 0;

            default:
                printf("Opção inválida!\n");
        }
    }
}