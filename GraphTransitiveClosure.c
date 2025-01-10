//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphTransitiveClosure - Transitive Closure of a directed graph
//

// Student Name :
// Student Number :
// Student Name :
// Student Number :

/*** COMPLETE THE GraphComputeTransitiveClosure FUNCTION ***/

#include "GraphTransitiveClosure.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphBellmanFordAlg.h"
#include "instrumentation.h"

// Compute the transitive closure of a directed graph
// Return the computed transitive closure as a directed graph
// Use the Bellman-Ford algorithm
Graph* GraphComputeTransitiveClosure(Graph* g) {
  assert(g != NULL && GraphIsDigraph(g) && GraphIsWeighted(g) == 0);

  unsigned int numVertices = GraphGetNumVertices(g);

  // Cria o grafo de fechamento transitivo
  Graph* transitiveClosure = GraphCreate(numVertices, 1, 0);

  // Executa o Bellman-Ford para cada vértice
  for (unsigned int u = 0; u < numVertices; u++) {
    GraphBellmanFordAlg* bfAlg = GraphBellmanFordAlgExecute(g, u);
    if (!bfAlg) {
      fprintf(stderr, "Erro ao calcular Bellman-Ford para o vértice %u\n", u);
      GraphDestroy(&transitiveClosure);
      return NULL;
    }

    // Adiciona as arestas para os vértices alcançáveis, ignorando laços
    for (unsigned int v = 0; v < numVertices; v++) {
      if (u != v && GraphBellmanFordAlgReached(bfAlg, v)) {
        GraphAddEdge(transitiveClosure, u, v);
      }
    }

    // Libera memória do Bellman-Ford
    GraphBellmanFordAlgDestroy(&bfAlg);
  }

  return transitiveClosure;
}
