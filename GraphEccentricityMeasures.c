// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphEccentricityMeasures
//

#include "GraphEccentricityMeasures.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "Graph.h"
#include "GraphAllPairsShortestDistances.h"

struct _GraphEccentricityMeasures {
  unsigned int*
      centralVertices;  // centralVertices[0] = number of central vertices
                        // array size is (number of central vertices + 1)
  int* eccentricity;    // the eccentricity value of each vertex
  Graph* graph;         // the graph
  int graphRadius;      // the graph radius
  int graphDiameter;    // the graph diameter
};

// Allocate memory
// Compute the vertex eccentricity values
// Compute graph radius and graph diameter
// Compute the set of central vertices
GraphEccentricityMeasures* GraphEccentricityMeasuresCompute(Graph* g) {
  assert(g != NULL);

  unsigned int numVertices = GraphGetNumVertices(g);
  assert(numVertices > 0);

  // Aloca memória para o resultado
  GraphEccentricityMeasures* measures = (GraphEccentricityMeasures*)malloc(sizeof(GraphEccentricityMeasures));
  assert(measures != NULL);

  measures->eccentricity = (int*)malloc(numVertices * sizeof(int));
  assert(measures->eccentricity != NULL);

  measures->centralVertices = NULL;
  measures->graph = g;

  int radius = INT_MAX, diameter = -1;

  // Calcula as distâncias de todos os pares
  GraphAllPairsShortestDistances* distances = GraphAllPairsShortestDistancesExecute(g);
  assert(distances != NULL);

  // Calcula a excentricidade de cada vértice e atualiza o raio e diâmetro
  for (unsigned int u = 0; u < numVertices; u++) {
    int maxDistance = -1;
    for (unsigned int v = 0; v < numVertices; v++) {
      int dist = GraphGetDistanceVW(distances, u, v);
      if (dist > maxDistance) maxDistance = dist;
    }

    measures->eccentricity[u] = maxDistance;
    if (maxDistance != -1) {
      if (maxDistance < radius) radius = maxDistance;
      if (maxDistance > diameter) diameter = maxDistance;
    }
  }

  measures->graphRadius = radius;
  measures->graphDiameter = diameter;

  // Calcula os vértices centrais
  unsigned int centralCount = 0;
  for (unsigned int u = 0; u < numVertices; u++) {
    if (measures->eccentricity[u] == radius) centralCount++;
  }

  measures->centralVertices = (unsigned int*)malloc((centralCount + 1) * sizeof(unsigned int));
  assert(measures->centralVertices != NULL);

  measures->centralVertices[0] = centralCount;
  unsigned int index = 1;
  for (unsigned int u = 0; u < numVertices; u++) {
    if (measures->eccentricity[u] == radius) measures->centralVertices[index++] = u;
  }

  // Libera as distâncias de todos os pares
  GraphAllPairsShortestDistancesDestroy(&distances);

  return measures;
}


void GraphEccentricityMeasuresDestroy(GraphEccentricityMeasures** p) {
  assert(*p != NULL);

  GraphEccentricityMeasures* aux = *p;

  free(aux->centralVertices);
  free(aux->eccentricity);

  free(*p);
  *p = NULL;
}

// Getting the computed measures

int GraphGetRadius(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphRadius;
}

int GraphGetDiameter(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  return p->graphDiameter;
}

int GraphGetVertexEccentricity(const GraphEccentricityMeasures* p,
                               unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));
  assert(p->eccentricity != NULL);

  return p->eccentricity[v];
}

// Getting a copy of the set of central vertices
// centralVertices[0] = number of central vertices in the set
unsigned int* GraphGetCentralVertices(const GraphEccentricityMeasures* p) {
  assert(p != NULL && p->centralVertices != NULL);

  // Cria uma cópia dos vértices centrais
  unsigned int centralCount = p->centralVertices[0];
  unsigned int* centralCopy = (unsigned int*)malloc((centralCount + 1) * sizeof(unsigned int));
  assert(centralCopy != NULL);

  // Copia os vértices centrais para a nova lista usando um loop
  for (unsigned int i = 0; i <= centralCount; i++) {
    centralCopy[i] = p->centralVertices[i];
  }

  return centralCopy;
}


// Print the graph radius and diameter
// Print the vertex eccentricity values
// Print the set of central vertices
void GraphEccentricityMeasuresPrint(const GraphEccentricityMeasures* p) {
  assert(p != NULL);

  // Imprime o raio e diâmetro do grafo
  printf("Graph Radius: %d\nGraph Diameter: %d\n", p->graphRadius, p->graphDiameter);

  unsigned int numVertices = GraphGetNumVertices(p->graph);
  printf("Vertex Eccentricities:\n");
  
  // Imprime as excentricidades dos vértices
  for (unsigned int i = 0; i < numVertices; i++) {
    printf("  Vertex %u: %d\n", i, p->eccentricity[i]);
  }

  // Imprime os vértices centrais
  printf("Central Vertices: ");
  for (unsigned int i = 1, centralCount = p->centralVertices[0]; i <= centralCount; i++) {
    printf("%u ", p->centralVertices[i]);
  }
  printf("\n");
}
