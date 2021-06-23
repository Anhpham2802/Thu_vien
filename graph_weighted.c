#include<stdio.h>
#include"graph_weight.h"
int main()
{
    int i, length, path[100], s, t;
    double w;
    Graph g = createGraph();// tao mot do thi g
    addVertex(g, 0, "V0");// add dinh V0 c� gia tri la 0
    addVertex(g, 1, "V1");// add dinh V1 co gia tri la 1
    addVertex(g, 2, "V2");// add dinh V2 co gia tri la 2
    addVertex(g, 3, "V3");// add dinh V3 co gia tri la 3
    addEdge(g, 0, 1, 1);// add canh 0-1 co trong so 1
    addEdge(g, 1, 2, 3);// add canh 1-2 co trong so 3
    addEdge(g, 2, 0, 3);// add canh 2-0 co trong so 3
    addEdge(g, 1, 3, 1);// add canh 1-3 co trong so 1
    addEdge(g, 3, 2, 1);// add canh 3-2 co trong so 1
    s = 0;
    t = 2;
    w = shortestPath(g, s, t, path, &length);// tim duong di ngan nhat tu s den t
    if (w == INFINITIVE_VALUE)
    {
         printf("No path from %s to %s\n", getVertex(g, s), getVertex(g, t));
    }else
    {
         printf("Path from %s to %s (with total distance %f)\n", getVertex(g, s), getVertex(g, t), w);
         for (i=0; i<length; i++)
             printf(" => %s", getVertex(g, path[i]));
         printf("\n");
    }
    
    dropGraph(g);   
}
