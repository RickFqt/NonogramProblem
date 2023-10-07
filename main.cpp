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


    for(int i{0}; i <= quadrados_vazios; ++i){
        // std::cout << "quadrados_vazios - i = " << quadrados_vazios - i << std::endl;
        // std::cout << "blocos_preenchidos - 1 = " << blocos_preenchidos - 1 << std::endl << std::endl;

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

        // if(pos_blocos[i]){
        //     std::cout << "1 ";
        // }
        // else{
        //     std::cout << "0 ";
        // }
    }

    // std::cout << "AAAA" << std::endl;

    int index_bloco = 0;
    int index_retorno = 0;
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

    // std::cout << std::endl;

    // for(int i{0}; i < retorno.size(); ++i){
    //     if(retorno[i]){
    //         std::cout << "1 " ;
    //     }
    //     else{
    //         std::cout << "0 " ;
    //     }
    // }
    
    // std::cout << std::endl;

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
                int linhas_preenchidas){
    
    int sum1 = 0;
    int sum11, sum12;

    for(int i{0}; i < colunas.size(); ++i){
        sum11 = 0; sum12 = 0;
        for(int j{0}; j < colunas[i].size(); ++j){
            sum11 += colunas[i][j];
        }

        for(int j{0}; j < linhas_preenchidas; ++j){

        }
    }

    int sum2 = 0;
    int sum21;
}


int main(){

    // Seed aleatória
    srand(time(nullptr));

    // Ler um arquivo aí
    int n_linhas, n_colunas;
    std::cin >> n_linhas >> n_colunas;

    std::string entrada;
    std::getline(std::cin, entrada);

    std::vector<std::vector<int>> linhas;
    std::vector<int> soma_linhas(n_linhas);
    std::vector<std::vector<int>> colunas;
    std::vector<int> soma_colunas(n_colunas);

    for(int i{0}; i < n_linhas; ++i){
        std::getline(std::cin, entrada);
        linhas.push_back(tokenize(entrada));

        // Calcula a soma de linhas
        soma_linhas[i] = soma_vetor(linhas[i]);
    }

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

    // for(int i{0}; i < n_linhas; ++i){
    //     for(int j{0}; j < n_colunas; ++j){
    //         // if(nonograma[i][j] == 1){
    //         //     std::cout << "O";
    //         // }
    //         // else{
    //         //     std::cout << " "; 
    //         // }
    //         std::cout << nonograma[i][j] << " "; 
    //     }
    //     std::cout << std::endl;
    // }


    // Algoritmo Heurístico :D

    // Enquanto não estiver completo:
    //      Escolher a proxima linha
    //      Preencher a linha corretamente de forma a minimizar conflitos

    // Enquanto não estiver completo:
    for(int i{0}; i < n_linhas; ++i){
        //Preencher a linha
            // BlocosVazios = n_colunas - TotalBlocosPreenchidos

            std::vector<bool> linha(n_colunas);
            if(soma_linhas[i] != 0){

                int quadrados_vazios = n_colunas - soma_linhas[i];
                // std::cout << quadrados_vazios << " " << linhas[i].size() << std::endl;
                int combinao = combinacao(quadrados_vazios + 1, linhas[i].size());

                int sorteado = rand() % combinao + 1;

                // FUNCAO BLAU UAU
                linha = convert_number(sorteado, quadrados_vazios, linhas[i].size(), n_colunas, linhas[i]);
                // for(int j{0}; j < n_colunas; ++j){
                //     if(linha[j]){

                //         std::cout << 1 << " ";
                //     }
                //     else{
                //         std::cout << 0 << " ";
                //     }
                // }
                // std::cout << std::endl;
            }



            for(int j{0}; j < n_colunas; ++j){
                if(linha[j]){

                    nonograma[i][j] = 1;
                }
                else{
                    nonograma[i][j] = 0;
                }
            }
            // std::cout << "asdawsdaw" << std::endl;




            // Simula 10 vezes?
            // Pega a melior
    }
    // std::cout << "asdawsdaw2" << std::endl;

    for(int i{0}; i < n_linhas; ++i){
        for(int j{0}; j < n_colunas; ++j){
            if(nonograma[i][j] == 1){
                std::cout << "O";
            }
            else{
                std::cout << " "; 
            }
            // std::cout << nonograma[i][j] << " "; 
        }
        std::cout << std::endl;
    }



    return 0;
}