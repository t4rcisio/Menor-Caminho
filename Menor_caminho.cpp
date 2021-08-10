/*  
    Grafo - Menor caminho
    Aluno: Tarcísio Batista Prates
    AEDII - CEFET-MG
    2021

    Vetor de ponteiros de linhas separadas, método de aramzenamento de matriz dinâmica, é DIFERENTE do método
    de implementação de lista de adjacência, pois N é sempre igual a M, onde N e M são linhas e colunas da tabela.
    
                        _________________
    |-------------------|___|___|___|___|
    |   |---------------|___|___|___|___|
    |   |   |-----------|___|___|___|___|
    |   |   |   |-------|___|___|___|___|
    A   B   C   D   ... .
                        .
                        .   
    Função que determina o menor caminho Dijkstra.
    link "Codigo Dijkstra" : http://www2.dcc.ufmg.br/livros/algoritmos/cap7/codigo/c/7.22-dijkstramatriz.c

*/


#include <iostream>
#include <string>
#include <vector>
#include <climits>
#include <stdio.h>
#include <stdlib.h>
#include<limits.h>

#define MAXNUMVERTICES  100
#define MAXNUMARESTAS   4500
#define FALSE           0
#define TRUE            1
#define INFINITO       INT_MAX
#define LSIZE 50

typedef int TipoValorVertice;

typedef int TipoPeso;

typedef struct TipoGrafo {
  TipoPeso Mat[MAXNUMVERTICES + 1][MAXNUMVERTICES + 1];
  int  NumVertices;
  int  NumArestas;
} TipoGrafo;

typedef TipoValorVertice TipoApontador;
typedef int TipoIndice;
typedef struct TipoItem {
  TipoPeso Chave;
} TipoItem;

typedef TipoItem TipoVetor[MAXNUMVERTICES + 1];

TipoApontador Aux;
int  i, NArestas;
short FimListaAdj;
TipoValorVertice V1, V2, Adj;
TipoPeso Peso;
TipoGrafo Grafo;
TipoValorVertice NVertices;
TipoIndice n;   /*Tamanho do heap*/
TipoValorVertice Raiz;


using namespace std;

enum class State { White, Gray, Black }; //Estados de um vértice

class Edge{             //Abstração de uma aresta

private:
    TipoPeso  length;        //Para armazenar o comprimento da adjacência;
    TipoPeso  agility;       //Tempo para percorrer a adjacência;
    int  exist;         //Parâmetro que armazena 1 caso a adjacência exista, e 0 caso contrário;
    bool visibility;
    TipoValorVertice id;

public:
    
    Edge(){ 
        length = 0;
        agility = 0;
        exist = 0;
        visibility = false;
    }
    void setId(TipoValorVertice m_id){
        id = m_id;
    }
    TipoValorVertice* getId(){return &id;}

    void setLength(TipoPeso m_length){
        length = m_length;
    }
    TipoPeso* getLength(){return &length;}

    void setAgility(TipoPeso m_agility){
        agility = m_agility;
    }
    TipoPeso* getAgility(){return &agility;}

    void setEdge(){
        exist = 1;
    }
    void removeEdge(){
        exist = 0;
    }
    void setVisibility(bool m_visi){
        visibility = m_visi;
    }
    bool getVisibility(){
        return visibility;
    }
    int edge(){return exist;}
    void setDefault(){
        length = 0;
        agility = 0;
        exist = 0;
        visibility = false;
    }

};

class Vertex{           //Abstração de um vértice

public:
    string name;        //Nome para o vértice   
    vector<Edge> edge;  //Colunas da matriz
    State state;        //Estado do vértice
    TipoValorVertice id;
public:
    void setId(TipoValorVertice m_id){
        id = m_id;
    }
    TipoValorVertice* getId(){return &id;}
    Vertex(){
        state = State::White;
    }
    State getState(){
        return state;
    }
    void setState(State m_state){
        state = m_state;
    }
    void defautVertex(){
        state = State::White;
    }
    string getName(){
        return name;
    }


};


class XGraph {                                                      //Abstração de um grafo representado em uma matriz de adjacências

public:
    vector<Vertex> vertex;                                          //Linhas da matriz
    vector<Vertex> AGM;
    int n_edge;
    int AGM_weight;

private:
    void push_back_Edge(){                                          //Mantem o formato matricial quadrada
        Edge default_edge;
        //default_edge.setEdge();
        for(int i = 0; i< vertex.size(); i++){
            while(vertex[i].edge.size() < vertex.size()){
                vertex[i].edge.push_back(default_edge);
            }
        }
    }

    int vazia(){
        if(vertex.size() == 0){
            cout << "Matriz vazia" << endl;
            return 0;
        }
        return 1;
    }

public:

    void insertVertex(Vertex m_vertex){                             //Insere um elemento na tabela
        vertex.push_back(m_vertex);
        TipoValorVertice id = vertex.size()-1;
        vertex[vertex.size()-1].setId(id);
        push_back_Edge();                                           //Extende as colunas de cada linha

    }

    void printXGrafo(vector<Vertex> table){                                             //Imprime a tabela
      printf("-------------------------------------------\n");
        printf("   \t");
        for(int i = 0; i< table.size(); i++){
            cout << table[i].name << "\t"; 
        }
        printf("\n");
         for(int i = 0; i< table.size(); i++){
            cout << table[i].name << ":\t";
            for(int j = 0; j< table[i].edge.size(); j++){
                cout << *table[i].edge[j].getLength() << "\t";
            }
            printf("\n"); 
        }
        printf("\n"); 
    }
    void printXGrafo_Agility(vector<Vertex> table){                                             //Imprime a tabela
      printf("-------------------------------------------\n");
        printf("   \t");
        for(int i = 0; i< table.size(); i++){
            cout << table[i].name << "\t"; 
        }
        printf("\n");
         for(int i = 0; i< table.size(); i++){
            cout << table[i].name << ":\t";
            for(int j = 0; j< table[i].edge.size(); j++){
                cout << *table[i].edge[j].getAgility() << "\t";
            }
            printf("\n"); 
        }
        printf("\n"); 
    }

    void insertEdge(string descA, string descB, int Twoway, TipoPeso length, TipoPeso time){              //Grava na tabela a existência da aresta entre os pontos
        int line = -1, column = -1;
        for(int i = 0; i < vertex.size(); i++){
            if(vertex[i].getName() == descA){
                line = i;
            }else if(vertex[i].getName() == descB){
                column= i;
            }
        }
        if(line >=0 && column >=0 ){
            TipoValorVertice id = column;
            vertex[line].edge[column].setEdge();
            vertex[line].edge[column].setLength(length);
            vertex[line].edge[column].setAgility(time);
            vertex[line].edge[column].setId(id);

            if(Twoway){                                             //Para habilitar o vértice não direcional
                vertex[column].edge[line].setEdge();
                vertex[column].edge[line].setLength(length);
                vertex[column].edge[line].setAgility(time);
                vertex[column].edge[line].setId(id);
            }

        }else{                                                      //Caso o endereço não exista
            cerr << "(Out of bounds) Insertion failure" << endl;

        }

    }

    void removeEdge(string descA, string descB, int Twoway){              //Remove uma ligação entre dois pontos

        int line = -1, column = -1;

        for(int i = 0; i < vertex.size(); i++){
            if(vertex[i].getName() == descA){
                line = i;
            }else if(vertex[i].getName() == descB){
                column= i;
            }

        }
        if(line>=0 && column>=0){
            if(vertex[line].edge[column].edge()){
                vertex[line].edge[column].removeEdge();
                vertex[line].edge[column].setDefault();
                if(Twoway){                                         //Para remover vértice não direcional
                    vertex[column].edge[line].removeEdge();
                    vertex[column].edge[line].setDefault();
                }
            }else{
                cerr << "This edge does't exist!" << endl;
            }
        }else{
             cerr << "(Out of bounds) Remotion failure" << endl;
        }
    }

};


class XGraphTools{                                                  //Ferramentas para manipular o grafo

private://Implementação futura
    vector<int> Breadth_Tree;                                       // Árvore de largura, pi;
    vector<int> distance;                                           // Vetor distancia
    XGraph min_tree;
    Vertex temp_vertex;

private:
    
void FGVazio(TipoGrafo *Grafo)
{ int  i, j;
  for (i = 0; i <= Grafo->NumVertices; i++) 
    for (j = 0; j <= Grafo->NumVertices; j++)
      Grafo->Mat[i][j] = 0;
}

void InsereAresta(TipoValorVertice *V1, TipoValorVertice *V2, TipoPeso *Peso, 
          TipoGrafo *Grafo)

{ Grafo->Mat[*V1][*V2] = *Peso;
}

short  ExisteAresta(TipoValorVertice Vertice1, TipoValorVertice Vertice2, 
            TipoGrafo *Grafo)
{ return (Grafo->Mat[Vertice1][Vertice2] > 0);
}

/*-- Operadores para obter a lista de adjacentes --*/

short ListaAdjVazia(TipoValorVertice *Vertice, TipoGrafo *Grafo)
{ TipoApontador Aux = 0;
  short ListaVazia = TRUE;
  while (Aux < Grafo->NumVertices && ListaVazia) 
    { if (Grafo->Mat[*Vertice][Aux] > 0)
      ListaVazia = FALSE;
      else
      Aux++;
    }
  return (ListaVazia == TRUE);
} 

TipoApontador PrimeiroListaAdj(TipoValorVertice *Vertice, TipoGrafo *Grafo)
{ TipoValorVertice Result;
  TipoApontador Aux = 0;
  short Listavazia = TRUE;
  while (Aux < Grafo->NumVertices && Listavazia) 
    { if (Grafo->Mat[*Vertice][Aux] > 0) 
      { Result = Aux; Listavazia = FALSE;
      } 
      else  Aux++;
    }
  if (Aux == Grafo->NumVertices)
  printf("Erro: Lista adjacencia vazia (PrimeiroListaAdj)\n");
  return Result;
}

void ProxAdj(TipoValorVertice *Vertice, TipoGrafo *Grafo, 
         TipoValorVertice *Adj, TipoPeso *Peso, TipoApontador *Prox, 
         short *FimListaAdj)
{ /* --Retorna Adj apontado por Prox--*/
  *Adj = *Prox;
  *Peso = Grafo->Mat[*Vertice][*Prox];
  (*Prox)++;
  while (*Prox < Grafo->NumVertices && Grafo->Mat[*Vertice][*Prox] == 0)
    (*Prox)++;
  if (*Prox == Grafo->NumVertices)  *FimListaAdj = TRUE;
} 

void ImprimeGrafo(TipoGrafo *Grafo)

{ int i, j;
  printf("   ");
  for (i = 0; i < Grafo->NumVertices; i++) printf("%3d", i); putchar('\n');
  for (i = 0; i < Grafo->NumVertices; i++) 
    { printf("%3d", i);
      for (j = 0; j < Grafo->NumVertices; j++)
        printf("%3d", Grafo->Mat[i][j]);
      putchar('\n');
    }
}  

void RefazInd(TipoIndice Esq, TipoIndice Dir, TipoItem *A, TipoPeso *P, 
            TipoValorVertice *Pos)

{ TipoIndice i = Esq;  int j;  TipoItem x;
  j = i * 2;  x = A[i];
  while (j <= Dir) 
    { if (j < Dir) 
      { if (P[A[j].Chave] > P[A[j+1].Chave]) j++; 
      }
      if (P[x.Chave] <= P[A[j].Chave])  goto L999;
      A[i] = A[j]; Pos[A[j].Chave] = i;
      i = j;   j = i * 2;
    }
  L999:  A[i] = x;  Pos[x.Chave] = i;
} 

void Constroi(TipoItem *A,  TipoPeso *P,  TipoValorVertice *Pos)

{ TipoIndice Esq;
  Esq = n / 2 + 1;
  while (Esq > 1) 
    { Esq--;
      RefazInd(Esq, n, A, P, Pos);
    }
} 

TipoItem RetiraMinInd(TipoItem *A,  TipoPeso *P,  TipoValorVertice *Pos)
{ TipoItem Result;
  if (n < 1) 
  { printf("Erro: heap vazio\n");
    return Result;
  }
  Result = A[1];
  A[1] = A[n]; Pos[A[n].Chave] = 1;
  n--;
  RefazInd(1, n, A, P, Pos);
  return Result;
}  

int DiminuiChaveInd(TipoIndice i, TipoPeso ChaveNova, TipoItem *A, 
             TipoPeso *P,  TipoValorVertice *Pos)
{ TipoItem x;
  if (ChaveNova > P[A[i].Chave]) 
  { //printf("Erro: ChaveNova maior que a chave atual\n");
     printf("Caminho indisponivel\n");
    exit(0);
  }
  P[A[i].Chave] = ChaveNova;
  while (i > 1 && P[A[i / 2].Chave] > P[A[i].Chave]) 
    { x = A[i / 2];
      A[i / 2] = A[i];
      Pos[A[i].Chave] = i / 2;
      A[i] = x;
      Pos[x.Chave] = i;
      i /= 2;
    }
    return 1;
} 

void Dijkstra(TipoGrafo *Grafo, TipoValorVertice *Raiz, int destino, int origem, XGraph table)
{ TipoPeso P[MAXNUMVERTICES + 1];
  TipoValorVertice Pos[MAXNUMVERTICES + 1];
  long Antecessor[MAXNUMVERTICES + 1];
  //short Itensheap[MAXNUMVERTICES + 1];
  TipoVetor A;
  TipoValorVertice u, v;
  TipoItem temp;
  for (u = 0; u <= Grafo->NumVertices; u++)
  { /*Constroi o heap com todos os valores igual a INFINITO*/
    Antecessor[u] = -1; P[u] = INFINITO;
    A[u+1].Chave = u;   /*Heap a ser construido*/
    /*Itensheap[u] = TRUE;*/  Pos[u] = u + 1;
  }
  n = Grafo->NumVertices;
  P[*(Raiz)] = 0;
  Constroi(A, P, Pos);
  while (n >= 1) 
  { /*enquanto heap nao vazio*/
    temp = RetiraMinInd(A, P, Pos);
    u = temp.Chave; /*Itensheap[u] = FALSE;*/
    if (!ListaAdjVazia(&u, Grafo))
    { Aux = PrimeiroListaAdj(&u, Grafo); FimListaAdj = FALSE;
      while (!FimListaAdj) 
      { ProxAdj(&u, Grafo, &v, &Peso, &Aux, &FimListaAdj);
        if (P[v] > (P[u] + Peso))
        { P[v] = P[u] + Peso; Antecessor[v] = u;
          DiminuiChaveInd(Pos[v], P[v], A, P, Pos);
          //printf("Caminho: v[%d] v[%ld] d[%d]", 
	         //v, Antecessor[v], P[v]);
         // scanf("%*[^\n]");
          //getchar();
        }
      }
    } 
  }
  int aux,aux2 = destino; int vetor[LSIZE];
  vetor[0] = destino; int m;
  for(m=1;Antecessor[aux2]!=-1;m++){
  aux = Antecessor[aux2];
  //printf("%d ",city_list[aux].cod);
    vetor[m] = aux;
    //printf("%d %d %d %s\n",vetor[m],aux,m,city_list[aux].cidade);
  aux2 = aux;
  }
  //printf("\n");
  vetor[m]= origem;
  for(int i=m-1;i>=0;i--){
    cout << " " << table.vertex[vetor[i]].getName();
  }
  printf("\nTotal: %d\n",P[destino]);
}


public:

    int short_way(XGraph table, string v_origem, string v_destino, int option){
        Grafo.NumArestas = table.vertex.size();
        Grafo.NumVertices = table.n_edge;
        FGVazio(&Grafo);
        TipoPeso *length;
        for(int line = 0; line < table.vertex.size(); line++){
            for(int column = 0; column < table.vertex[line].edge.size(); column++){
                length = table.vertex[line].edge[column].getLength();
                if(*length != 0){
                    //cout << "id[line] = " << *table.vertex[line].getId() << endl;
                    //cout << "id[column] = " << *table.vertex[line].edge[column].getId() << endl;
                    //cout << "Length = " << *length << endl;
                    if(option)
                    InsereAresta(table.vertex[line].getId(), table.vertex[line].edge[column].getId(),table.vertex[line].edge[column].getLength(),&Grafo);
                    else
                        InsereAresta(table.vertex[line].getId(), table.vertex[line].edge[column].getId(),table.vertex[line].edge[column].getAgility(),&Grafo);
                }
            }
        }
        
        int origem = -1, destino = -1;
        for(int i = 0; i < table.vertex.size(); i++){
            if(table.vertex[i].getName() == v_origem){
                origem = i;
            }else if(table.vertex[i].getName() == v_destino){
                destino= i;
            }
        }
        if(origem < 0 || destino <0){
            cerr << "Origem ou destino inválido!";
            return 0;
        }
        TipoValorVertice root = origem;
        Dijkstra(&Grafo,&root, destino,origem, table);

        return 1;
    }


    int loadFile(char file_name[], XGraph & table){ //Le um arquivo com os dados do grafo
    
    FILE *file;
    file = fopen(file_name, "r");
    if(file==NULL){
        cerr << "Can't open file " << file_name << endl;
        return 0;
    }
    int n_vertex,n_edge, result;
    char c_vertex;
    Vertex temp_vertex;
    result = fscanf(file, "%i\n", &n_vertex);
    //cout << n_vertex << endl;
    for(int i = 0; i < n_vertex; i++){
        result = fscanf(file, "%c\n", &c_vertex);
        std::string s(1, c_vertex);
        //cout << s << endl;
        temp_vertex.name = s;
        table.insertVertex(temp_vertex);

    }
    result = fscanf(file, "%i\n", &n_edge);
    table.n_edge = n_edge;
    //cout << n_edge << endl;
    char a,b;
    int value, time;
    for(int i = 0; i <= n_edge; i++){
        result = fscanf(file, "%c %c %d %d\n", &a,&b,&value, &time);
        std::string a1(1, a);
        std::string b1(1, b);
        //cout << a1 << " " << b1 << " " << value << endl;
        table.insertEdge(a1,b1,0,value,time);

    }

        return 1;

    }


};


int main(){

XGraph table;
XGraphTools tools;
Vertex line;
int option, dir = 0,length, time;
string l,c;
string name;
string origem, destino;
char file_name[100];
while(true){
    printf("-------------------------------------------\n");
    cout<< "[1]Inserir Vértice\t[2]Remover Aresta\n[3]Inserir aresta\t[4]Carregar arquivo\n[5]Ver Grafo\t\t[6]Menor caminho\n[10]Sair" << endl;
    cin >> option;
    
    switch (option)
    {
    case 1:
        cout<<"Descrição: ";
        cin >> line.name;
        table.insertVertex(line);
        break;
    case 2: 
        cout << "Descrição Linha: ";cin >> l;
        cout << "Descrição Coluna:"; cin >> c;
        table.removeEdge(l,c,dir);
        break;
    case 3: 
        if(dir == -1){
            cout << "Bidirecional? [0]Não [1]Sim\n: ";
            cin >> dir;
        }
        cout << "Descrição linha: ";cin >> l;
        cout << "Descrição Coluna:"; cin >> c;
        cout << "Comprimento: "; cin >> length;
        cout << "Tempo: "; cin >> time ;
        table.insertEdge(l,c,dir,length, time);
        break;
    case 4:
        cout << "File name: ";
        scanf("%s",file_name);
        tools.loadFile(file_name,table);
        break;
    case 5:
        cout << "Grafo de Distancias: " << endl;
        table.printXGrafo(table.vertex);
        cout << "Grafo de Tempo: " << endl;
        table.printXGrafo_Agility(table.vertex);
        break;
    case 6:
        cout << "Origem: "; cin >> origem;
        cout << "Destino: "; cin >> destino;
        printf("Menor Distancia\nPercurso: ");
        tools.short_way(table,origem,destino,1);
        printf("Menor Tempo\nPercurso: ");
        tools.short_way(table,origem,destino,0);

        break;
    case 10: 
        exit(0);
    default:
        break;
    }


}


}