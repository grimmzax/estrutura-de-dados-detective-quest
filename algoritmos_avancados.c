#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// Detective Quest - Mapa da Mansão com Árvore Binária
// Autor: Jonathan de Souza Araújo
// Descrição: Simula a exploração de uma mansão representada por uma árvore binária.
// O jogador pode navegar pelos cômodos escolhendo esquerda (e) ou direita (d).
// ============================================================================

// -----------------------------------------------------------------------------
// Definição da struct Sala
// Cada sala possui um nome e ponteiros para as salas à esquerda e à direita.
// -----------------------------------------------------------------------------
typedef struct Sala {
    char nome[40];           // Nome do cômodo
    struct Sala* esquerda;   // Ponteiro para a sala à esquerda
    struct Sala* direita;    // Ponteiro para a sala à direita
} Sala;

// -----------------------------------------------------------------------------
// Função criarSala
// Cria dinamicamente uma sala com o nome informado e ponteiros nulos.
// -----------------------------------------------------------------------------
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    if (nova) {
        strcpy(nova->nome, nome);
        nova->esquerda = NULL;
        nova->direita = NULL;
    }
    return nova;
}

// -----------------------------------------------------------------------------
// Função explorarSalas
// Permite ao jogador navegar pela árvore binária da mansão.
// A cada sala, exibe o nome e pergunta se deseja ir para a esquerda, direita ou sair.
// A navegação termina ao chegar em um nó-folha (sem caminhos) ou se o jogador escolher sair.
// -----------------------------------------------------------------------------
void explorarSalas(Sala* atual) {
    char escolha;
    while (atual) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Verifica se é um nó-folha (sem caminhos)
        if (!atual->esquerda && !atual->direita) {
            printf("Fim do caminho! Não há mais portas para explorar.\n");
            break;
        }

        // Exibe opções de navegação
        printf("Escolha o caminho:\n");
        if (atual->esquerda) printf("  (e) Esquerda: %s\n", atual->esquerda->nome);
        if (atual->direita)  printf("  (d) Direita: %s\n", atual->direita->nome);
        printf("  (s) Sair da exploração\n");
        printf("Opção: ");
        scanf(" %c", &escolha);

        // Processa a escolha do jogador
        if (escolha == 'e' && atual->esquerda) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita) {
            atual = atual->direita;
        } else if (escolha == 's') {
            printf("Exploração encerrada pelo jogador.\n");
            break;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// -----------------------------------------------------------------------------
// Função main
// Monta o mapa da mansão (árvore binária) e inicia a exploração.
// -----------------------------------------------------------------------------
int main() {
    // Criação manual da árvore binária representando o mapa da mansão
    // Estrutura exemplo:
    //                Hall de Entrada
    //               /                \
    //        Biblioteca            Cozinha
    //         /      \             /     \
    //   Sala de Estar Sótão   Jardim   Porão

    Sala* hall = criarSala("Hall de Entrada");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* cozinha = criarSala("Cozinha");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* sotao = criarSala("Sótão");
    Sala* jardim = criarSala("Jardim");
    Sala* porao = criarSala("Porão");

    // Conectando as salas (montagem da árvore)
    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    biblioteca->esquerda = salaEstar;
    biblioteca->direita = sotao;
    cozinha->esquerda = jardim;
    cozinha->direita = porao;

    // Início da exploração a partir do Hall de Entrada
    printf("=== Detective Quest: Exploração da Mansão ===\n");
    explorarSalas(hall);

    // Liberação da memória alocada para as salas
    free(salaEstar);
    free(sotao);
    free(biblioteca);
    free(jardim);
    free(porao);
    free(cozinha);
    free(hall);

    return 0;
}