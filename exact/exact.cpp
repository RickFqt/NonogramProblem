#include <bits/stdc++.h>
#define local_ultimo_chute std::get<0>(pilha_recursiva.top())
#define nonograma_antes std::get<1>(pilha_recursiva.top())
#define linhas_completadas_antes std::get<2>(pilha_recursiva.top())
#define colunas_completadas_antes std::get<3>(pilha_recursiva.top())
#define qual_ultimo_chute std::get<4>(pilha_recursiva.top())
#define chute_type std::tuple<std::pair<int, int>, std::vector<std::vector<int>>, std::vector<bool>, std::vector<bool>, bool>

#define PRINT_NONOGRAMA for(int i{0}; i < n_linhas; ++i){for(int j{0}; j < n_colunas; ++j){if(nonograma[i][j] == 1){std::cout << "O";}/*else if(nonograma[i][j] == 0){std::cout << "X";}*/else{std::cout << " ";}}std::cout << std::endl;}

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
        // std::cout << "reverted!" << std::endl;
        std::reverse(numeros_sorteados.begin(), numeros_sorteados.end());
    }
    for(int j : numeros_sorteados){
        // Converte o número sorteado à respectiva alocação de blocos
        vetor_sorteado = convert_number(j, quadrados_vazios, coluna.size(), n_linhas, coluna);
        // std::cout << "Vetor Sorteado: ";
        //     for(int p = 0; p < vetor_sorteado.size(); ++p){
        //         std::cout << vetor_sorteado[p] << " \n"[p == vetor_sorteado.size() - 1];
        //     }
        if(linha_valida(vetor_sorteado, coluna_nonograma)){

            // std::cout << "Vetor Que deu Certo: ";
            // for(int p = 0; p < vetor_sorteado.size(); ++p){
            //     std::cout << vetor_sorteado[p] << " \n"[p == vetor_sorteado.size() - 1];
            // }
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

bool is_nonogram_solved(std::vector<bool> linhas_completadas, std::vector<bool> colunas_completadas){

    for(int i{0}; i < linhas_completadas.size(); ++i){
        if(!linhas_completadas[i]){
            return false;
        }
    }

    for(int i{0}; i < colunas_completadas.size(); ++i){
        if(!colunas_completadas[i]){
            return false;
        }
    }

    return true;
}

void atualiza_afetados(std::vector<int> updated, std::vector<bool> completados,
                       std::vector<bool>& in_to_be_seen, std::queue<int>&to_be_seen){
    int updated_size = updated.size();
    int afetado;

    for(int i{0}; i < updated_size; ++i){
        afetado = updated[i];
        if(!completados[afetado] && !in_to_be_seen[afetado]){
            to_be_seen.push(afetado);
            in_to_be_seen[afetado] = true;
        }
    }
}

void regra_1(std::vector<std::pair<int, int>> leftmost, std::vector<std::pair<int, int>> rightmost,
             std::vector<bool> completados, std::vector<bool>& in_to_be_seen, std::queue<int>&to_be_seen,
             std::vector<std::vector<int>>& nonograma, std::vector<int> blocos, int idx, bool eh_coluna = false){

    
    std::vector<int> afetados;


    // // TODO: ADAPTAR PRA REGRA 2
    // if(leftmost == rightmost){

    //     std::vector<std::pair<int, int>> blau = {{0, -1}};
    //     for(int i{0}; i < leftmost.size(); ++i){
    //         blau.push_back(leftmost[i]);
    //     }
    //     if(eh_coluna){

    //         blau.push_back({nonograma[0].size(), nonograma[0].size()});
    //     }
    //     else{
    //         blau.push_back({nonograma.size(), nonograma.size()});
    //     }


    //     // Preenche todos os X
    //     for(int j{0}; j < blocos.size() + 1; ++j){
    //         if(blau[j+1].first - blau[j].second > 0){

    //             for(int k{blau[j].second + 1}; k <= blau[j+1].first - 1; ++k){
    //                 if(eh_coluna){
    //                     if(nonograma[k][idx] == -1){
    //                         afetados.push_back(k);
    //                         nonograma[k][idx] = 0;
    //                     }
    //                 }
    //                 else{
    //                     if(nonograma[idx][k] == -1){
    //                         afetados.push_back(k);
    //                         nonograma[idx][k] = 0;
    //                     }
    //                 }
    //             }
    //         }
    //     }

    // }
    
    // Preenche todos os quadrados que tenham "overlap"
    for(int j{0}; j < blocos.size(); ++j){
        if(leftmost[j].second - rightmost[j].first >= 0){

            for(int k{rightmost[j].first}; k <= leftmost[j].second; ++k){
                if(eh_coluna){
                    if(nonograma[k][idx] == -1){
                        afetados.push_back(k);
                        nonograma[k][idx] = 1;
                    }
                }
                else{
                    if(nonograma[idx][k] == -1){
                        afetados.push_back(k);
                        nonograma[idx][k] = 1;
                    }
                }
            }
        }
    }


    atualiza_afetados(afetados, completados, in_to_be_seen, to_be_seen);

}

void regra_2(std::vector<std::pair<int, int>> leftmost, std::vector<std::pair<int, int>> rightmost,
             std::vector<bool> completados, std::vector<bool>& in_to_be_seen, std::queue<int>&to_be_seen,
             std::vector<std::vector<int>>& nonograma, std::vector<int> blocos, int idx, bool eh_coluna = false){

    
    std::vector<int> afetados;

    // Vê os quadrados entre o canto da esquerda e o primeiro bloco leftmost
    for(int i{0}; i < leftmost[0].first; ++i){
        if(eh_coluna){
            if(nonograma[i][idx] == -1){
                afetados.push_back(i);
                nonograma[i][idx] = 0;
            }
            
        }
        else{
            if(nonograma[idx][i] == -1){
                afetados.push_back(i);
                nonograma[idx][i] = 0;
            }
        }
    }

    // Vê os quadrados entre o canto da direita e o ultimo bloco rightmost
    for(int i{rightmost[rightmost.size()-1].second + 1}; i < (eh_coluna ? nonograma.size() : nonograma[0].size()); ++i){
        if(eh_coluna){
            if(nonograma[i][idx] == -1){
                afetados.push_back(i);
                nonograma[i][idx] = 0;
            }
            
        }
        else{
            if(nonograma[idx][i] == -1){
                afetados.push_back(i);
                nonograma[idx][i] = 0;
            }
        }
    }

    // Vê os quadrados entre cada leftmost e rightmost
    for(int i{0}; i < blocos.size() - 1; ++i){
        if(rightmost[i].second - leftmost[i+1].first < 0){
            for(int j{rightmost[i].second+ 1}; j < leftmost[i+1].first; ++j){
                if(eh_coluna){
                    if(nonograma[j][idx] == -1){
                        afetados.push_back(j);
                        nonograma[j][idx] = 0;
                    }
                    
                }
                else{
                    if(nonograma[idx][j] == -1){
                        afetados.push_back(j);
                        nonograma[idx][j] = 0;
                    }
                }
            }
        }
    }


    if(leftmost == rightmost){
        if(eh_coluna){
            for(int i{0}; i < nonograma.size(); ++i){
                if(nonograma[i][idx] == -1){
                    afetados.push_back(i);
                    nonograma[i][idx] = 0;
                }
            }
        }
        else{
            for(int i{0}; i < nonograma[0].size(); ++i){
                if(nonograma[idx][i] == -1){
                    afetados.push_back(i);
                    nonograma[idx][i] = 0;
                }
            }
        }
    }

    atualiza_afetados(afetados, completados, in_to_be_seen, to_be_seen);

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

    // std::cout << "Preprocesso colunas..." << std::endl;
    pre_processamento(colunas, nonograma, colunas_completas, false);
    // PRINT_NONOGRAMA
    // std::cout << "-----------------" << std::endl;

    //std::cout << "Preprocesso..." << std::endl;
    pre_processamento(linhas, nonograma, linhas_completas, true);
    //PRINT_NONOGRAMA
    //std::cout << "-----------------" << std::endl;

    std::queue<int> linhas_to_see;
    std::queue<int> colunas_to_see;
    std::vector<bool> in_linhas_to_see(n_linhas);
    std::vector<bool> in_colunas_to_see(n_colunas);

    for(int i{0}; i < n_linhas; ++i){
        if(!linhas_completas[i]){
            linhas_to_see.push(i);
            in_linhas_to_see[i] = true;
        }
    }

    for(int i{0}; i < n_colunas; ++i){
        if(!colunas_completas[i]){
            colunas_to_see.push(i);
            in_colunas_to_see[i] = true;
        }
    }

    int to_see;
    std::vector<int> coluna_nonograma(n_linhas);
    std::vector<std::pair<int, int>> rightmost;
    std::vector<std::pair<int, int>> leftmost;

    // Tupla de recursão, usada caso não seja possível aplicar mais regras lógicas
    // Argumento 0 (pair<int, int>): local onde o "chute" foi realizado;
    // Argumento 1 (vector<vector<int>>): nonograma antes do chute;
    // Argumento 2 (vector<bool>): vetor de linhas completas;
    // Argumento 3 (vector<bool>): vetor de colunas completas;
    // Argumento 4 (bool): identifica se o chute foi X (falso) ou 0 (verdadeiro);
    std::stack<std::tuple<std::pair<int, int>, std::vector<std::vector<int>>, std::vector<bool>, std::vector<bool>, bool>> pilha_recursiva;
    bool unsolvable = false;
    bool absurdo = false;

    while(!is_nonogram_solved(linhas_completas, colunas_completas) || unsolvable){

        //std::cout << "Vou tentar..." << std::endl;
        //std::cout << "Doidera 0" << std::endl;
        while(!linhas_to_see.empty() || !colunas_to_see.empty()){
            if(absurdo){
                break;
            }
            //std::cout << "Doidera 0.5" << std::endl;
            //std::cout << "Tentando Linhas..." <<std::endl;
            while(!linhas_to_see.empty()){
                //std::cout << "Doidera 0.7 " << std::endl;
                //std::cout << linhas_to_see.size() << std::endl;
                to_see = linhas_to_see.front();
                leftmost = mais_a_esquerda(linhas[to_see], nonograma[to_see], soma_linhas[to_see], n_linhas, n_colunas);

                // Ver aqui se deu ruim
                if(leftmost[0].first == -1){
                    //std::cout << "ABSURDO!!!!!!!" << std::endl;
                    absurdo = true;
                    break;
                }

                rightmost = mais_a_direita(linhas[to_see], nonograma[to_see], soma_linhas[to_see], n_linhas, n_colunas);

                // Aplicar regra 1
                regra_1(leftmost, rightmost, colunas_completas, in_colunas_to_see, colunas_to_see, nonograma, linhas[to_see], to_see);


                // Aplicar regra 2
                regra_2(leftmost, rightmost, colunas_completas, in_colunas_to_see, colunas_to_see, nonograma, linhas[to_see], to_see);

                // Aplicar regra 3

                // Aplicar regra 4

                // Aplicar regra 5

                if(leftmost == rightmost){
                    linhas_completas[to_see] = true;
                }

                linhas_to_see.pop();
                in_linhas_to_see[to_see] = false;

                //PRINT_NONOGRAMA
                //std::cout << "-----------------" << std::endl;

            }
            //std::cout << "Errado 1" << std::endl;

            //std::cout << "Tentando Colunas..." <<std::endl;
            while(!colunas_to_see.empty()){
                //std::cout << "Doidera 0.8" << std::endl;
                //std::cout << colunas_to_see.size() << std::endl;
                if(absurdo){
                    break;
                }

                to_see = colunas_to_see.front();

                for(int i{0}; i < n_linhas; ++i){
                    coluna_nonograma[i] = nonograma[i][to_see];
                }

                //std::cout << "to_see:" << to_see << std::endl;
                //std::cout << "Antes mais a cima" << std::endl;

                leftmost = mais_a_cima(colunas[to_see], coluna_nonograma, soma_colunas[to_see], n_colunas, n_linhas);

                //std::cout << "Depois mais a cima" << std::endl;

                // Ver aqui se deu ruim

                if(leftmost[0].first == -1){
                    absurdo = true;
                    break;
                }

                //std::cout << "Antes mais a baixo" << std::endl;

                rightmost = mais_a_baixo(colunas[to_see], coluna_nonograma, soma_colunas[to_see], n_colunas, n_linhas);

                //std::cout << "Depois mais a baixo" << std::endl;
                // for(int i{0}; i < leftmost.size(); ++i){
                //     std::cout << "("<<leftmost[i].first<<","<<leftmost[i].second<<") ";
                // }
                // std::cout << std::endl;
                // for(int i{0}; i < rightmost.size(); ++i){
                //     std::cout << "("<<rightmost[i].first<<","<<rightmost[i].second<<") ";
                // }
                // std::cout << std::endl;

                //std::cout << "Antes regra 1" << std::endl;

                // Aplicar regra 1
                regra_1(leftmost, rightmost, linhas_completas, in_linhas_to_see, linhas_to_see, nonograma, colunas[to_see], to_see, true);

                //std::cout << "Depois regra 1" << std::endl;
                // Aplicar regra 2
                regra_2(leftmost, rightmost, linhas_completas, in_linhas_to_see, linhas_to_see, nonograma, colunas[to_see], to_see, true);

                // Aplicar regra 3

                // Aplicar regra 4

                // Aplicar regra 5

                if(leftmost == rightmost){
                    colunas_completas[to_see] = true;
                }

                colunas_to_see.pop();
                in_colunas_to_see[to_see] = false;

                //PRINT_NONOGRAMA
                //std::cout << "-----------------" << std::endl;
            }
        }

        //std::cout << "Errado 2" << std::endl;

        // std::cout << "Nao consegui, ficou assim:" << std::endl;
        // PRINT_NONOGRAMA
        // break;
        if(absurdo){
            //std::cout << "Achei absurdo! Voltando pro anterior:" << std::endl;
            while(!linhas_to_see.empty()){
                in_linhas_to_see[linhas_to_see.front()] = false;
                linhas_to_see.pop();
            }
            while(!colunas_to_see.empty()){
                in_colunas_to_see[colunas_to_see.front()] = false;
                colunas_to_see.pop();
            }

            if(pilha_recursiva.empty()){
                unsolvable = true;
            }
            else{
                if(qual_ultimo_chute == false){
                    //std::cout << "Vou chutar o " << local_ultimo_chute.first << " " << local_ultimo_chute.second << " com 0, assim:" << std::endl;
                    qual_ultimo_chute = true;
                    
                }
                else{
                    //std::cout << "Deu tudo errado, voltando pro penultimo chute...: ";
                    pilha_recursiva.pop();
                    if(!pilha_recursiva.empty()){

                        //std::cout << "(" << local_ultimo_chute.first << ", " << local_ultimo_chute.second << ")" << std::endl;
                    }
                }

                if(pilha_recursiva.empty()){
                    unsolvable = true;
                }
                else{
                    nonograma = nonograma_antes;
                    //PRINT_NONOGRAMA;
                    linhas_completas = linhas_completadas_antes;
                    colunas_completas = colunas_completadas_antes;

                    nonograma[local_ultimo_chute.first][local_ultimo_chute.second] = 1;
                    //PRINT_NONOGRAMA
                    

                    linhas_to_see.push(local_ultimo_chute.first);
                    colunas_to_see.push(local_ultimo_chute.second);
                }
            }

            absurdo = false;
        }
        else if(!is_nonogram_solved(linhas_completas, colunas_completas)){
            // Achar um local pra chutar
            //std::cout << "Nao consegui, ficou assim:" << std::endl;
            //PRINT_NONOGRAMA

            // for(int i{0}; i < linhas_completas.size(); ++i){
            //     if(linhas_completas[i]){
            //         std::cout << "1 ";
            //     }
            //     else{
            //         std::cout << "0 ";
            //     }
            // }
            // std::cout << std::endl;

            // for(int i{0}; i < colunas_completas.size(); ++i){
            //     if(colunas_completas[i]){
            //         std::cout << "1 ";
            //     }
            //     else{
            //         std::cout << "0 ";
            //     }
            // }
            // std::cout << std::endl;
            
            //std::cout << "Nao consegui, 1:" << std::endl;
            std::vector<int> sorteio;
            std::pair<int, int> local_sorteado = {0, 0};
            if(rand() % 2){
                // Sorteia um local com base em uma linha ainda nao completa
                for(int i{0}; i < linhas_completas.size(); ++i){
                    if(!linhas_completas[i]){
                        sorteio.push_back(i);
                    }
                }

                std::random_shuffle(sorteio.begin(), sorteio.end());

                for(int i{0}; i < n_colunas; ++i){
                    if(nonograma[sorteio[0]][i] == -1){
                        local_sorteado = {sorteio[0], i};
                        break;
                    }
                }

            }
            else{
                // Sorteia um local com base em uma coluna ainda nao completa
                for(int i{0}; i < colunas_completas.size(); ++i){
                    if(!colunas_completas[i]){
                        sorteio.push_back(i);
                    }
                }

                std::random_shuffle(sorteio.begin(), sorteio.end());

                for(int i{0}; i < n_linhas; ++i){
                    if(nonograma[i][sorteio[0]] == -1){
                        local_sorteado = {i, sorteio[0]};
                        break;
                    }
                }
            }

            //std::cout << "Nao consegui, 2:" << std::endl;
            // Salvar o estado do nonograma, e qual foi o chute realizado
            chute_type chute(local_sorteado, nonograma, linhas_completas, colunas_completas, false);
            pilha_recursiva.push(chute);

            //std::cout << "Nao consegui, 3:" << std::endl;
            
            // Fazer o chute
            nonograma[local_ultimo_chute.first][local_ultimo_chute.second] = 0;
            //std::cout << "Vou chutar o " << local_sorteado.first << " " << local_sorteado.second << " com X, assim:" << std::endl;
            //PRINT_NONOGRAMA

            linhas_to_see.push(local_ultimo_chute.first);
            colunas_to_see.push(local_ultimo_chute.second);
        }
        
    }


    if(unsolvable){
        std::cout << "Sem solução :D" << std::endl;
    }

    PRINT_NONOGRAMA


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