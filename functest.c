/**
 * @file funcoes.c
 * @author Leonardo Costa (a31528@alunos.ipca.pt)
 * @brief Implementação das funções principais do projeto de gestão de grafos e antenas.
 * @version 0.1
 * @date 2025-05-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "functest.h"

/**
 * @brief Cria um novo grafo e inicializa seus campos.
 * 
 * Esta função aloca memória dinamicamente para uma nova estrutura Grafo.
 * Inicializa a lista de vértices como vazia (NULL) e o contador de vértices como 0.
 * 
 * - Se a alocação de memória falhar, retorna NULL.
 * 
 * @return Grafo* Ponteiro para o novo grafo criado ou NULL se falhar a alocação.
 */

Grafo* CriarGrafo() {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo)); //Aloca dinamicamente memória para um novo grafo
    if (!grafo) return NULL; //se nao houver espaço suficiente para criar o grafo retorna null
    grafo->vertices = NULL; // inicia a lista de vertices como vazia
    grafo->num_vertices = 0; // o grafo no inicio vai ter  0 vertices
    return grafo; // retorna o grafo sem nada
}

/**
 * @brief Procura um vértice no grafo com coordenadas específicas.
 * 
 * Percorre a lista de vértices do grafo procurando um vértice que tenha
 * as coordenadas (x, y) fornecidas como parâmetro.
 * 
 * @param g Ponteiro para o grafo onde a pesquisa será realizada.
 * @param x Coordenada X do vértice a procurar.
 * @param y Coordenada Y do vértice a procurar.
 * 
 * @return Vertice* Ponteiro para o vértice encontrado ou NULL se não existir.
 */

Vertice* ProcurarVertice(Grafo* g, int x, int y) {
    Vertice* atual = g->vertices; //Inicia a procura a partir do primeiro vértice da lista ligada
    while (atual) {
        if (atual->x == x && atual->y == y) //atualiza as coordenadas da antena
            return atual; //retorna para o vertice encontrado
        atual = atual->prox; // se nao esta correta as coordenadas do vertice ele percorre a lista ate encontrar o que pretende
    }
    return NULL; // se nao encontrar o vertice anula a funcao ou seja n acontece
}

/**
 * @brief Adiciona um novo vértice ao grafo com coordenadas e frequência especificadas.
 * 
 * Esta função cria um novo vértice, inicializa os seus campos com as coordenadas (x, y),
 * frequência e um ID único. O vértice é inserido no início da lista de vértices do grafo.
 * 
 * Antes de adicionar, verifica se já existe um vértice com as mesmas coordenadas.
 * Se existir, não adiciona e retorna o grafo original.
 * 
 * @param g Ponteiro para o grafo onde o vértice será adicionado.
 * @param x Coordenada X do novo vértice.
 * @param y Coordenada Y do novo vértice.
 * @param freq Frequência associada ao vértice.
 * @param sucesso Ponteiro para booleano que indica se a adição foi bem-sucedida.
 * 
 * @return Grafo* Ponteiro para o grafo atualizado.
 */

Grafo* AdicionarVertice(Grafo* g, int x, int y, char freq, bool* sucesso) {
    *sucesso = false; // o ponteiro sucesso é definio como false
    if (ProcurarVertice(g, x, y)) return g; // Verifica se já existe um vértice com as mesmas coordenadas

    Vertice* novo = malloc(sizeof(Vertice));// define otamanho alocado para o vertice
    if (!novo) return g; // Se falhar a alocação, retorna o grafo sem alterações

    novo->id = g->num_vertices++;  // Atribui ID único
    novo->x = x; // atualiza a cordenada x nova para o x do vertice criado
    novo->y = y; // atualiza y
    novo->freq = freq; 
    novo->arestas = NULL;//o vertice criado (nasce) sem ligacao nenhumaou seja sem aresta
    novo->prox = g->vertices; 
    g->vertices = novo;   // Liga o novo vértice à lista de vértices do grafo (inserção no início da lista)
    g->num_vertices++; // mais um vertice para o grafo
    *sucesso = true; // vertice adicionado com sucesso
    return g; // retorna o grafo com um vertice a mais
}

/**
 * @brief Liberta a memória ocupada por uma lista ligada de arestas.
 * 
 * Esta função percorre a lista ligada de arestas, libertando a memória
 * de cada nó para evitar fugas de memória.
 * 
 * @param a Ponteiro para o início da lista ligada de arestas.
 * 
 * @return bool Retorna true após libertar toda a lista.
 */

bool LibertarListaArestas(Aresta* a) {
    while (a) { // Enquanto existir uma aresta na lista
        Aresta* temp = a; // Guarda o ponteiro atual numa variável temporária
        a = a->prox; // Avança para a próxima aresta da lista
        free(temp);  // Liberta a memória da aresta atual
    }
    return true; // retorna bool se for uma funcao bem sucedida por ser booleana
}

/**
 * @brief Remove a aresta entre dois vértices no grafo.
 * 
 * Esta função procura os vértices de origem e destino no grafo e remove
 * as arestas que ligam esses dois vértices em ambas as direções (origem->destino e destino->origem).
 * Se os vértices não existirem, não faz alterações.
 * 
 * @param g Ponteiro para o grafo onde a remoção será feita.
 * @param xOrig Coordenada X do vértice de origem.
 * @param yOrig Coordenada Y do vértice de origem.
 * @param xDest Coordenada X do vértice de destino.
 * @param yDest Coordenada Y do vértice de destino.
 * @param sucesso Ponteiro para variável booleana que indica se a remoção foi bem sucedida.
 * 
 * @return Grafo* Ponteiro para o grafo atualizado.
 */

Grafo* RemoverAresta(Grafo* g, int xOrig, int yOrig, int xDest, int yDest, bool* sucesso) {
    *sucesso = false;
    Vertice* origem = ProcurarVertice(g, xOrig, yOrig); // procura o vertice inicial o primeiro
    Vertice* destino = ProcurarVertice(g, xDest, yDest); // procura o vertice final ou a seguir ou seja os dois ligados
    if (!origem || !destino) return g; // se nao encontrar nenhum vertice com as coordenadas pedidas devolve o grafo sem alteracoes

    
    Aresta* atual = origem->arestas; // atual percorre a lista de arestas do vértice origem
    Aresta* anterior = NULL; //guarda o ponteiro da aresta anterior
    while (atual) { 
        if (atual->destino == destino) { //Verificamos se esta aresta liga ao vértice de destino
            if (anterior) anterior->prox = atual->prox; // Pedes à aresta anterior para ignorar a atual e ligar-se diretamente à próxima (atual->prox).
            else origem->arestas = atual->prox; // Resultado: a lista continua, mas sem o primeiro elemento
            free(atual); //liberta a memoria da aresta  
            *sucesso = true; // 
            break; //Paramos o ciclo porque já removemos a aresta.
        }
        //Avançamos na lista, se ainda não encontramos a ligação.
        anterior = atual;
        atual = atual->prox;
    }

    
    atual = destino->arestas; //Agora percorremos a lista de arestas do vértice destino
    anterior = NULL; // guarda o ponteiro da aresta anterior
    while (atual) {
        if (atual->destino == origem) {
            if (anterior) anterior->prox = atual->prox; // o mesmo processo so q ao contrario
            else destino->arestas = atual->prox;
            free(atual);
            *sucesso = true; 
            break;
        }
        anterior = atual;
        atual = atual->prox;
    }

    return g;
}

/**
 * @brief Remove um vértice do grafo com todas as suas ligações.
 * 
 * Esta função procura o vértice no grafo com as coordenadas fornecidas (x, y)
 * e remove-o da lista de vértices. Antes de remover, elimina todas as arestas 
 * que apontam para esse vértice, garantindo que não fiquem ligações pendentes.
 * Se o vértice não existir, o grafo permanece inalterado.
 * 
 * @param g Ponteiro para o grafo onde será feita a remoção.
 * @param x Coordenada X do vértice a remover.
 * @param y Coordenada Y do vértice a remover.
 * @param sucesso Ponteiro para variável booleana que indica se a remoção foi bem sucedida.
 * 
 * @return Grafo* Ponteiro para o grafo atualizado.
 */

Grafo* RemoverVertice(Grafo* g, int x, int y, bool* sucesso) {
    *sucesso = false;
    Vertice *atual = g->vertices, *anterior = NULL; //cria o ponteiro atual que armazena o 1 vertice do grafo,c ponteiro anterior é criado para que nao se perca nada da lista
    while (atual) {
        if (atual->x == x && atual->y == y) { // quando sao digitadas as coordenadas de um vertice
            
            Vertice* v = g->vertices; // cria um ponteiro que toma o valor do primeiro vertice
            while (v) {
                bool dummy;
                RemoverAresta(g, v->x, v->y, x, y, &dummy); // remove a aresta que foi desejada
                v = v->prox; // atualiza a lista e o primeiro vertice sera o proximo
            }
            LibertarListaArestas(atual->arestas); // liberta a primeira aresta
            if (anterior) anterior->prox = atual->prox; // remove o vertice no meio e Se existe um nó anterior, ele "pula" o nó atual, apontando direto para o próximo.
            else g->vertices = atual->prox; //Quando o vértice a remover é o primeiro da lista, atualizamos o ponteiro inicial da lista para o próximo vértice.
            free(atual); // liberta a memoria do vertice removido
            g->num_vertices--; // tira o numero de verticess removido
            *sucesso = true;
            break; //para se removeu
        }
        anterior = atual;
        atual = atual->prox; //percorre a lista sem perder o vertice anterior
    }
    return g;//retorna o grafo se nao encontrar o vertice pretendido
}

/**
 * @brief Adiciona uma aresta entre dois vértices no grafo.
 * 
 * Esta função cria uma ligação direcionada do vértice de origem para o vértice 
 * de destino, representando uma aresta na estrutura do grafo. Antes de adicionar,
 * verifica se ambos os vértices existem e se a aresta já não está presente, 
 * evitando duplicações.
 * 
 * @param g Ponteiro para o grafo onde a aresta será adicionada.
 * @param xOrig Coordenada X do vértice de origem.
 * @param yOrig Coordenada Y do vértice de origem.
 * @param xDest Coordenada X do vértice de destino.
 * @param yDest Coordenada Y do vértice de destino.
 * @param sucesso Ponteiro para variável booleana que indica se a aresta foi adicionada com sucesso.
 * 
 * @return Grafo* Ponteiro para o grafo atualizado.
 */

Grafo* AdicionarAresta(Grafo* g, int xOrig, int yOrig, int xDest, int yDest, bool* sucesso) {
    *sucesso = false;
    Vertice* origem = ProcurarVertice(g, xOrig, yOrig); // procura o vertice de origem
    Vertice* destino = ProcurarVertice(g, xDest, yDest); // procura o destino
    if (!origem || !destino) return g; // se nao encontrar retorna o grafo

    Aresta* a = origem->arestas; // cria a variavel a q armazena a primeira aresta
    while (a) {
        if (a->destino == destino) return g; //se a aresta destino ja existir retorna o grafo
        a = a->prox; // percorre a lista
    }

    Aresta* nova = malloc(sizeof(Aresta)); // cria momoria para nova aresta
    if (!nova) return g; // se nao houver espaço retorna o grafo
    nova->destino = destino;// Define o destino da nova aresta
    nova->prox = origem->arestas; // O novo nó 'nova' vai apontar para a primeira aresta atual do vértice 'origem' (inserção no início da lista)
    origem->arestas = nova; //Agora, o vértice 'origem' passa a ter 'nova' como a primeira aresta da sua lista.
    *sucesso = true;
    return g;
}

/**
 * @brief Liberta toda a memória associada ao grafo e aos seus vértices.
 * 
 * Esta função percorre todos os vértices do grafo, libertando as listas de arestas 
 * associadas a cada vértice, e depois liberta a memória dos próprios vértices. 
 * Finalmente, liberta a memória da estrutura do grafo.
 * 
 * @param g Ponteiro para o grafo a ser destruído.
 * @param sucesso Ponteiro para variável booleana que indica se a operação foi bem-sucedida.
 * 
 * @return NULL Retorna NULL para facilitar a atribuição do ponteiro do grafo a NULL após a destruição.
 */

Grafo* DestruirGrafo(Grafo* g, bool* sucesso) {
    *sucesso = false;
    Vertice* atual = g->vertices; // o ponteiro atual aponta para o primeiro vertice
    while (atual) {
        Vertice* temp = atual; // Cria um ponteiro temp para guardar o vértice atual, para que possamos liberar sua memória depois.
        LibertarListaArestas(temp->arestas); // Liberta todas as arestas ligadas ao vértice apontado por temp
        atual = atual->prox;// Atualiza atual para apontar para o próximo vértice da lista
        free(temp); // liberta a memoria do primeiro vertice
    }
    free(g); // elimina a memoria do grafo removido
    *sucesso = true;
    return NULL; //retorna o grafo sem nada
}

/**
 * @brief Lê um grafo a partir de um ficheiro de texto.
 * 
 * Esta função abre um ficheiro cujo nome é fornecido e lê linha a linha,
 * criando vértices no grafo para cada caractere diferente de '.'.
 * As coordenadas x e y são usadas para definir a posição do vértice.
 * O grafo é criado do zero dentro da função.
 * 
 * @param g Ponteiro para o grafo atual (será substituído pelo novo grafo criado).
 * @param nomeFicheiro Nome do ficheiro de texto a ler.
 * @param sucesso Ponteiro para variável booleana que indica se a leitura foi bem-sucedida.
 * 
 * @return Grafo* Ponteiro para o grafo lido do ficheiro (novo grafo criado).
 */

Grafo* LerFicheiro(Grafo* g, const char* nomeFicheiro, bool* sucesso) {
    FILE* f = fopen(nomeFicheiro, "r"); // Abre o ficheiro para leitura.
    if (!f) { // Se não foi possível abrir o ficheiro
        *sucesso = false; // nao ha sucesso
        return g; // retorna o grafo como estava
    }

    g = CriarGrafo(); // cria grafo sem nada
    int x = 0, y = 0; // cria variaveis x e y com valor inicial de 0
    char c; // Declara variável para armazenar cada carácter lido do ficheiro

    while ((c = fgetc(f)) != EOF) { // while lê cada carácter até ao fim do ficheiro
        if (c == '\n') { // se vai para a linha de baixo
            y++; // Incrementa y para indicar que passámos para a próxima linha
            x = 0; // Reinicia x para zero no início de cada nova linha
        } else {
            if (c != '.') { // se no ficheiro nao for o . indica que será adicionado um vertice 
                bool adicionado;
                g = AdicionarVertice(g, x, y, c, &adicionado);//está a adicionar um vértice nas coordenadas (x, y) com o valor c
            }
            x++; // soma 1 x a cada vez que se move para a direita e ao chegar ao fim aciona o y++ e recomeça o x denovo
        }
    }

    *sucesso = true; //O ficheiro foi lido com sucesso
    fclose(f); // fecha o ficheiro 
    return g;
}

/**
 * @brief Deduz e adiciona vértices "nefastos" num grafo baseado em reflexões.
 * 
 * Esta função percorre todos os pares de vértices do grafo e verifica se têm a mesma
 * frequência e não são o caractere '#'. Para cada par, calcula as posições espelhadas
 * dos vértices relativamente ao outro, e tenta adicionar novos vértices nessas posições
 * com frequência '#', que representam vértices "nefastos".
 * 
 * Se algum vértice nefasto for adicionado com sucesso, a função retorna true,
 * indicando que o grafo foi modificado.
 * 
 * @param g Ponteiro para o grafo onde serão adicionados os vértices nefastos.
 * 
 * @return bool true se pelo menos um vértice nefasto foi adicionado; false caso contrário.
 */

bool deduzirNefasto(Grafo* g) {
    bool modificou = false;
    for (Vertice* v1 = g->vertices; v1 != NULL; v1 = v1->prox) { //Começa um ciclo for que percorre todos os vértices do grafo, usando o ponteiro v1 
        for (Vertice* v2 = g->vertices; v2 != NULL; v2 = v2->prox) { // Isto faz com que cada vértice seja comparado com todos os outros (incluindo ele mesmo).
            if (v1 == v2) continue; // Se v1 e v2 apontam para o mesmo vértice, ignora essa iteração do ciclo (passa para o próximo v2
            if (v1->freq == v2->freq && v1->freq != '#') { //Verifica se os dois vértices têm a mesma frequência (freq) e essa frequência não é o carácter '#'
               //Calcula as coordenadas "espelhadas" do vértice v2 relativamente a v1. É como refletir v2 em torno de v1.
                int x_espelho1 = 2 * v1->x - v2->x;
                int y_espelho1 = 2 * v1->y - v2->y;

                //Faz o mesmo cálculo inverso, espelhando v1 relativamente a v2.
                int x_espelho2 = 2 * v2->x - v1->x;
                int y_espelho2 = 2 * v2->y - v1->y;

                bool sucesso;

                if (x_espelho1 >= 0 && y_espelho1 >= 0) { //Se as coordenadas espelhadas são válidas (não negativas)
                    AdicionarVertice(g, x_espelho1, y_espelho1, '#', &sucesso); //tenta adicionar um novo vértice com frequência '#' (marcador) nessas coordenadas. 
                    if (sucesso) modificou = true;//Se for adicionado com sucesso
                }

                if (x_espelho2 >= 0 && y_espelho2 >= 0) { // a mesma coisa para o segundo espelhamento
                    AdicionarVertice(g, x_espelho2, y_espelho2, '#', &sucesso);
                    if (sucesso) modificou = true;
                }
            }
        }
    }
    return modificou;// ermina os ciclos e retorna modificou, que indica se alguma modificação (adição) foi feita ao grafo.
}

/**
 * @brief Lista todas as antenas do grafo e as suas conexões (arestas).
 * 
 * Esta função percorre todos os vértices do grafo, imprime as coordenadas e a frequência
 * de cada antena, seguido das antenas ligadas a ela (arestas). Além disso, conta o número
 * total de antenas existentes no grafo.
 * 
 * @param g Ponteiro para o grafo contendo as antenas.
 * @param contador Ponteiro para um inteiro onde será armazenado o número total de antenas.
 * 
 * @return Vertice* Ponteiro para o primeiro vértice da lista do grafo.
 */

 Vertice* listarAntenas(Grafo* g, int* contador){
    if (!g || !contador) return NULL; // o grafo ou o ponteiro contador não foram passados (são NULL).

    *contador = 0; // o contador começa a zero
    Vertice* v = g->vertices; // o ponteiro v criado aponta para o primeiro vertice
    while (v) {
        printf("Antena (%d, %d) [%c] -> ", v->x, v->y, v->freq); //exibe as coordenadas e frequência da antena atual
        Aresta* a = v->arestas; // o ponteiro a tem o valor da primeira aresta
        while (a) {
            printf("%c(%d, %d) ", a->destino->freq, a->destino->x, a->destino->y); // exibe todas as antenas destino das arestas do vértice atual.
            a = a->prox; // percorre a lista de arestas
        }
        printf("\n");
        (*contador)++; // conta uma antena visitada
        v = v->prox;//percorre a lista de vertices
    }

    return g->vertices; //retorna o ponteiro para o primeiro vértice da lista de vértices.
}

/**
 * @brief Guarda o grafo num ficheiro de texto.
 *
 * Esta função cria uma matriz bidimensional representando as antenas
 * presentes no grafo, usando as suas coordenadas (x,y) para posicioná-las.
 * Cada célula da matriz representa uma posição no plano. As antenas são
 * representadas pelo seu caractere de frequência. As posições sem antenas
 * são preenchidas com '.'.
 * 
 * O conteúdo da matriz é escrito num ficheiro de texto com o nome especificado,
 * onde cada linha corresponde a uma linha da matriz.
 *
 * @param g Ponteiro para o grafo a guardar.
 * @param nomeFicheiro Nome do ficheiro onde o grafo será guardado.
 *
 * @return true se o grafo foi guardado com sucesso, false caso contrário
 *         (por exemplo, erro na alocação de memória ou abertura do ficheiro).
 */

bool guardarGrafo(Grafo* g, const char* nomeFicheiro) {
    if (!g) return false;

    FILE* f = fopen(nomeFicheiro, "w");
    if (!f) return false;

    Vertice* v = g->vertices;
    while (v) {
        // Exemplo de formato: x y freq
        fprintf(f, "%d %d %c\n", v->x, v->y, v->freq);
        v = v->prox;
    }

    fclose(f);
    return true;
}

/**
 * @brief Procura um vértice no grafo pelo seu identificador único.
 * 
 * Percorre a lista de vértices do grafo procurando um vértice cujo
 * campo `id` seja igual ao valor fornecido.
 * 
 * @param g Ponteiro para o grafo onde a procura será realizada.
 * @param id Identificador único do vértice a encontrar.
 * 
 * @return Ponteiro para o vértice com o id correspondente, ou NULL se não existir.
 */

Vertice* encontrarVerticePorID(Grafo* g, int id) {
    Vertice* v = g->vertices;
    while (v) {
        if (v->id == id) return v;
        v = v->prox;
    }
    return NULL;
}

/**
 * @brief Insere uma aresta da origem para o destino no grafo.
 * 
 * Adiciona uma aresta na lista de adjacência do vértice de origem
 * que aponta para o vértice de destino, evitando duplicatas.
 * 
 * @param origem Ponteiro para o vértice de origem.
 * @param destino Ponteiro para o vértice de destino.
 * 
 * @return true se a aresta foi inserida com sucesso, false caso contrário
 *         (por exemplo, se a aresta já existir ou falha na alocação).
 */

bool inserirAresta(Vertice* origem, Vertice* destino) {
    if (!origem || !destino) return false;

    // Evitar aresta duplicada
    Aresta* atual = origem->arestas;
    while (atual) {
        if (atual->destino == destino) return false; // já existe
        atual = atual->prox;
    }

    Aresta* nova = malloc(sizeof(Aresta));
    if (!nova) return false;

    nova->destino = destino;
    nova->prox = origem->arestas;
    origem->arestas = nova;

    return true;
}

/**
 * @brief Remove a aresta da origem para o destino no grafo.
 * 
 * Procura e remove a aresta que conecta o vértice origem ao vértice destino
 * na lista de adjacência do vértice origem.
 * 
 * @param origem Ponteiro para o vértice de origem.
 * @param destino Ponteiro para o vértice de destino.
 * 
 * @return true se a aresta foi encontrada e removida, false caso contrário.
 */

bool removerAresta(Vertice* origem, Vertice* destino) {
    if (!origem || !destino) return false;

    Aresta* atual = origem->arestas;
    Aresta* anterior = NULL;

    while (atual) {
        if (atual->destino == destino) {
            if (anterior) anterior->prox = atual->prox;
            else origem->arestas = atual->prox;
            free(atual);
            return true;
        }
        anterior = atual;
        atual = atual->prox;
    }

    return false;
}

/**
 * @brief Liga vértices que têm a mesma frequência no grafo.
 * 
 * Percorre todos os pares de vértices no grafo e cria arestas bidirecionais 
 * entre os vértices que possuem a mesma frequência, desde que a frequência
 * não seja '#' ou '.'.
 * 
 * @param g Ponteiro para o grafo.
 * 
 * @return true se pelo menos uma nova aresta foi adicionada, false caso contrário.
 */

bool ligarVerticesComMesmaFrequencia(Grafo* g) {
    bool modificou = false;
    for (Vertice* v1 = g->vertices; v1 != NULL; v1 = v1->prox) {
        for (Vertice* v2 = v1->prox; v2 != NULL; v2 = v2->prox) {
            if (v1->freq == v2->freq && v1->freq != '#' && v1->freq != '.') {
                if (inserirAresta(v1, v2)) modificou = true;
                if (inserirAresta(v2, v1)) modificou = true;
            }
        }
    }
    return modificou;
}

/**
 * @brief Gera uma representação em matriz do grafo como uma string.
 * 
 * Cria uma matriz 2D representada por uma string onde cada linha termina com '\n' e 
 * a matriz contém os caracteres das frequências dos vértices nas suas posições (x, y).
 * As posições sem vértices são preenchidas com '.'.
 * 
 * A string resultante deve ser libertada pelo chamador para evitar memory leaks.
 * 
 * @param g Ponteiro para o grafo.
 * 
 * @return Ponteiro para uma string alocada dinamicamente contendo a matriz do grafo,
 *         ou NULL em caso de falha na alocação.
 */

char* gerarMatrizGrafo(Grafo* g) {
    int maxX = 0, maxY = 0;
    for (Vertice* v = g->vertices; v != NULL; v = v->prox) {
        if (v->x > maxX) maxX = v->x;
        if (v->y > maxY) maxY = v->y;
    }

    int linhas = maxY + 1;
    int colunas = maxX + 1;
    int tamanho = linhas * (colunas + 1) + 1;

    char* buffer = malloc(tamanho);
    if (!buffer) return NULL;

    for (int y = 0; y < linhas; y++) {
        for (int x = 0; x < colunas; x++) {
            buffer[y * (colunas + 1) + x] = '.';
        }
        buffer[y * (colunas + 1) + colunas] = '\n';
    }
    buffer[tamanho - 1] = '\0';

    for (Vertice* v = g->vertices; v != NULL; v = v->prox) {
        buffer[v->y * (colunas + 1) + v->x] = v->freq;
    }

    return buffer;
}

/**
 * @brief Reseta o campo de visitação de todos os vértices do grafo para zero.
 * 
 * Percorre todos os vértices do grafo e define o campo `visita` como 0, indicando
 * que nenhum vértice está marcado como visitado. Também reinicia o campo `topo` do grafo para 1.
 * 
 * @param g Ponteiro para o grafo.
 * 
 * @return true se a operação foi bem-sucedida, false se o ponteiro do grafo for NULL.
 */

bool limparVisitados(Grafo* g) {
    if (g == NULL) return false; // falhou porque o grafo não existe
    
    Vertice* atual = g->vertices;
    while (atual != NULL) {
        atual->visita = 0;
        atual = atual->prox;
    }
    g->topo = 1;
    return true; // sucesso
}

/**
 * @brief Executa a busca em profundidade (DFS) recursiva a partir de um vértice.
 * 
 * Marca o vértice atual como visitado atribuindo um número sequencial de visitação,
 * e recursa para todos os vértices adjacentes ainda não visitados.
 * 
 * @param v Ponteiro para o vértice atual da DFS.
 * @param g Ponteiro para o grafo, usado para atualizar o contador de visitação.
 * 
 * @return true se o vértice foi visitado com sucesso, false se o vértice for NULL ou já visitado.
 */

bool dfsRecursivo(Vertice* v, Grafo* g) {
    if (v == NULL || v->visita != 0) return false;

    v->visita = g->topo++;

    Aresta* a = v->arestas;
    while (a != NULL) {
        dfsRecursivo(a->destino, g);
        a = a->prox;
    }
    return true;  // sucesso em visitar este vértice
}

/**
 * @brief Inicia a busca em profundidade (DFS) no grafo a partir do vértice nas coordenadas (x, y).
 * 
 * Esta função limpa o estado de visitação dos vértices, procura o vértice inicial
 * com as coordenadas especificadas e chama a função recursiva de DFS para visitar
 * todos os vértices alcançáveis a partir dele.
 * 
 * @param g Ponteiro para o grafo onde a DFS será realizada.
 * @param x Coordenada x do vértice inicial.
 * @param y Coordenada y do vértice inicial.
 * 
 * @return true se a DFS foi iniciada e executada com sucesso, false caso contrário
 *         (grafo inválido, vértice inicial não encontrado, falha ao limpar visitas).
 */

bool dfs(Grafo* g, int x, int y) {
    if (!limparVisitados(g)) return false;

    Vertice* inicio = ProcurarVertice(g, x, y);
    if (inicio == NULL) return false;

    dfsRecursivo(inicio, g);
    return true;
}

/**
 * @brief Adiciona um vértice ao final da fila ligada.
 * 
 * Esta função cria um novo elemento de fila contendo o vértice dado
 * e o adiciona ao final da lista ligada que representa a fila.
 * Se a fila estiver vazia (ponteiro *f é NULL), o novo elemento passa a ser o primeiro.
 * 
 * @param f Ponteiro para o ponteiro da fila (lista ligada) onde o vértice será adicionado.
 * @param v Ponteiro para o vértice a ser adicionado à fila.
 * 
 * @return true se a operação foi bem-sucedida, false se falhou na alocação de memória.
 */

bool adicionarAFila(Fila** f, Vertice* v) {
    Fila* novo = malloc(sizeof(Fila));
    if (novo == NULL) {
        return false;  // falha ao alocar memória
    }
    novo->v = v;
    novo->prox = NULL;
    if (*f == NULL) {
        *f = novo;
    } else {
        Fila* temp = *f;
        while (temp->prox) temp = temp->prox;
        temp->prox = novo;
    }
    return true;  // sucesso
}

/**
 * @brief Remove e retorna o primeiro vértice da fila ligada.
 * 
 * Esta função remove o primeiro elemento da fila (lista ligada)
 * e retorna o ponteiro para o vértice armazenado nesse elemento.
 * Se a fila estiver vazia, retorna NULL.
 * 
 * @param f Ponteiro para o ponteiro da fila (lista ligada) de onde o vértice será removido.
 * 
 * @return Ponteiro para o vértice removido, ou NULL se a fila estiver vazia.
 */

Vertice* removerDaFila(Fila** f) {
    if (*f == NULL) return NULL;
    Fila* temp = *f;
    *f = temp->prox;
    Vertice* v = temp->v;
    free(temp);
    return v;
}

/**
 * @brief Executa uma busca em largura (BFS) no grafo a partir do vértice com coordenadas (x, y).
 * 
 * A função inicializa os estados de visita dos vértices, localiza o vértice inicial,
 * e realiza a BFS marcando a ordem de visitação em cada vértice.
 * 
 * @param g Ponteiro para o grafo onde a busca será realizada.
 * @param x Coordenada X do vértice inicial.
 * @param y Coordenada Y do vértice inicial.
 * 
 * @return true se a busca foi executada com sucesso, false caso o vértice inicial não seja encontrado.
 */

bool bfs(Grafo* g, int x, int y) {
    limparVisitados(g);
    Vertice* inicio = ProcurarVertice(g, x, y);
    if (inicio == NULL) return false;  // Não encontrou o vértice inicial

    Fila* fila = NULL;
    adicionarAFila(&fila, inicio);
    inicio->visita = g->topo++;

    while (fila != NULL) {
        Vertice* atual = removerDaFila(&fila);
        Aresta* a = atual->arestas;
        while (a != NULL) {
            if (a->destino->visita == 0) {
                a->destino->visita = g->topo++;
                adicionarAFila(&fila, a->destino);
            }
            a = a->prox;
        }
    }
    return true;  // Busca executada com sucesso
}

/**
 * @brief Mostra a ordem de visita dos vértices no grafo.
 * 
 * Percorre todos os vértices do grafo e imprime aqueles que foram visitados,
 * exibindo suas coordenadas, frequência e a ordem em que foram visitados.
 * 
 * @param g Ponteiro para o grafo.
 * 
 * @return true se pelo menos um vértice foi visitado e exibido, false caso contrário.
 */

bool mostrarcaminho(Grafo* g) {
    bool encontrou = false;
    printf("Ordem de visita dos vértices:\n");
    Vertice* atual = g->vertices;
    while (atual != NULL) {
        if (atual->visita != 0) {
            printf("Antena em (%d, %d), Freq: %c, Ordem: %d\n", 
                   atual->x, atual->y, atual->freq, atual->visita);
            encontrou = true;
        }
        atual = atual->prox;
    }
    return encontrou;
}

/**
 * @brief Lê arestas de um ficheiro binário e adiciona ao grafo.
 * 
 * O ficheiro binário deve conter sequências de 4 inteiros representando
 * as coordenadas (x1, y1) e (x2, y2) de duas antenas (vértices) que
 * serão ligadas por uma aresta.
 * 
 * Para cada par de coordenadas lidas, procura os vértices no grafo e,
 * se existirem, adiciona uma aresta entre eles.
 * 
 * @param g Ponteiro para o grafo onde as arestas serão adicionadas.
 * @param nomeFicheiro Nome do ficheiro binário a ser lido.
 * 
 * @return Ponteiro para o grafo atualizado.
 */

Grafo* LerArestasBinario(Grafo* g, const char* nomeFicheiro) {
    FILE* f = fopen(nomeFicheiro, "rb");
    if (!f) {
        perror("Erro ao abrir ficheiro de arestas");
        return g;
    }

    int x1, y1, x2, y2;
    while (fread(&x1, sizeof(int), 1, f) == 1 &&
           fread(&y1, sizeof(int), 1, f) == 1 &&
           fread(&x2, sizeof(int), 1, f) == 1 &&
           fread(&y2, sizeof(int), 1, f) == 1) {

        Vertice* v1 = ProcurarVertice(g, x1, y1);
        Vertice* v2 = ProcurarVertice(g, x2, y2);
        if (v1 && v2) {
            bool rs = false;
            AdicionarAresta(g, v1->x, v1->y, v2->x, v2->y, &rs);
        }
    }

    fclose(f);
    return g;
}

/**
 * @brief Guarda as arestas do grafo num ficheiro binário.
 * 
 * Percorre todos os vértices do grafo e escreve no ficheiro as
 * arestas, evitando duplicação ao gravar apenas quando o id do
 * vértice de origem é menor que o id do vértice destino.
 * 
 * Cada aresta é representada por quatro inteiros correspondentes às
 * coordenadas (x, y) do vértice origem e do vértice destino.
 * 
 * @param g Ponteiro para o grafo cujas arestas serão guardadas.
 * @param nomeFicheiro Nome do ficheiro binário onde as arestas serão escritas.
 * 
 * @return true se a operação for bem sucedida, false caso contrário.
 */

bool GuardarArestasBinario(Grafo* g, const char* nomeFicheiro) {
    FILE* f = fopen(nomeFicheiro, "wb");
    if (!f) {
        return false;
    }

    Vertice* v = g->vertices;
    while (v) {
        Aresta* a = v->arestas;
        while (a) {
            if (v->id < a->destino->id) {
                fwrite(&v->x, sizeof(int), 1, f);
                fwrite(&v->y, sizeof(int), 1, f);
                fwrite(&a->destino->x, sizeof(int), 1, f);
                fwrite(&a->destino->y, sizeof(int), 1, f);
            }
            a = a->prox;
        }
        v = v->prox;
    }

    fclose(f);
    return true;
}
