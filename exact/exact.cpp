#include <bits/stdc++.h>


std::vector<std::vector<int>> triangulo_pascal({{1}});

// ---------------------- Metodos auxiliares ----------------

std::vector<int> tokenize(std::string input){

    std::istringstream iss(input);
    std::vector<int> tokens;

    std::string token;
    while (std::getline(iss, token, ' ')) {
        tokens.push_back(stoi(token));
    }
    return tokens;
}

int soma_vetor(std::vector<int> vetor){
    int total = 0;
    for(int i{0}; i < vetor.size(); ++i){
        total += vetor[i];
    }
    return total;
}

int combinacao(int n, int p){
    if(triangulo_pascal.size() > n){

        if(triangulo_pascal[n].size() > p){
            return triangulo_pascal[n][p];
        }

        return -1;
    }
    else{
        
        int idx_inicial = triangulo_pascal.size();
        while(triangulo_pascal.size() <= n){
            triangulo_pascal.push_back({1});
        }
        for(int i{idx_inicial}; i <= n; ++i){

            for(int j{1}; j <= i; ++j){
                if(j == i){
                    triangulo_pascal[i].push_back(1);
                }
                else{
                    triangulo_pascal[i].push_back(combinacao(i - 1, j - 1) + combinacao(i - 1, j));
                }
            }
        }

        return combinacao(n, p);
    }
}

// ---------------------- Metodos da heuristica ----------------

std::vector<std::vector<int>> calcula_blocos(std::vector<std::vector<int>>nonograma){

    int n_linhas = nonograma.size();
    int n_colunas = nonograma[0].size();
    std::vector<std::vector<int>> quantidade_blocos(n_colunas);
    std::vector<int> continuidade_blocos(n_colunas);
    std::vector<int> bloco_atual(n_colunas);

    for(int i{0}; i < n_linhas; ++i){

        for(int j{0}; j < n_colunas; ++j){

            if(nonograma[i][j] == 0 || nonograma[i][j] == -1){

                if(continuidade_blocos[j] > 0){
                    
                    continuidade_blocos[j] = 0;
                    bloco_atual[j]++;
                }
            }
            else if(nonograma[i][j]  == 1){

                continuidade_blocos[j]++;
                if(continuidade_blocos[j] == 1){
                    // Atualiza qual é o bloco atual
                    // Atualiza o tamanho do bloco atual
                    quantidade_blocos[j].push_back(1);
                }
                else{
                    quantidade_blocos[j][bloco_atual[j]]++;
                }
            }
        }
    }

    return quantidade_blocos;

}

// Funcao objetivo calculada quando o nonograma está completo
int funcao_objetivo(std::vector<std::vector<int>>linhas,
                std::vector<std::vector<int>>colunas,
                std::vector<std::vector<int>>nonograma){
    
    int sum1 = 0;
    int sum11, sum12;

    for(int i{0}; i < colunas.size(); ++i){
        sum11 = 0; sum12 = 0;
        sum11 += soma_vetor(colunas[i]);

        for(int j{0}; j < linhas.size(); ++j){
            if(nonograma[j][i] != -1){
                sum12 += nonograma[j][i];
            }
        }

        sum1 += abs(sum11 - sum12);
    }

    // Segunda parcela do somatório
    int sum2 = 0, c_pk, b_pk;
    std::vector<std::vector<int>> blocos_por_coluna = calcula_blocos(nonograma);

    for(int k{0}; k < colunas.size(); ++k){
        for(int p{0}; p < std::max(colunas[k].size(), blocos_por_coluna[k].size()); ++p){
            if(p >= colunas[k].size()){
                c_pk = 0;
            }
            else{
                c_pk = colunas[k][p];
            }

            if(p >= blocos_por_coluna[k].size()){
                b_pk = 0;
            }
            else{
                b_pk = blocos_por_coluna[k][p];
            }
            sum2 += abs(c_pk - b_pk);
        }
    }
    return sum1 + sum2;
}

void pre_processamento(std::vector<std::vector<int>>colunas,
                       std::vector<std::vector<int>>& nonograma, 
                       std::vector<bool>& colunas_completas,
                       bool isLinha){
    
    int n_colunas = colunas.size();
    int n_linhas;
    if(isLinha){
        n_linhas = nonograma[0].size();
    }
    else{
        n_linhas = nonograma.size();
    }
    int current_idx;


    for(int i{0}; i < n_colunas; ++i){
        std::vector<std::pair<int, int>>inicio_fim;
        std::vector<std::pair<int, int>>fim_inicio;

        // Encontra as posições dos blocos mais acima na coluna
        current_idx = 0;
        for(int j{0}; j < colunas[i].size(); ++j){
            inicio_fim.push_back({current_idx, current_idx + colunas[i][j]-1});
            current_idx += colunas[i][j] + 1;
        }

        // Encontra as posições dos blocos mais abaixo na coluna
        current_idx = n_linhas - 1;
        for(int j{(int)colunas[i].size() - 1}; j >= 0; --j){
            fim_inicio.push_back({current_idx - colunas[i][j] + 1, current_idx});
            current_idx -= (colunas[i][j] + 1);
        }
        std::reverse(fim_inicio.begin(), fim_inicio.end());

        // Caso a coluna não tenha blocos, preenche tudo com X
        if(colunas[i][0] == 0){
            for(int j{0}; j < n_linhas; ++j){
                if(isLinha){
                    nonograma[i][j] = 0;
                }
                else{
                    nonograma[j][i] = 0;
                }
            }
            colunas_completas[i] = true;
        }

        // Preenche todos os quadrados que tenham "overlap"
        for(int j{0}; j < colunas[i].size(); ++j){
            if(inicio_fim[j].second - fim_inicio[j].first >= 0){

                for(int k{fim_inicio[j].first}; k <= inicio_fim[j].second; ++k){

                    if(isLinha){
                        nonograma[i][k] = 1;
                    }
                    else{
                        nonograma[k][i] = 1;
                    }
                }
            }
        }

        if(inicio_fim == fim_inicio){
            colunas_completas[i] = true;
        }
    }

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

bool linha_valida(const std::vector<bool>& linha, const std::vector<int>& linha_nonograma){
    bool retorno = true;

    for(int i{0}; i < linha.size(); ++i){

        if(linha[i] && linha_nonograma[i] == 0){
            return false;
        }

        if(!linha[i] && linha_nonograma[i] == 1){
            return false;
        }
    }

    return retorno;
}

std::vector<std::pair<int, int>> mais_a_cima
(std::vector<int>coluna, std::vector<int>coluna_nonograma,
 int soma_coluna, int n_colunas, int n_linhas, bool maisabaixo = false){


    int quadrados_vazios = n_linhas - soma_coluna; // Número de quadrados vazios necessários nessa linha
    int combinao = combinacao(quadrados_vazios + 1, coluna.size()); // Número máximo de possíveis alocações dos blocos

    std::vector<int> numeros_sorteados;
    std::vector<bool> vetor_sorteado;
    std::vector<std::pair<int, int>>inicio_fim;

    for(int i=1; i <= combinao; ++i){
        numeros_sorteados.push_back(i);
    }

    if(maisabaixo){
        std::reverse(numeros_sorteados.begin(), numeros_sorteados.end());
    }
    for(int j{1}; j <= combinao; ++j){
        // Converte o número sorteado à respectiva alocação de blocos
        vetor_sorteado = convert_number(j, quadrados_vazios, coluna.size(), n_linhas, coluna);
        if(linha_valida(vetor_sorteado, coluna_nonograma)){

            std::cout << "Vetor Que deu Certo: ";
            for(int p = 0; p < vetor_sorteado.size(); ++p){
                std::cout << vetor_sorteado[p] << " \n"[p == vetor_sorteado.size() - 1];
            }
            int current_bloco = 0;
            for(int i{0}; i < vetor_sorteado.size(); ++i){

                if(vetor_sorteado[i]){

                    
                    inicio_fim.push_back({i, i + coluna[current_bloco] - 1});
                    i += coluna[current_bloco] - 1;
                    current_bloco++;
                }
            }

            return inicio_fim;
        }
    }


    return {{-1, -1}};


    

}

std::vector<std::pair<int, int>> mais_a_baixo
(std::vector<int>coluna, std::vector<int>coluna_nonograma,
int soma_coluna, int n_colunas, int n_linhas){

    return mais_a_cima(coluna, coluna_nonograma, soma_coluna, n_colunas, n_linhas, true);
}

std::vector<std::pair<int, int>> mais_a_esquerda
(std::vector<int>linha, std::vector<int>linha_nonograma,
 int soma_linha, int n_linhas, int n_colunas){

    return mais_a_cima(linha, linha_nonograma, soma_linha, n_linhas, n_colunas);
}

std::vector<std::pair<int, int>> mais_a_direita
( std::vector<int>linha, std::vector<int>linha_nonograma,
  int soma_linha, int n_linhas, int n_colunas){

    return mais_a_cima(linha, linha_nonograma, soma_linha, n_linhas, n_colunas, true);
}

int main(){
    std::srand ( unsigned ( std::time(0) ) );

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

    std::vector<bool> linhas_completas(n_linhas);
    std::vector<bool> colunas_completas(n_colunas);

    // Pré-processamento: percorre as colunas e linhas, e marca os quadrados que são certos de ocorrer

    pre_processamento(colunas, nonograma, colunas_completas, false);
    pre_processamento(linhas, nonograma, linhas_completas, true);

    std::queue<int> linhas_to_see;
    std::queue<int> colunas_to_see;

    for(int i{0}; i < n_linhas; ++i){
        if(!linhas_completas[i]){
            linhas_to_see.push(i);
        }
    }

    for(int i{0}; i < n_colunas; ++i){
        if(!colunas_completas[i]){
            colunas_to_see.push(i);
        }
    }

    int to_see;
    std::vector<int> coluna_nonograma(n_linhas);
    std::vector<std::pair<int, int>> rightmost;
    std::vector<std::pair<int, int>> leftmost;
    while(!linhas_to_see.empty() || !colunas_to_see.empty()){
        while(!linhas_to_see.empty()){
            to_see = linhas_to_see.front();
            leftmost = mais_a_esquerda(linhas[to_see], nonograma[to_see], soma_linhas[to_see], n_linhas, n_colunas);
            rightmost = mais_a_direita(linhas[to_see], nonograma[to_see], soma_linhas[to_see], n_linhas, n_colunas);
            
            // Aplicar regra 1

            // Aplicar regra 2

            // Aplicar regra 3

            // Aplicar regra 4

            // Aplicar regra 5

            linhas_to_see.pop();
        }

        while(!colunas_to_see.empty()){
            to_see = colunas_to_see.front();

            for(int i{0}; i < n_linhas; ++i){
                coluna_nonograma[i] = nonograma[i][to_see];
            }


            leftmost = mais_a_cima(colunas[to_see], coluna_nonograma, soma_colunas[to_see], n_colunas, n_linhas);
            rightmost = mais_a_baixo(colunas[to_see], coluna_nonograma, soma_colunas[to_see], n_colunas, n_linhas);

            // Aplicar regra 1

            // Aplicar regra 2

            // Aplicar regra 3

            // Aplicar regra 4

            // Aplicar regra 5

            colunas_to_see.pop();
        }
    }

    for(int i{0}; i < n_linhas; ++i){

        mais_a_esquerda(linhas[i], nonograma[i], soma_linhas[i], n_linhas, n_colunas);
    }


    for(int i{0}; i < n_linhas; ++i){
        for(int j{0}; j < n_colunas; ++j){
            if(nonograma[i][j] == 1){
                std::cout << "O";
            }
            else if(nonograma[i][j] == 0){
                std::cout << "X";
            }
            else{
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }


    // Metodo exato :D

    // Enquanto o nonograma não estiver completo:
    //      Enquanto houver linhas/colunas a serem escolhidas:
    //          Escolhe uma linha/coluna
    //          Aplica as regras de 1 a 5
    //          Adicionar na "fila" as colunas/linhas modificadas pelas regras

    //      Se nonograma não estiver completo:
    //          Chute um valor para um quadrado não completado?
    //          OU DFS completa
    //


    



    return 0;
}