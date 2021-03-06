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
int DAG(Graph graph);
void TSort(Graph g, int output[], int *n);
void dropGraph(Graph graph);
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