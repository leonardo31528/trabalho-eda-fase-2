#ifndef EDBD263F_EDE2_44A9_BADA_FF5F2E5385E2
#define EDBD263F_EDE2_44A9_BADA_FF5F2E5385E2


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


/// @brief Estrutura que representa uma antena (vértice)
typedef struct Vertice {
    int id;
    int x, y;                  ///< Coordenadas únicas da antena
    char freq;                 ///< Frequência da antena
    int visita;
    struct Vertice* prox;
    struct Aresta* arestas;    ///< Lista de arestas ligadas a esta antena
} Vertice;

/// @brief Estrutura que representa uma ligação (aresta) entre antenas
typedef struct Aresta {
    Vertice* destino;          ///< Ponteiro para o vértice de destino
    struct Aresta* prox;       ///< Próxima aresta na lista
} Aresta;

typedef struct Grafo{
    Vertice* vertices;
    int num_vertices;          ///< Contador do número de vértices
    int topo;  // auxiliar para ordem de visitadoos
} Grafo;

typedef struct Fila {
    Vertice* v;
    struct Fila* prox;
} Fila;


Grafo* CriarGrafo();

Vertice* ProcurarVertice(Grafo* g, int x, int y) ;

Grafo* AdicionarVertice(Grafo* g, int x, int y, char freq, bool* sucesso) ;

bool LibertarListaArestas(Aresta* a) ;

Grafo* RemoverAresta(Grafo* g, int xOrig, int yOrig, int xDest, int yDest, bool* sucesso) ;

Grafo* RemoverVertice(Grafo* g, int x, int y, bool* sucesso) ;
Grafo* AdicionarAresta(Grafo* g, int xOrig, int yOrig, int xDest, int yDest, bool* sucesso) ;

Grafo* DestruirGrafo(Grafo* g, bool* sucesso) ;
Grafo* LerFicheiro(Grafo* g, const char* nomeFicheiro, bool* sucesso);
bool guardarGrafo(Grafo* g, const char* nomeFicheiro) ;
Vertice* listarAntenas(Grafo* g, int* contador);
bool deduzirNefasto(Grafo* g);

Vertice* encontrarVerticePorID(Grafo* g, int id) ;

bool inserirAresta(Vertice* origem, Vertice* destino);

bool removerAresta(Vertice* origem, Vertice* destino);

bool ligarVerticesComMesmaFrequencia(Grafo* g);

char* gerarMatrizGrafo(Grafo* g);


bool limparVisitados(Grafo* g) ;

bool dfsRecursivo(Vertice* v, Grafo* g) ;

bool dfs(Grafo* g, int x, int y) ;

bool adicionarAFila(Fila** f, Vertice* v) ;

Vertice* removerDaFila(Fila** f) ;

bool bfs(Grafo* g, int x, int y) ;

bool mostrarcaminho(Grafo* g);

Grafo* LerArestasBinario(Grafo* g, const char* nomeFicheiro) ;

bool GuardarArestasBinario(Grafo* g, const char* nomeFicheiro);

#endif /* EDBD263F_EDE2_44A9_BADA_FF5F2E5385E2 */
