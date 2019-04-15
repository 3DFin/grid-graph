/*=============================================================================
 * [first_edge, adj_vertices, connectivities] = grid_to_graph_mex(shape,
 *      connectivity = 1)
 * 
 *  Hugo Raguet 2019
 *===========================================================================*/
#include <cstdint>
#include "mex.h"
#include "../../include/grid_graph.hpp"

typedef uint8_t conn_t;
# define mxCONN_CLASS mxUINT8_CLASS
# define mxCONN_ID "uint8"

/* template for handling several index types */
template <typename index_t, mxClassID mxINDEX_CLASS>
static void grid_to_graph_mex(int nlhs, mxArray **plhs, int nrhs,
    const mxArray **prhs)
{
    /* retrieve inputs */
    size_t D = mxGetNumberOfElements(prhs[0]);
    index_t* shape = (index_t*) mxGetData(prhs[0]);
    uint8_t connectivity = nrhs > 1 ? mxGetScalar(prhs[1]) : 1;

    /* retrieve sizes */
    size_t E = num_edges_grid_graph<index_t, conn_t>(D, shape, connectivity);
    index_t V = 1;
    for (size_t d = 0; d < D; d++){ V *= shape[d]; }

    /* retrieve edges and connectivity */
    index_t* edges = (index_t*) mxMalloc(sizeof(index_t)*2*E);

    conn_t* connectivities = nullptr;
    if (nlhs > 2){
        plhs[2] = mxCreateNumericMatrix(1, E, mxCONN_CLASS, mxREAL);
        connectivities = (conn_t*) mxGetData(plhs[2]);
    }

    adjacency_grid_graph<index_t, conn_t>(D, shape, connectivity, edges,
        connectivities); 

    /* convert to forward star representation */
    plhs[0] = mxCreateNumericMatrix(1, V + 1, mxINDEX_CLASS, mxREAL);
    index_t* first_edge = (index_t*) mxGetData(plhs[0]);

    index_t* reindex = (index_t*) mxMalloc(sizeof(index_t)*E);

    adjacency_to_forward_star<index_t, index_t>(V, E, (const index_t*) edges,
        first_edge, reindex);

    /* permute ending vertices to get adjacent vertices */
    plhs[1] = mxCreateNumericMatrix(1, E, mxINDEX_CLASS, mxREAL);
    index_t* adj_vertices = (index_t*) mxGetData(plhs[1]);
    for (size_t e = 0; e < E; e++){
        adj_vertices[reindex[e]] = edges[2*e + 1];
    }
    
    /* permute connectivity correspondingly (reuse edges storage) */
    if (nlhs > 2){ 
        for (size_t e = 0; e < E; e++){
            edges[reindex[e]] = connectivities[e];
        }
        for (size_t e = 0; e < E; e++){ connectivities[e] = edges[e]; }
    }

    mxFree((void*) edges);
    mxFree((void*) reindex);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{ 
    if (mxGetClassID(prhs[0]) == mxUINT16_CLASS){
        grid_to_graph_mex<uint16_t, mxUINT16_CLASS>(nlhs, plhs, nrhs, prhs);
    }else if (mxGetClassID(prhs[0]) == mxUINT32_CLASS){
        grid_to_graph_mex<uint32_t, mxUINT32_CLASS>(nlhs, plhs, nrhs, prhs);
    }else if (mxGetClassID(prhs[0]) == mxUINT64_CLASS){
        grid_to_graph_mex<uint64_t, mxUINT64_CLASS>(nlhs, plhs, nrhs, prhs);
    }else{
        mexErrMsgIdAndTxt("MEX", "Grid to graph: argument 'shape' must be of "
            "an unsigned integer (16, 32, 64) class (%s given).",
            mxGetClassName(prhs[0]));
    }
}
