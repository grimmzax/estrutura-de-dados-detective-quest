#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// Detective Quest - Mapa da Mansão com Árvore Binária e Coleta de Pistas (BST)
// Autor: Jonathan de Souza Araújo
// Descrição: Permite explorar uma mansão (árvore binária), coletar pistas em cada cômodo
// e exibir todas as pistas coletadas em ordem alfabética (BST).
// ============================================================================

// -----------------------------------------------------------------------------
// Definição da struct Sala
// Cada sala possui um nome, uma pista (opcional) e ponteiros para as salas à esquerda e à direita.
// -----------------------------------------------------------------------------
typedef struct Sala {
    char nome[40];           // Nome do cômodo
    char pista[60];          // Pista encontrada no cômodo (pode ser "")
    struct Sala* esquerda;   // Ponteiro para a sala à esquerda
    struct Sala* direita;    // Ponteiro para a sala à direita
} Sala;

// -----------------------------------------------------------------------------
// Definição da struct PistaNode (BST de pistas)
// Cada nó armazena uma pista e ponteiros para esquerda/direita (ordenação alfabética).
// -----------------------------------------------------------------------------
typedef struct PistaNode {
    char pista[60];                // Conteúdo da pista
    struct PistaNode* esquerda;    // Ponteiro para pista menor (esquerda)
    struct PistaNode* direita;     // Ponteiro para pista maior (direita)
} PistaNode;

// -----------------------------------------------------------------------------
// Função criarSala
// Cria dinamicamente uma sala com nome e pista informados, ponteiros nulos.
// -----------------------------------------------------------------------------
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    if (nova) {
        strcpy(nova->nome, nome);
        strcpy(nova->pista, pista);
        nova->esquerda = NULL;
        nova->direita = NULL;
    }
    return nova;
}

// -----------------------------------------------------------------------------
// Função inserirPista
// Insere uma nova pista na BST de pistas, mantendo a ordenação alfabética.
// -----------------------------------------------------------------------------
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (!pista || pista[0] == '\0') return raiz; // Não insere pistas vazias
    if (raiz == NULL) {
        PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }
    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (cmp > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    // Se igual, não insere duplicata
    return raiz;
}

// -----------------------------------------------------------------------------
// Função exibirPistas
// Exibe todas as pistas coletadas em ordem alfabética (in-order traversal da BST).
// -----------------------------------------------------------------------------
void exibirPistas(PistaNode* raiz) {
    if (raiz) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// -----------------------------------------------------------------------------
// Função liberarPistas
// Libera toda a memória alocada para a BST de pistas.
// -----------------------------------------------------------------------------
void liberarPistas(PistaNode* raiz) {
    if (raiz) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

// -----------------------------------------------------------------------------
// Função explorarSalasComPistas
// Permite ao jogador navegar pela mansão, coletando pistas e armazenando na BST.
// A cada sala, exibe o nome, a pista (se houver) e pergunta o próximo caminho.
// -----------------------------------------------------------------------------
void explorarSalasComPistas(Sala* atual, PistaNode** pistas) {
    char escolha;
    while (atual) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Se houver pista, coleta e adiciona à BST
        if (atual->pista[0] != '\0') {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            *pistas = inserirPista(*pistas, atual->pista);
        } else {
            printf("Nenhuma pista neste cômodo.\n");
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
// Função liberarSalas
// Libera toda a memória alocada para a árvore de salas (mapa da mansão).
// -----------------------------------------------------------------------------
void liberarSalas(Sala* sala) {
    if (sala) {
        liberarSalas(sala->esquerda);
        liberarSalas(sala->direita);
        free(sala);
    }
}

// -----------------------------------------------------------------------------
// Função main
// Monta o mapa da mansão (árvore binária), inicia a exploração e exibe as pistas coletadas.
// -----------------------------------------------------------------------------
int main() {
    // Criação manual da árvore binária representando o mapa da mansão
    // Cada sala pode ter uma pista (ou não)
    Sala* hall = criarSala("Hall de Entrada", "Pegadas de lama no tapete");
    Sala* biblioteca = criarSala("Biblioteca", "Livro fora do lugar");
    Sala* cozinha = criarSala("Cozinha", "Faca sumida do suporte");
    Sala* salaEstar = criarSala("Sala de Estar", "");
    Sala* sotao = criarSala("Sótão", "Janela aberta");
    Sala* jardim = criarSala("Jardim", "Pegadas recentes na terra");
    Sala* porao = criarSala("Porão", "Chave caída no chão");

    // Conectando as salas (montagem da árvore)
    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    biblioteca->esquerda = salaEstar;
    biblioteca->direita = sotao;
    cozinha->esquerda = jardim;
    cozinha->direita = porao;

    // Inicializa a BST de pistas como vazia
    PistaNode* pistas = NULL;

    // Início da exploração a partir do Hall de Entrada
    printf("=== Detective Quest: Exploração da Mansão ===\n");
    explorarSalasComPistas(hall, &pistas);

    // Exibe todas as pistas coletadas em ordem alfabética
    printf("\nPistas coletadas (ordem alfabética):\n");
    if (pistas)
        exibirPistas(pistas);
    else
        printf("Nenhuma pista foi coletada.\n");

    // Liberação da memória alocada para as pistas e salas
    liberarPistas(pistas);
    liberarSalas(salaEstar);
    liberarSalas(sotao);
    liberarSalas(biblioteca);
    liberarSalas(jardim);
    liberarSalas(porao);
    liberarSalas(cozinha);
    liberarSalas(hall);

    return 0;
}