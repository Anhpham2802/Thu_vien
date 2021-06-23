#include<stdio.h>
#include"graph_color.h"
int main()
{
    int Edge[2][Max];
    int matran[Max][Max] = {0};
    int v[Max];
    char C_name[Max][Max];
    int numC =0;
    int numV,numE;
    readfile("input.txt",Edge,&numV,&numE);
    printf("intput\n");
    hienThi_Input(Edge,numV,numE);
    maTran_Ke(matran,numE,Edge);
    hienThiMatrix(matran,numV);
    numC = to_mau(matran,numV,v);
    readfilecolor("colorname.txt",C_name,numC);
    write_file("hienthi.dot",C_name,Edge,numV,numE,v);

    return 0;
}