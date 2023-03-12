/**
 * Brinzoi Ion-Robert
 * 30221
 *
 * Evaluarea BFS 
 * 
 * Complexitatea algoritmului BFS este O(V + E), unde V este numarul varfurilor si E este numarul muchiilor.
 *
 **/
#include <stdlib.h>
#include <string.h>
#include "bfs.h"

void enQ(Queue* k, Node* h) {
    Qnode* tmp = (Qnode*)(malloc(sizeof(Qnode)));
    tmp->nxt = NULL;
    tmp->prv = NULL;
    tmp->Gnode = h;
    if (k->hd == NULL && k->tl == NULL)
    {
        k->hd = tmp;
        k->tl = tmp;
    }
    else
    {
        k->tl->nxt = tmp;
        tmp->prv = k->tl;
        k->tl = tmp;
    }
}

Node* deQ(Queue* k) {
    if (k->hd == NULL && k->tl == NULL)
        return NULL;
    else if (k->hd == k->tl) {
        Node* tmp = k->hd->Gnode;
        k->hd = NULL;
        k->tl = NULL;
        return tmp;
    }
    else
    {
        Node* tmp = k->hd->Gnode;
        k->hd->nxt->prv = NULL;
        k->hd = k->hd->nxt;
        return tmp;
    }
}



int get_neighbors(const Grid *grid, Point p, Point neighb[])
{
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4
    int current_idx = 0;
    if (p.row - 1 >= 0) {
        if (grid->mat[p.row - 1][p.col] == 0) {
            neighb[current_idx] = { p.row - 1,p.col };
            current_idx++;
        }
    }
    if (p.row + 1 < grid->rows) {
        if (grid->mat[p.row + 1][p.col] == 0) {
            neighb[current_idx] = { p.row + 1,p.col };
            current_idx++;
        }
    }
    if (p.col - 1 >= 0) {
        if (grid->mat[p.row][p.col - 1] == 0) {
            neighb[current_idx] = { p.row,p.col - 1 };
            current_idx++;
        }
    }
    if (p.col + 1 < grid->cols) {
        if (grid->mat[p.row][p.col + 1] == 0) {
            neighb[current_idx] = { p.row,p.col + 1 };
            current_idx++;
        }
    }
    return current_idx;
}

void grid_to_graph(const Grid *grid, Graph *graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(grid->mat[i][j] == 0){
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }else{
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for(i=0; i<grid->rows; ++i){
        for(j=0; j<grid->cols; ++j){
            if(nodes[i][j] != NULL){
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for(i=0; i<graph->nrNodes; ++i){
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if(graph->v[i]->adjSize != 0){
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for(j=0; j<graph->v[i]->adjSize; ++j){
                if( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0){
                        graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if(k < graph->v[i]->adjSize){
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
    if(graph->v != NULL){
        for(int i=0; i<graph->nrNodes; ++i){
            if(graph->v[i] != NULL){
                if(graph->v[i]->adj != NULL){
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void bfs(Graph *graph, Node *s, Operation *op)
{
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();
    for (int i = 0; i < graph->nrNodes; i++) {
            graph->v[i]->color = COLOR_WHITE;
            if (op != NULL) op->count(1);
    }
    s->color = COLOR_GRAY;
    s->dist = 0;
    s->parent = NULL;
    if (op != NULL) op->count(3);

    Queue* k = (Queue*)malloc(sizeof(Queue));
    k->hd = NULL;
    k->tl = NULL;

    if(op!=NULL) op->count();
    enQ(k, s);


    while (k->tl != NULL) {
        if (op != NULL) op->count();

        Node* crt = deQ(k);
        if (op != NULL) op->count();

        for (int l = 0; l < crt->adjSize; l++) {
            if (op != NULL) op->count();
            if (crt->adj[l]->color == COLOR_WHITE) {
                if(op != NULL) op->count(4);
                crt->adj[l]->color = COLOR_GRAY;
                (crt->adj[l]->dist) = crt->dist + 1;
                crt->adj[l]->parent = crt;
                enQ(k, crt->adj[l]);
            }
        }
        crt->color = COLOR_BLACK;
        if (op != NULL) op->count();
    }
}

void print_rprz_R1(int* s,Point* repr, int s_size, int lookingFor, int mt_spaces) {
    for (int i = 0; i < s_size; i++)
        if (s[i] == lookingFor) {
            for (int i = 0; i < mt_spaces; i++)
                printf("\t");
            printf("(%d,%d)\n", repr[i].row,repr[i].col);
            print_rprz_R1(s,repr,s_size, i, mt_spaces + 1);
        }
}

void print_bfs_tree(Graph *graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for(int i=0; i<graph->nrNodes; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            transf[i] = n;
            ++n;
        }else{
            transf[i] = -1;
        }
    }
    if(n == 0){
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for(int i=0; i<graph->nrNodes && !err; ++i){
        if(graph->v[i]->color == COLOR_BLACK){
            if(transf[i] < 0 || transf[i] >= n){
                err = 1;
            }else{
                repr[transf[i]] = graph->v[i]->position;
                if(graph->v[i]->parent == NULL){
                    p[transf[i]] = -1;
                }else{
                    err = 1;
                    for(int j=0; j<graph->nrNodes; ++j){
                        if(graph->v[i]->parent == graph->v[j]){
                            if(transf[j] >= 0 && transf[j] < n){
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if(!err){
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs
        print_rprz_R1(p, repr, n, -1, 0);
    }

    if(p != NULL){
        free(p);
        p = NULL;
    }
    if(repr != NULL){
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[])
{
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000
    return -1;
}

int checkr(Graph* gr,int first,int last) {
    for (int i = 0; i < gr->v[first]->adjSize; i++) {
        if (gr->v[first]->adj[i] == gr->v[last]) {
            return 0;
        }
    }
    return 1;
}

void crt_rnd_graph(Graph* gr, int nr_edg, int nr_v) {
    int* s = (int*)malloc(nr_v * sizeof(int));
    FillRandomArray(s, nr_v, 0, nr_v - 1, true);
    for (int i = 0; i < nr_v; i++) {
        gr->v[i]->adjSize = 0;
        gr->v[i]->adj = (Node**)malloc(sizeof(Node*) * nr_v);
    }

    for (int i = 0; i < nr_v - 1; i++) {
        gr->v[s[i]]->adj[gr->v[s[i]]->adjSize] = gr->v[s[i + 1]];
        (gr->v[s[i]]->adjSize)++;
        gr->v[s[i + 1]]->adj[gr->v[s[i + 1]]->adjSize] = gr->v[s[i]];
        (gr->v[s[i + 1]]->adjSize)++;
    }

    free(s);
    int remain = nr_edg - nr_v + 1;
    int first;
    int last;
    int checker = 0;
    for (int i = 0; i < remain; i++) {
        first = rand() % nr_v;
        last = rand() % nr_v;
        if (first != last && checkr(gr,first,last) && checkr(gr,last,first)) {

            gr->v[first]->adj[gr->v[first]->adjSize] = gr->v[last];
            (gr->v[first]->adjSize)++;
            gr->v[last]->adj[gr->v[last]->adjSize] = gr->v[first];
            (gr->v[last]->adjSize)++;
        }
        else
            i--;

    }
}
    



void performance()
{
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for(n=1000; n<=4500; n+=100){
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected


        crt_rnd_graph(&graph, n, graph.nrNodes);
        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for (n = 100; n <= 200; n += 10) {
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for(i=0; i<graph.nrNodes; ++i){
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected

        crt_rnd_graph(&graph, 4500, graph.nrNodes);
        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}
