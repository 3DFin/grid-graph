## Some tools for manipulating nearest-neighbors graphs defined on regular grids

C++ routines.  
Parallelization with OpenMP.  
Mex interfaces for GNU Octave or Matlab.  

A grid graph in dimension _D_ is defined by the following parameters:  

 - `D` - the number of dimensions  
 - `shape` - array of length `D`, giving the grid size in each dimension
 - `connectivity` - defines the neighboring relationship;  
     corresponds to the square of the maximum Euclidean distance between two
     neighbors;
     if less than 4, it defines the number of coordinates allowed
     to simultaneously vary (+1 or -1) to define a neighbor; (in that case,
     each level ℓ of connectivity in dimension _D_ adds
        C<sub>ℓ</sub><sup>_D_</sup>⨯2<sup>ℓ</sup>
     neighbors).  
     Corresponding number of neighbors for `D` = 2 and 3:  

     |connectivity |   1|   2|   3|
     |-------------|---:|---:|---:|
     |          2D |   4|   8| (8)|
     |          3D |   6|  18|  26|

     Note that a connectivity of 4 or more includes neighbors whose
     coordinates might differ by 2 or more from the coordinates of the
     considered vertex.
     In that sense, in dimension 4 or more, including all immediately
     surrounding vertices (that is, all vertices for which each coordinate 
     differ by 1 or less) would then also include vertices from a more
     distant surround: the neighbor _v_ + (2, 0, 0, 0) is at the same
     distance as the neighbor _v_ + (1, 1, 1, 1).

A graph with _V_ vertices and _E_ edges is represented either as adjacency
list (array of _E_ edges given as ordered pair of vertices), or as
forward-star, where edges are numeroted (from 0 to _E_ − 1) so that all edges
originating from a same vertex are consecutive, and represented by the
following parameters:    

 - `first_edge` - array of length _V_ + 1, indicating for each vertex, the
    first edge starting from the vertex (or, if there are none, starting from
    the next vertex); the last value is always the total number of edges  
 - `adj_vertices` - array of length _E_, indicating for each edge, its ending
    vertex  

Vertices of the grid are indexed in column-major order, that is consecutive 
vertices along the first dimension gets consecutive indices, then consecutive
columns, and so on along further dimensions.

Work possibly in parallel with OpenMP API  

### Directory tree
    .   
    ├── include/    C++ headers, with some doc  
    ├── octave/     GNU Octave or Matlab code  
    │   └── mex/    MEX C++ interfaces
    └── src/        C++ sources  

See `octave/compile_mex.m` for typical compilation commands; it can be run directly from the GNU Octave interpreter.  

### References and license
This software is under the GPLv3 license.  

Hugo Raguet 2019  
