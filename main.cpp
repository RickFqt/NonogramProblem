#include <bits/stdc++.h>

std::vector<int> tokenize(std::string input){

    std::istringstream iss(input);
    std::vector<int> tokens;

    std::string token;
    while (std::getline(iss, token, ' ')) {
        tokens.push_back(stoi(token));
    }
    return tokens;
}

int funcaodoida(std::vector<std::vector<int>>linhas,
                std::vector<std::vector<int>>colunas,
                std::vector<std::vector<int>>nonograma,
                int linhas_preenchidas){
    
    
}


int main(){

    // Ler um arquivo aí
    int n_linhas, n_colunas;
    std::cin >> n_linhas >> n_colunas;

    std::string entrada;
    std::getline(std::cin, entrada);

    std::vector<std::vector<int>> linhas;
    std::vector<std::vector<int>> colunas;

    for(int i{0}; i < n_linhas; ++i){
        std::getline(std::cin, entrada);
        linhas.push_back(tokenize(entrada));
    }

    for(int i{0}; i < n_colunas; ++i){
        std::getline(std::cin, entrada);
        colunas.push_back(tokenize(entrada));
    }

    // Matriz do nonograma
    // Cada célula contém um inteiro (-1, 0 ou 1)
    // -1 --> Célula marcada com X
    // 0 --> Célula ainda não marcada
    // 1 --> Célula colorida
    std::vector<std::vector<int>> nonograma(n_linhas, std::vector<int>(n_colunas));

    // Algoritmo Heurístico :D

    // Enquanto não estiver completo:
    //      Escolher a proxima linha
    //      Preencher a linha corretamente de forma a minimizar conflitos

    // Enquanto não estiver completo:
    for(int i{0}; i < n_linhas; ++i){
        //Preencher a linha
            // Simula 10 vezes?
            // Pega a melior
    }

    return 0;
}