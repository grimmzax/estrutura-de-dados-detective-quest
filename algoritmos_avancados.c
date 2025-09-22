#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 13 // Tamanho da tabela hash (primo para melhor distribuição)

// ============================================================================
// Detective Quest - Capítulo Final: Pistas, Suspeitos e Julgamento
// Autor: Jonathan de Souza Araújo
// Descrição: Permite explorar uma mansão (árvore binária), coletar pistas (BST),
// associar pistas a suspeitos (hash) e julgar o culpado com base nas evidências.
// ============================================================================

// -----------------------------------------------------------------------------
// Struct Sala: representa um cômodo da mansão (nó da árvore binária)
// -----------------------------------------------------------------------------
typedef struct Sala {
    char nome[40];           // Nome do cômodo
    char pista[60];          // Pista encontrada no cômodo (pode ser "")
    struct Sala* esquerda;   // Ponteiro para a sala à esquerda
    struct Sala* direita;    // Ponteiro para a sala à direita
} Sala;

// -----------------------------------------------------------------------------
// Struct PistaNode: nó da BST de pistas coletadas
// -----------------------------------------------------------------------------
typedef struct PistaNode {
    char pista[60];                // Conteúdo da pista
    struct PistaNode* esquerda;    // Ponteiro para pista menor (esquerda)
    struct PistaNode* direita;     // Ponteiro para pista maior (direita)
} PistaNode;

// -----------------------------------------------------------------------------
// Struct HashPista: entrada da tabela hash (pista -> suspeito)
// -----------------------------------------------------------------------------
typedef struct HashPista {
    char pista[60];                // Chave: pista
    char suspeito[40];             // Valor: nome do suspeito
    struct HashPista* prox;        // Encadeamento para colisões
} HashPista;

// -----------------------------------------------------------------------------
// Função criarSala
// Cria dinamicamente um cômodo com nome e pista informados, ponteiros nulos.
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
// Função inserirPista (BST)
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
// Função hashString
// Gera um índice para a tabela hash a partir de uma string (pista).
// -----------------------------------------------------------------------------
unsigned int hashString(const char* str) {
    unsigned int hash = 0;
    while (*str)
        hash = (hash * 31 + *str++) % TAM_HASH;
    return hash;
}

// -----------------------------------------------------------------------------
// Função inserirNaHash
// Insere uma associação pista/suspeito na tabela hash.
// -----------------------------------------------------------------------------
void inserirNaHash(HashPista* tabela[], const char* pista, const char* suspeito) {
    unsigned int idx = hashString(pista);
    HashPista* novo = (HashPista*)malloc(sizeof(HashPista));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[idx];
    tabela[idx] = novo;
}

// -----------------------------------------------------------------------------
// Função encontrarSuspeito
// Busca o suspeito associado a uma pista na tabela hash.
// -----------------------------------------------------------------------------
const char* encontrarSuspeito(HashPista* tabela[], const char* pista) {
    unsigned int idx = hashString(pista);
    HashPista* atual = tabela[idx];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

// -----------------------------------------------------------------------------
// Função explorarSalas
// Permite ao jogador navegar pela mansão, coletando pistas e armazenando na BST.
// A cada sala, exibe o nome, a pista (se houver) e pergunta o próximo caminho.
// -----------------------------------------------------------------------------
void explorarSalas(Sala* atual, PistaNode** pistas, HashPista* tabela[]) {
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
// Função exibirPistas
// Exibe todas as pistas coletadas em ordem alfabética (in-order traversal da BST).
// -----------------------------------------------------------------------------
void exibirPistas(PistaNode* raiz, HashPista* tabela[]) {
    if (raiz) {
        exibirPistas(raiz->esquerda, tabela);
        printf("- %s (Suspeito: %s)\n", raiz->pista, encontrarSuspeito(tabela, raiz->pista));
        exibirPistas(raiz->direita, tabela);
    }
}

// -----------------------------------------------------------------------------
// Função contarPistasPorSuspeito
// Conta quantas pistas na BST apontam para o suspeito informado.
// -----------------------------------------------------------------------------
int contarPistasPorSuspeito(PistaNode* raiz, HashPista* tabela[], const char* suspeito) {
    if (!raiz) return 0;
    int count = 0;
    const char* s = encontrarSuspeito(tabela, raiz->pista);
    if (s && strcmp(s, suspeito) == 0)
        count = 1;
    return count + contarPistasPorSuspeito(raiz->esquerda, tabela, suspeito)
                + contarPistasPorSuspeito(raiz->direita, tabela, suspeito);
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
// Função liberarHash
// Libera toda a memória alocada para a tabela hash de pistas.
// -----------------------------------------------------------------------------
void liberarHash(HashPista* tabela[]) {
    for (int i = 0; i < TAM_HASH; i++) {
        HashPista* atual = tabela[i];
        while (atual) {
            HashPista* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
}

// -----------------------------------------------------------------------------
// Função verificarSuspeitoFinal
// Solicita acusação do jogador e verifica se há pelo menos 2 pistas para o suspeito.
// -----------------------------------------------------------------------------
void verificarSuspeitoFinal(PistaNode* pistas, HashPista* tabela[]) {
    char suspeito[40];
    printf("\n=== Fase de Julgamento ===\n");
    printf("Com base nas pistas, quem você acusa? Digite o nome do suspeito: ");
    scanf(" %[^\n]", suspeito);

    int count = contarPistasPorSuspeito(pistas, tabela, suspeito);
    printf("\nPistas que apontam para %s: %d\n", suspeito, count);
    if (count >= 2) {
        printf("Acusação aceita! %s foi considerado CULPADO com base nas evidências.\n", suspeito);
    } else {
        printf("Acusação rejeitada. Não há evidências suficientes contra %s.\n", suspeito);
    }
}

// -----------------------------------------------------------------------------
// Função main
// Monta o mapa da mansão, associa pistas a suspeitos, permite exploração e julgamento.
// -----------------------------------------------------------------------------
int main() {
    // Criação manual da árvore binária representando o mapa da mansão
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

    // Inicializa a tabela hash de pistas para suspeitos
    HashPista* hash[TAM_HASH] = {0};

    // Associa pistas a suspeitos (definido no código)
    inserirNaHash(hash, "Pegadas de lama no tapete", "Sr. Green");
    inserirNaHash(hash, "Livro fora do lugar", "Sra. White");
    inserirNaHash(hash, "Faca sumida do suporte", "Coronel Mustard");
    inserirNaHash(hash, "Janela aberta", "Sr. Green");
    inserirNaHash(hash, "Pegadas recentes na terra", "Sr. Green");
    inserirNaHash(hash, "Chave caída no chão", "Sra. White");

    // Início da exploração a partir do Hall de Entrada
    printf("=== Detective Quest: Exploração da Mansão ===\n");
    explorarSalas(hall, &pistas, hash);

    // Exibe todas as pistas coletadas em ordem alfabética, com suspeitos
    printf("\nPistas coletadas (ordem alfabética):\n");
    if (pistas)
        exibirPistas(pistas, hash);
    else
        printf("Nenhuma pista foi coletada.\n");

    // Fase de julgamento: jogador faz acusação
    verificarSuspeitoFinal(pistas, hash);

    // Liberação da memória alocada para as pistas, salas e hash
    liberarPistas(pistas);
    liberarSalas(salaEstar);
    liberarSalas(sotao);
    liberarSalas(biblioteca);
    liberarSalas(jardim);
    liberarSalas(porao);
    liberarSalas(cozinha);
    liberarSalas(hall);
    liberarHash(hash);

    return 0;
}