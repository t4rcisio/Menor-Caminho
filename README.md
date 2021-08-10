# Menor-Caminho - Dijkstra
Algoritmo que encontra o menor caminho entre dois pontos usando o algoritmo de Dijkstra

Formato de armazenamento de dados:

Matriz de adjacência

    |-------------------|___|___|___|___|
    |   |---------------|___|___|___|___|
    |   |   |-----------|___|___|___|___|
    |   |   |   |-------|___|___|___|___|
    A   B   C   D   ... .
                        .
                        . 
   


Formato do arquivo de entrada:
Esse formato de entrada permite costruir dois grafos:
-Grafo de distância
-Grafo de tempo

5 <----- Quantidade de vértices
a
b
c
d
e
10 <---- Quantidade de arestas
a b 3 3 <----- [vértice_A] [vértice_B] [distância] [tempo]
a d 5 5
b c 2 6
b d 2 2
c e 2 2
d b 3 1
d c 5 4
d e 9 6
e a 6 3
e c 4 7


<h1>Menu</h1>
-------------------------------------------
[1]Inserir Vértice      [2]Remover Aresta
[3]Inserir aresta       [4]Carregar arquivo
[5]Ver Grafo            [6]Menor caminho
[10]Sair
