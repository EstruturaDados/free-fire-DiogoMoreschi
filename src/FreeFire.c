#include <stdio.h>
#include <string.h>
/*
 * FreeFire - Sistema de Mochila (Desafio Código da Ilha)
 * Implementa três níveis: Novato, Aventureiro e Mestre.
 * C11 - código modularizado, comentado e compilável com gcc.
 */
#include <stdlib.h>
#include <stdbool.h>

#define CAPACITY 10
#define NAME_SIZE 50
#define TYPE_SIZE 30

typedef struct {
    char nome[NAME_SIZE];
    char tipo[TYPE_SIZE];
    int quantidade;
    int prioridade; /* 1 a 5 - usado no nível Mestre */
} Item;

typedef enum {
    POR_NOME = 1,
    POR_TIPO = 2,
    POR_PRIORIDADE = 3
} SortCriteria;

/* Inventário estático (capacidade fixa) e variáveis globais de estado */
static Item mochila[CAPACITY];
static int contador = 0; /* número atual de itens na mochila */
static bool ordenado_por_nome = false; /* indica se a lista está ordenada por nome */

/* ---------- Helpers de entrada/saída ---------- */
/* Limpa o restante do buffer de entrada */
static void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* Lê uma linha segura usando fgets e remove o '\n' final */
static void read_line(char *buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}

/* Lê um inteiro seguro a partir de stdin, com fallback */
static int read_int(void) {
    char buf[32];
    read_line(buf, sizeof(buf));
    return (int)strtol(buf, NULL, 10);
}

/* ---------- Operações de Mochila (Novato) ---------- */
/* Adiciona um item na mochila (sem uso de ponteiros nos requisitos Novato) */
static void adicionar_item(void) {
    if (contador >= CAPACITY) {
        puts("Mochila cheia. Remova um item antes de adicionar outro.");
        return;
    }

    Item novo;
    printf("Nome: ");
    read_line(novo.nome, NAME_SIZE);
    printf("Tipo: ");
    read_line(novo.tipo, TYPE_SIZE);

    printf("Quantidade: ");
    novo.quantidade = read_int();

    /* prioridade só é usada no nível Mestre, mas pedimos para compatibilidade */
    printf("Prioridade (1-5): ");
    novo.prioridade = read_int();
    if (novo.prioridade < 1) novo.prioridade = 1;
    if (novo.prioridade > 5) novo.prioridade = 5;

    mochila[contador] = novo;
    contador++;
    ordenado_por_nome = false; /* nova inserção pode desfazer ordenação */

    puts("Item adicionado com sucesso.");
}

/* Remove item pelo nome (procura simples e shift) */
static void remover_item(void) {
    if (contador == 0) {
        puts("Mochila vazia.");
        return;
    }

    char nome[NAME_SIZE];
    printf("Nome do item a remover: ");
    read_line(nome, NAME_SIZE);

    int idx = -1;
    for (int i = 0; i < contador; ++i) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        puts("Item não encontrado na mochila.");
        return;
    }

    /* Shift left para preencher o espaço */
    for (int j = idx; j < contador - 1; ++j) {
        mochila[j] = mochila[j + 1];
    }
    contador--;
    puts("Item removido com sucesso.");
}

/* Lista todos os itens em formato de tabela */
static void listar_itens(void) {
    if (contador == 0) {
        puts("Mochila vazia.");
        return;
    }

    printf("+----+-------------------------------+-----------------+----------+----------+\n");
    printf("| #  | Nome                          | Tipo            | Quantidade | Prioridade |\n");
    printf("+----+-------------------------------+-----------------+----------+----------+\n");
    for (int i = 0; i < contador; ++i) {
        printf("| %2d | %-29s | %-15s | %8d | %8d |\n",
               i + 1,
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade,
               mochila[i].prioridade);
    }
    printf("+----+-------------------------------+-----------------+----------+----------+\n");
}

/* ---------- Busca Sequencial (Aventureiro) ---------- */
static void buscar_sequencial(void) {
    if (contador == 0) {
        puts("Mochila vazia.");
        return;
    }

    char nome[NAME_SIZE];
    printf("Nome a procurar (busca sequencial): ");
    read_line(nome, NAME_SIZE);

    bool encontrou = false;
    for (int i = 0; i < contador; ++i) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            printf("Item encontrado na posição %d:\n", i + 1);
            printf("  Nome: %s\n", mochila[i].nome);
            printf("  Tipo: %s\n", mochila[i].tipo);
            printf("  Quantidade: %d\n", mochila[i].quantidade);
            printf("  Prioridade: %d\n", mochila[i].prioridade);
            encontrou = true;
            break; /* primeira ocorrência */
        }
    }
    if (!encontrou) {
        puts("Item não encontrado.");
    }
}

/* ---------- Ordenação (Mestre) - Insertion Sort com contador ---------- */
/* Compara dois itens segundo o critério; incrementa contador sempre que uma comparação lógica ocorre. */
static int comparar_items(const Item *a, const Item *b, SortCriteria criterio, long long *comparacoes) {
    if (criterio == POR_NOME) {
        (*comparacoes)++;
        return strcmp(a->nome, b->nome);
    } else if (criterio == POR_TIPO) {
        (*comparacoes)++;
        return strcmp(a->tipo, b->tipo);
    } else { /* POR_PRIORIDADE */
        (*comparacoes)++;
        /* prioridade: maior prioridade deve ser considerada "menor" na ordenação? Vamos ordenar desc em prioridade maior primeiro */
        if (a->prioridade == b->prioridade) return 0;
        return (b->prioridade - a->prioridade); /* inverso para ordenar decrescente */
    }
}

static long long insertion_sort(SortCriteria criterio) {
    long long comparacoes = 0;
    for (int i = 1; i < contador; ++i) {
        Item key = mochila[i];
        int j = i - 1;

        /* Enquanto j >= 0 e mochila[j] > key segundo o critério, desloca */
        while (j >= 0 && comparar_items(&mochila[j], &key, criterio, &comparacoes) > 0) {
            mochila[j + 1] = mochila[j];
            j--;
        }
        mochila[j + 1] = key;
    }

    /* Marca se está ordenado por nome */
    ordenado_por_nome = (criterio == POR_NOME);
    return comparacoes;
}

/* Menu de ordenação que pede critério e executa Insertion Sort */
static void menu_ordenacao(void) {
    if (contador <= 1) {
        puts("Nada para ordenar (0 ou 1 item).");
        return;
    }

    printf("Escolha o criterio de ordenacao:\n");
    printf("  1 - Por Nome\n");
    printf("  2 - Por Tipo\n");
    printf("  3 - Por Prioridade (maior prioridade primeiro)\n");
    printf("Opcao: ");
    int opc = read_int();

    if (opc < 1 || opc > 3) {
        puts("Opcao invalida.");
        return;
    }

    SortCriteria crit = (SortCriteria)opc;
    long long comps = insertion_sort(crit);
    puts("Ordenacao concluida.");
    printf("Comparacoes realizadas no Insertion Sort: %lld\n", comps);
}

/* ---------- Busca Binária (Mestre) - por nome, exige ordenado por nome ---------- */
static void busca_binaria_por_nome(void) {
    if (contador == 0) {
        puts("Mochila vazia.");
        return;
    }

    if (!ordenado_por_nome) {
        puts("Busca binaria so e permitida se a mochila estiver ordenada por nome.");
        return;
    }

    char nome[NAME_SIZE];
    printf("Nome a procurar (busca binaria): ");
    read_line(nome, NAME_SIZE);

    int low = 0, high = contador - 1;
    bool encontrado = false;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(mochila[mid].nome, nome);
        if (cmp == 0) {
            printf("Item encontrado na posicao %d:\n", mid + 1);
            printf("  Nome: %s\n", mochila[mid].nome);
            printf("  Tipo: %s\n", mochila[mid].tipo);
            printf("  Quantidade: %d\n", mochila[mid].quantidade);
            printf("  Prioridade: %d\n", mochila[mid].prioridade);
            encontrado = true;
            break;
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (!encontrado) {
        puts("Item nao encontrado (busca binaria).");
    }
}

/* ---------- Menu Principal e fluxo interativo ---------- */
static void mostrar_menu(void) {
    puts("\n=== Mochila - Desafio Codigo da Ilha (Free Fire) ===");
    puts("1 - Adicionar item (Novato)");
    puts("2 - Remover item pelo nome (Novato)");
    puts("3 - Listar itens (Novato)");
    puts("4 - Buscar item por nome (Aventureiro - sequencial)");
    puts("5 - Ordenar mochila (Mestre - Insertion Sort)");
    puts("6 - Buscar por nome (Mestre - Busca Binaria; exige ordenado por nome)");
    puts("0 - Sair");
    printf("Escolha: ");
}

int main(void) {
    /* Mensagem inicial e loop do-while + switch conforme requerido */
    int opcao;
    do {
        mostrar_menu();
        opcao = read_int();

        switch (opcao) {
            case 1:
                adicionar_item();
                break;
            case 2:
                remover_item();
                break;
            case 3:
                listar_itens();
                break;
            case 4:
                buscar_sequencial();
                break;
            case 5:
                menu_ordenacao();
                break;
            case 6:
                busca_binaria_por_nome();
                break;
            case 0:
                puts("Saindo. Obrigado por usar a mochila virtual!");
                break;
            default:
                puts("Opcao invalida. Tente novamente.");
        }
    } while (opcao != 0);

    return 0;
}


