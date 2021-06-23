#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<string.h>
#include<stdlib.h>
#define Max 10000
void readfile(char *f,int Edge[2][Max],int *numV, int *numE);
void hienThi_Input(int Edge[2][Max],int numV,int numE);
void maTran_Ke(int matran[Max][Max],int numE,int Edge[2][Max]);
void hienThiMatrix(int matran[Max][Max],int numV);
int to_duoc(int matran[Max][Max],int numV,int v[],int i,int c);
int to_1_mau(int matran[Max][Max],int numV,int v[],int color);
int to_mau(int matran[Max][Max],int numV,int v[]);
void readfilecolor(char *f,char C_name[Max][Max],int numC);
void write_file(char *f,char C_name[Max][Max],int Edge[2][Max],int numV,int numE,int v[]);

// docfile txt dau vao
void readfile(char *f,int Edge[2][Max],int *numV, int *numE)
{
    FILE *fp;
    fp = fopen(f,"r");
    if(fp == NULL)
    {
        printf("file input khong ton tai !!!\n");
        return;
    }
    fscanf(fp,"%d",numV);
    fscanf(fp,"%d",numE);
    int i;
    for(i=0;i<*numE;i++)
    {
        fscanf(fp,"%d",&Edge[0][i]);
        fscanf(fp,"%d",&Edge[1][i]);
    }
    fclose(fp);
}
void hienThi_Input(int Edge[2][Max],int numV,int numE)
{
    printf("%d\t%d\n",numV,numE);
    int i;
    for(i=0;i<numE;i++)
    {
        printf("%d\t%d\n",Edge[0][i],Edge[1][i]);
    }
}
void maTran_Ke(int matran[Max][Max],int numE,int Edge[2][Max])
{
    int i;
    for(i=0;i<numE;i++)
    {
        matran[Edge[0][i]][Edge[1][i]] = 1;
        matran[Edge[1][i]][Edge[0][i]] = 1;
    }
}
void hienThiMatrix(int matran[Max][Max],int numV)
{
    int i,j;
    for(i=1;i<=numV;i++)
    {
        for(j=1;j<=numV;j++)
        {
            printf("%5d",matran[i][j]);
        }
        printf("\n");
    }
}
int to_duoc(int matran[Max][Max],int numV,int v[],int i,int c)
{
    int j;
    for(j=1;j<=numV;j++)
    {
        if(matran[i][j] && v[j] == c)
            return 0;
    }
    return 1;
}
int to_1_mau(int matran[Max][Max],int numV,int v[],int color)
{
    int i;
    int count =0;
    for(i=1;i<=numV;i++)
    {
        if(!v[i] && to_duoc(matran,numV,v,i,color))
        {
            v[i] = color;
            count ++;
        }
    }
    return count;
}
int to_mau(int matran[Max][Max],int numV,int v[])
{
    int i;
    for(i=1;i<= numV;i++)
        v[i] = 0;
    int numC =0;// chua co dinh nao to ---- so mau gan bang 0
    int count = 0;// so dinh duoc to
    while(count < numV)
    {
        numC ++;
        count += to_1_mau(matran,numV,v,numC);
    }
    return numC;
}
void readfilecolor(char *f,char C_name[Max][Max],int numC)
{
    FILE *fp;
    fp = fopen(f,"r");
    if(fp == NULL)
    {
        printf("khong ton tai filecolor !!\n");
        return;
    }
    int i;
    for(i=1;i<=numC;i++)
    {
        fscanf(fp,"%s",C_name[i]);
    }
    fclose(fp);
}
void write_file(char *f,char C_name[Max][Max],int Edge[2][Max],int numV,int numE,int v[])
{
    FILE *fp;
    fp = fopen(f,"wb");
    if(fp == NULL)
    {
        printf("khong the tao file !!!");
        return;
    }
    fprintf(fp,"graph\n{\n");
    int i;
    for(i=1;i<=numV;i++)
    {
        fprintf(fp,"%d [fillcolor = %s , style = filled];\n",i,C_name[v[i]]);
    }
    for(i=0;i<numE;i++)
    {
        fprintf(fp,"%d--%d;\n",Edge[0][i],Edge[1][i]);
    }
    fprintf(fp,"}");
    fclose(fp);
}