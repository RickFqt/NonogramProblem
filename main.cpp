#include <bits/stdc++.h>

std::vector<int> fatorial_vector({1});

std::vector<int> tokenize(std::string input){

    std::istringstream iss(input);
    std::vector<int> tokens;

    std::string token;
    while (std::getline(iss, token, ' ')) {
        tokens.push_back(stoi(token));
    }
    return tokens;
}

int fatorial(int n){
    if(fatorial_vector.size() > n){
        return fatorial_vector[n];
    }

    fatorial_vector.push_back(n * fatorial(n-1));

    return fatorial_vector[n];
}

int combinacao(int n, int p){
    return (fatorial(n))/(fatorial(p)*fatorial(n-p));
}

std::vector<bool> convert_number(int n_sorteado, int quadrados_vazios, int blocos_preenchidos, 
                                 int largura, std::vector<int> blocos){
    
    std::vector<bool> pos_blocos(quadrados_vazios + 1);
    std::vector<bool> retorno(largura);

    // Converte o numero sorteado para o formato de distribuicao de blocos associado
    for(int i{0}; i <= quadrados_vazios; ++i){

        if(blocos_preenchidos == 0){
            pos_blocos[i] = false;
        }
        else if(combinacao(quadrados_vazios - i, blocos_preenchidos - 1) < n_sorteado){
            n_sorteado -= combinacao(quadrados_vazios - i, blocos_preenchidos - 1);
            pos_blocos[i] = false;

        }
        else{
            --blocos_preenchidos;
            pos_blocos[i] = true;
        }

    }


    int index_bloco = 0;
    int index_retorno = 0;

    // Converte a linha resultante para uma linha própria do nonograma
    for(int i{0}; i < pos_blocos.size(); ++i){
        if(pos_blocos[i]){
            for(int j{0}; j < blocos[index_bloco]; ++j){
                retorno[index_retorno] = 1;
                ++index_retorno;
            }
            ++index_bloco;
            
        }
    
        ++index_retorno;
    }


    return retorno;
    

}

int soma_vetor(std::vector<int> vetor){
    int total = 0;
    for(int i{0}; i < vetor.size(); ++i){
        total += vetor[i];
    }
    return total;
}


int funcaodoida(std::vector<std::vector<int>>linhas,
                std::vector<std::vector<int>>colunas,
                std::vector<std::vector<int>>nonograma,
                std::vector<bool>linha_analisada, 
                int idx_linha_analisada,
                int linhas_preenchidas){
    
    int sum1 = 0;
    int sum11, sum12;

    for(int i{0}; i < colunas.size(); ++i){
        sum11 = 0; sum12 = 0;
        sum11 += soma_vetor(colunas[i]);

        for(int j{0}; j <= linhas_preenchidas; ++j){
            if(j == idx_linha_analisada){
                if(linha_analisada[i]){
                    sum12 += 1;
                }
            }
            else if(nonograma[j][i] != -1){
                sum12 += nonograma[j][i];
            }
        }

        sum1 += abs(sum11 - sum12);
    }

    // int sum2 = 0;
    // int sum21;

    // for(int i{0}; i < colunas.size(); ++i){

    // }

    return sum1;
}

std::vector<bool> escolher_linha(std::vector<std::vector<bool>>linhas_construidas, std::vector<std::vector<int>>linhas,
                                 std::vector<std::vector<int>>colunas, std::vector<std::vector<int>>nonograma,
                                 int idx_linha_analisada, int linhas_preenchidas){
    int min = INT_MAX;
    int i_min = 0;
    int current;

    for(int i{0}; i < linhas_construidas.size(); ++i){
        current = funcaodoida(linhas, colunas, nonograma, linhas_construidas[i], idx_linha_analisada, linhas_preenchidas);
        if (current < min){
            i_min = i;
            min = current;
        }
    }

    return linhas_construidas[i_min];

}

int main(){

    // Seed aleatória
    srand(time(nullptr));

    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    // Ler um arquivo aí
    int n_linhas, n_colunas;
    std::cin >> n_linhas >> n_colunas;


    std::vector<std::vector<int>> linhas; // Guarda o valor dos blocos de cada linha
    std::vector<int> soma_linhas(n_linhas); // Guarda a soma do tamanho dos blocos de cada linha
    std::vector<std::vector<int>> colunas; // Guarda o valor dos blocos de cada coluna
    std::vector<int> soma_colunas(n_colunas); // Guarda a soma do tamanho dos blocos de cada coluna

    std::string entrada;
    std::getline(std::cin, entrada);

    // Lê as linhas
    for(int i{0}; i < n_linhas; ++i){
        std::getline(std::cin, entrada);
        linhas.push_back(tokenize(entrada));

        // Calcula a soma de linhas
        soma_linhas[i] = soma_vetor(linhas[i]);
    }

    // Lê as colunas
    for(int i{0}; i < n_colunas; ++i){
        std::getline(std::cin, entrada);
        colunas.push_back(tokenize(entrada));

        // Calcula a soma de colunas
        soma_colunas[i] = soma_vetor(colunas[i]);
    }


    // Matriz do nonograma
    // Cada célula contém um inteiro (-1, 0 ou 1)
    // -1 --> Célula ainda não marcada
    // 0 --> Célula marcada com X
    // 1 --> Célula colorida
    std::vector<std::vector<int>> nonograma(n_linhas, std::vector<int>(n_colunas, -1));


    // Algoritmo Heurístico :D

    // Enquanto não estiver completo:
    //      Escolher a proxima linha
    //      Fazer 10 construcoes para a linha corretamente sem se preocupar com as colunas
    //      Escolher a linha construída que menos causa conflito com as colunas

    // Enquanto não estiver completo:
    for(int i{0}; i < n_linhas; ++i){
        //Preencher a linha

        
        int quadrados_vazios = n_colunas - soma_linhas[i]; // Número de quadrados vazios necessários nessa linha
        int combinao = combinacao(quadrados_vazios + 1, linhas[i].size()); // Número máximo de possíveis alocações dos blocos
        int tamanho = std::min(combinao, 100);
        std::vector<std::vector<bool>> linhas_construidas(tamanho, std::vector<bool>(n_colunas));

        if(soma_linhas[i] != 0){
            
            std::vector<int> numeros_sorteados;

            for(int j{1}; j <= combinao; ++j){
                numeros_sorteados.push_back(j);
            }
            std::random_shuffle(numeros_sorteados.begin(), numeros_sorteados.end());

            for(int j{0}; j < tamanho; ++j){

                int sorteado = numeros_sorteados[j];
                // Converte o número sorteado à respectiva alocação de blocos
                linhas_construidas[j] = convert_number(sorteado, quadrados_vazios, linhas[i].size(), n_colunas, linhas[i]);
            }


        }

        std::vector<bool> linha_escolhida;
        if(soma_linhas[i] == 0){
            linha_escolhida = linhas_construidas[0];
        }
        else{

            linha_escolhida = escolher_linha(linhas_construidas, linhas, colunas, nonograma, i, i);

        }
        // Escolhe a linha com menor conflito



        // Preenche o nonograma com a linha escolhida
        for(int j{0}; j < n_colunas; ++j){
            if(linha_escolhida[j]){

                nonograma[i][j] = 1;
            }
            else{
                nonograma[i][j] = 0;
            }
        }

    }

    // Print do nonograma resultante
    for(int i{0}; i < n_linhas; ++i){
        for(int j{0}; j < n_colunas; ++j){
            if(nonograma[i][j] == 1){
                std::cout << "O";
            }
            else{
                std::cout << " "; 
            }
        }
        std::cout << std::endl;
    }



    return 0;
}