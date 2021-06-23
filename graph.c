#include "jrb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dllist.h"
#define SIZE_MAX 1000
typedef Dllist Queue;
Queue q;

void Enqueue(Queue *q, Jval jval)
{
    dll_append(*q, jval);
}

int Dequeue(Queue *q)
{
    Queue tmp = dll_first(*q);
    int v = jval_i(tmp->val);
    dll_delete_node(tmp);
    return v;
}

/*
typedef JRB Graph;
*/
typedef struct
{
    JRB edges;
    JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2);
int hasEdge(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int *output);
int outdegree(Graph graph, int v, int *output);
void BFS(Graph graph, int start, int stop, void (*func)(int));
void DFS(Graph graph, int start, int stop, void (*func)(int));
int DAG(Graph graph);
void TSort(Graph g, int output[], int *n);
double shortestPath(Graph graph, int s, int t, int* path, int*length);
void dropGraph(Graph graph);

int main(int argc, char const *argv[])
{
    Graph g = createGraph();
    q = new_dllist();
    int output[SIZE_MAX];
    int n = 0;

    addVertex(g, 0, "CS102");
    addVertex(g, 1, "CS140");
    addVertex(g, 2, "CS160");
    addVertex(g, 3, "CS302");
    addVertex(g, 4, "CS311");
    addVertex(g, 5, "MATH300");
    addEdge(g, 0, 1);
    addEdge(g, 0, 2);
    addEdge(g, 1, 3);
    addEdge(g, 5, 4);
    addEdge(g, 3, 4);

    n = outdegree(g, 1, output);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", output[i]);
    }
    
    printf("\n");

    printf("%s\n", (DAG(g)) ? "Cycle" : "Not Cycle");
    TSort(g, output, &n);

    printf("The topological order:\n");
    for (int i = 0; i < n; i++)
        printf("%s\n", getVertex(g, output[i]));

    dropGraph(g);
    printf("\n");
    return 0;
}

Graph createGraph()
{
    Graph g;
    g.edges = make_jrb();
    g.vertices = make_jrb();
    return g;
}

void addVertex(Graph g, int v1, char *name)
{
    JRB Node = jrb_find_int(g.vertices, v1);

    if (Node == NULL)
    {
        jrb_insert_int(g.vertices, v1, new_jval_s(name));
    }
}

char *getVertex(Graph graph, int id)
{
    JRB Node = jrb_find_int(graph.vertices, id);

    if (Node != NULL)
    {
        return jval_s(Node->val);
    }
}

void addEdge(Graph g, int v1, int v2)
{
    JRB Node = jrb_find_int(g.edges, v1);
    JRB tree;
    if (Node == NULL)
    {
        tree = make_jrb();
        jrb_insert_int(g.edges, v1, new_jval_v(tree));
    }
    else
    {
        tree = (JRB)jval_v(Node->val);
    }
    jrb_insert_int(tree, v2, new_jval_i(1));
}

int hasEdge(Graph graph, int v1, int v2)
{
    JRB Node = jrb_find_int(graph.edges, v1);
    if (Node != NULL)
    {
        JRB tmp = jrb_find_int(jval_v(Node->val), v2);
        if (tmp != NULL)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

int indegree(Graph graph, int v, int *output)
{
    int re = 0;
    JRB tmp, tmp1;
    JRB tree;
    jrb_traverse(tmp, graph.edges)
    {
        tree = (JRB)jval_v(tmp->val);
        jrb_traverse(tmp1, tree)
        {
            if (jval_i(tmp1->key) == v)
            {
                output[re++] = jval_i(tmp->key);
            }
        }
    }
    return re;
}

int outdegree(Graph graph, int v, int *output)
{
    int re = 0;

    JRB Node = jrb_find_int(graph.edges, v);
    JRB tmp;
    if (Node != NULL)
    {
        JRB tree = (JRB)jval_v(Node->val);
        jrb_traverse(tmp, tree)
        {
            output[re++] = jval_i(tmp->key);
        }
    }

    return re;
}

void BFS(Graph graph, int start, int stop, void (*func)(int))
{
   int visited[1000] = {};
   int n, output[100], i, u, v;
   Dllist node, queue;
   
   queue = new_dllist();
   dll_append(queue, new_jval_i(start));

   while ( !dll_empty(queue) )
   {
      node = dll_first(queue);
      u = jval_i(node->val);
      dll_delete_node(node);
      if (!visited[u]) 
      {
          func(u);
          visited[u] = 1;
          if ( u == stop ) return;           
          n = getAdjacentVertices(graph, u, output);
          for (i=0; i<n; i++)
          {
              v = output[i];
              if (!visited[v])
                 dll_append(queue, new_jval_i(v));
          }
      }
   }                            
}

void DFS(Graph graph, int start, int stop, void (*func)(int))
{
   int visited[1000] = {};
   int n, output[100], i, u, v;
   Dllist node, stack;
   
   stack = new_dllist();
   dll_append(stack, new_jval_i(start));

   while ( !dll_empty(stack) )
   {
      node = dll_last(stack);
      u = jval_i(node->val);
      dll_delete_node(node);
      if (!visited[u]) 
      {
          func(u);
          visited[u] = 1;
          if ( u == stop ) return;           
          n = getAdjacentVertices(graph, u, output);
          for (i=0; i<n; i++)
          {
              v = output[i];
              if (!visited[v])
                 dll_append(stack, new_jval_i(v));
          }
      }
   }                            
}

int DAG(Graph graph)
{
    int visited[SIZE_MAX];
    int u, v;
    int output[SIZE_MAX];
    int re, start;
    JRB vertex;

    jrb_traverse(vertex, graph.vertices)
    {
        memset(visited, 0, sizeof(visited));
        Enqueue(&q, vertex->key);
        start = jval_i(vertex->key);

        while (!dll_empty(q))
        {
            u = Dequeue(&q);
            if (!visited[u])
            {
                visited[u] = 1;
                re = outdegree(graph, u, output);
                for (int i = 0; i < re; i++)
                {
                    v = output[i];
                    if (v == start)
                    {
                        return 1;
                    }
                    if (visited[v] == 0)
                    {
                        Enqueue(&q, new_jval_i(v));
                    }
                }
            }
        }
    }
    return 0;
}

void TSort(Graph g, int output[], int *n)
{
    *n = 0;
    JRB tmp;
    int v;
    int out[SIZE_MAX];
    int indeg[SIZE_MAX] = {0};
    jrb_traverse(tmp, g.vertices)
    {
        indeg[jval_i(tmp->key)] += indegree(g, jval_i(tmp->key), out);
        if (indeg[jval_i(tmp->key)] == 0)
        {
            Enqueue(&q, tmp->key);
        }
    }

    while (!dll_empty(q))
    {
        v = Dequeue(&q);
        output[(*n)++] = v;
        int k = outdegree(g, v, out);
        for (int i = 0; i < k; i++)
        {
            indeg[out[i]]--;
            if (indeg[out[i]] == 0)
                Enqueue(&q, new_jval_i(out[i]));
        }
    }
}

double shortestPath(Graph g, int s, int t, int* path, int*length)
{
   double distance[1000], min, w, total;
   int previous[1000], tmp[1000];
   int n, output[100], i, u, v, start;
   Dllist ptr, queue, node;

   for (i=0; i<1000; i++)
       distance[i] = INFINITIVE_VALUE;
   distance[s] = 0;
   previous[s] = s;
       
   queue = new_dllist();
   dll_append(queue, new_jval_i(s));

   while ( !dll_empty(queue) )
   {
      // get u from the priority queue   
      min = INFINITIVE_VALUE;   
      dll_traverse(ptr, queue)
      {
          u = jval_i(ptr->val);              
          if (min > distance[u])
          {
             min = distance[u];
             node = ptr;
          }                 
      }
      dll_delete_node(node);
      
      if (u == t) break; // stop at t
      
      n = outdegree(g, u, output);
      for (i=0; i<n; i++)
      {
          v = output[i];
          w = getEdgeValue(g, u, v);
          if ( distance[v] > distance[u] + w )
          {    
              distance[v] = distance[u] + w;
              previous[v] = u;
          }     
          dll_append(queue, new_jval_i(v));
      }
   }
   total = distance[t]; 
   if (total != INFINITIVE_VALUE)
   {
       tmp[0] = t;
       n = 1;              
       while (t != s)
       {
             t = previous[t];
             tmp[n++] = t;
       }
       for (i=n-1; i>=0; i--)
           path[n-i-1] = tmp[i];
       *length = n;                
   }
   return total;   
}

void dropGraph(Graph g)
{
    JRB tmp, tree;
    jrb_traverse(tmp, g.edges)
    {
        tree = (JRB)jval_v(tmp->val);
        jrb_free_tree(tree);
    }

    jrb_free_tree(g.edges);
    jrb_free_tree(g.vertices);
}