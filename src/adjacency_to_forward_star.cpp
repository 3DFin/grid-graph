/*=============================================================================
 * Hugo Raguet 2019
 *===========================================================================*/
#include <cstdint>
#include "../include/grid_graph.hpp"
#include "../include/omp_num_threads.hpp"

template <typename vertex_t, typename edge_t>
void adjacency_to_forward_star(vertex_t V, size_t E, const vertex_t* edges,
    edge_t* first_edge, edge_t* reindex)
{
    /* compute number of edges for each vertex */
    for (vertex_t v = 0; v < V; v++){ first_edge[v] = 0; }
    for (size_t e = 0; e < E; e++){ reindex[e] = first_edge[edges[2*e]]++; }

    /* compute cumulative sum and shift to the right */
    edge_t sum = 0; // first_edge[0] is always 0
    for (vertex_t v = 0; v <= V; v++){
        edge_t tmp = first_edge[v];
        first_edge[v] = sum;
        sum += tmp;
    } // first_edge[V] should be total number of edges

    /* finalize reindex */
    #pragma omp parallel for NUM_THREADS(E)
    for (size_t e = 0; e < E; e++){ reindex[e] += first_edge[edges[2*e]]; }
}

/* instantiate for compilation */
template void adjacency_to_forward_star<uint16_t, uint16_t>(uint16_t V,
    size_t E, const uint16_t* edges, uint16_t* first_edge, uint16_t* reindex);
template void adjacency_to_forward_star<uint32_t, uint32_t>(uint32_t V,
    size_t E, const uint32_t* edges, uint32_t* first_edge, uint32_t* reindex);
template void adjacency_to_forward_star<uint64_t, uint64_t>(uint64_t V,
    size_t E, const uint64_t* edges, uint64_t* first_edge, uint64_t* reindex);
