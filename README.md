# NonogramProblem
Repositório para o desenvolvimento de algoritmos para resolver nonogramas. Os algoritmos foram desenvolvidos em linguagem C++

## Como realizar um experimento

### Inputs
A fim de tentar aplicar um algoritmo a um nonograma, primeiramente é necessário
transformá-lo em um arquivo de entrada. Um nonograma é reconhecido e lido pelo
algoritmo como um arquivo de texto, no qual a primeira linha consiste de dois
inteiros (número de linhas N e número de colunas M). As próximas N linhas
consistem das restrições das linhas do nonograma, de cima para baixo. Depois,
as próximas M linhas consistem das restrições das colunas do nonograma, da
esquerda para a direita. Como exemplo, o arquivo presente em
[inputs/example.txt](https://github.com/RickFqt/NonogramProblem/blob/main/inputs/in1.txt)
é utilizado para representar o seguinte nonograma:

<img src="./docs/imgs/example.png" alt="example_img" width=200>

### Compilando e executando

Tendo acesso ao algoritmo, será necessário compilá-lo usando o g++. Para compilar o código heurístico, utilize o seguinte comando:

```
$ g++ heuristic.cpp -o solve.out
```

Após gerar o executável, para aplicar o algoritmo a algum caso de teste, será necessário informar o caminho para esse caso de teste como input. O seguinte comando executa o algoritmo heurístico no nonograma representado por [inputs/example.txt](https://github.com/RickFqt/NonogramProblem/blob/main/inputs/in1.txt). A solução do nonograma encontrada pelo algoritmo será salva no arquivo [out.txt](https://github.com/RickFqt/NonogramProblem/blob/main/out.txt).


```
$ ./solve.out <inputs/example.txt >out.txt
```
Para testar o algoritmo em outro nonograma, basta substituir "inputs/example.txt" para o caminho do arquivo de entrada desejado. 
Ademais, caso queira comparar a solução encontrada com a solução real, as soluções de alguns dos arquivos de entrada estão disponíveis na pasta [solutions](https://github.com/RickFqt/NonogramProblem/blob/main/solutions).

## Medição de tempo de execução

A fim de realizar o teste de tempo, pode-se utilizar os seguintes comandos:

```
$ g++ outputTime.cpp -o time.out

$ ./time.out
```
O resultado dos testes será salvo no arquivo [outputTime.txt](https://github.com/RickFqt/NonogramProblem/blob/main/outputTime.txt).