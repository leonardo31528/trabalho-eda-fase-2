#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "functest.h"

int main()
{
    bool sucesso = false;
    Grafo *grafo = CriarGrafo();
    if (!grafo)
    {
        printf("Erro ao criar o grafo.\n");
        return 1;
    }

    grafo = LerFicheiro(grafo, "exemplo.txt", &sucesso);
    if (!sucesso)
    {
        printf("Erro ao ler o ficheiro de antenas.\n");
        grafo = DestruirGrafo(grafo, &sucesso);
        return 1;
    }
    deduzirNefasto(grafo);

    //LerArestasBinario(grafo, "arestas.bin"); // ler arestas guardas

bool result = ligarVerticesComMesmaFrequencia(grafo); // guardar o resultado da ligação
result = false; 
grafo = RemoverAresta(grafo, 4, 4, 5, 2, &result);

    
    GuardarArestasBinario(grafo , "arestas.bin");

    int contador = 0;
listarAntenas(grafo, &contador);
    char* matriz = gerarMatrizGrafo(grafo);
    if (matriz) {
        fputs(matriz, stdout);  // Aqui faz a impressão
        free(matriz);
    } else {
        fprintf(stderr, "Erro a gerar matriz\n");
    }

    int x = 5;
    int y = 2;

    bfs(grafo, x, y);
    mostrarcaminho(grafo);

    dfs(grafo, x, y);
    mostrarcaminho(grafo);


    if (guardarGrafo(grafo, "resultado.txt"))
    {
        printf("\nFicheiro guardado");
    }

    grafo = DestruirGrafo(grafo, &sucesso);
    if (!sucesso)
    {
        printf("Erro ao destruir o grafo.\n");
        return 1;
    }

    return 0;
}
