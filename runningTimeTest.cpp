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

// Funcao objetivo calculada durante o algoritmo
int funcao_objetivo(std::vector<std::vector<int>>linhas,
                std::vector<std::vector<int>>colunas,
                std::vector<std::vector<int>>nonograma,
                std::vector<bool>linha_analisada, 
                int idx_linha_analisada,
                int linhas_preenchidas){
    

    // Primeira parcela do somatorio
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


    // Segunda parcela do somatório
    int sum2 = 0, c_pk = 0, b_pk = 0;
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
                       std::vector<std::vector<int>>& nonograma){
    
    int n_colunas = colunas.size();
    int n_linhas = nonograma.size();
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
                nonograma[j][i] = 0;
            }
        }

        // Preenche todos os quadrados que tenham "overlap"
        for(int j{0}; j < colunas[i].size(); ++j){
            if(inicio_fim[j].second - fim_inicio[j].first >= 0){

                for(int k{fim_inicio[j].first}; k <= inicio_fim[j].second; ++k){
                    nonograma[k][i] = 1;
                }
            }
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


std::vector<bool> escolher_linha(std::vector<std::vector<bool>>linhas_construidas, std::vector<std::vector<int>>linhas,
                                 std::vector<std::vector<int>>colunas, std::vector<std::vector<int>>nonograma,
                                 int idx_linha_analisada, int linhas_preenchidas){
    int min = INT_MAX;
    int i_min = 0;
    int current;

    for(int i{0}; i < linhas_construidas.size(); ++i){
        current = funcao_objetivo(linhas, colunas, nonograma, linhas_construidas[i], idx_linha_analisada, linhas_preenchidas);
        if (current < min){
            i_min = i;
            min = current;
        }
    }

    return linhas_construidas[i_min];

}


int N_REPETITIONS = 50;

int main(int argc, char * argv[]){

    std::srand ( unsigned ( std::time(0) ) );

    std::string testCases[12] = {
        "./inputs/dancer.txt",
        "./inputs/cat.txt",
        "./inputs/skid.txt",
        "./inputs/bucks.txt",
        "./inputs/edge.txt",
        "./inputs/smoke.txt",
        "./inputs/knot.txt",
        "./inputs/swing.txt",
        "./inputs/mum.txt",
        "./inputs/dicap.txt",
        "./inputs/tragic.txt",
        "./inputs/merka.txt",
        // "./inputs/petro.txt",
        // "./inputs/m&m.txt",
        // "./inputs/signed.txt",
        // "./inputs/light.txt",
        // "./inputs/forever.txt",
        // "./inputs/center.txt",
        // "./inputs/hot.txt",
        // "./inputs/karate.txt"
    };

    std::ofstream out_file;
    out_file.open ("outputTime.txt");
    for(int times = 0; times < 12; ++times){
        double elapsed_time_mean = 0;
        double media_objetivo = 0;
        int n_sucessos = 0;
        int objetivo_min = INT_MAX;
        int objetivo_max = -1;

        std::ifstream file(testCases[times]);
        if (!file) {
            std::cerr << "Não foi possível abrir o arquivo: " << testCases[times] << std::endl;
            return 1;
        }

        // Ler um arquivo aí
        
        std::string entrada;

        std::getline(file, entrada);
        auto tokenLine = tokenize(entrada);

        int n_linhas, n_colunas;
        n_linhas = tokenLine[0];
        n_colunas = tokenLine[1];


        std::vector<std::vector<int>> linhas; // Guarda o valor dos blocos de cada linha
        std::vector<int> soma_linhas(n_linhas); // Guarda a soma do tamanho dos blocos de cada linha
        std::vector<std::vector<int>> colunas; // Guarda o valor dos blocos de cada coluna
        std::vector<int> soma_colunas(n_colunas); // Guarda a soma do tamanho dos blocos de cada coluna

        // Lê as linhas
        for(int i{0}; i < n_linhas; ++i){
            std::getline(file, entrada);
            linhas.push_back(tokenize(entrada));

            // Calcula a soma de linhas
            soma_linhas[i] = soma_vetor(linhas[i]);
        }

        // Lê as colunas
        for(int i{0}; i < n_colunas; ++i){
            std::getline(file, entrada);
            colunas.push_back(tokenize(entrada));

            // Calcula a soma de colunas
            soma_colunas[i] = soma_vetor(colunas[i]);
        }
        file.close();

        for(int repetitions = 0; repetitions < N_REPETITIONS; ++ repetitions){

            

            /// ============================== TIMER COMEÇA ================================================
            auto start = std::chrono::steady_clock::now();


            // Matriz do nonograma
            // Cada célula contém um inteiro (-1, 0 ou 1)
            // -1 --> Célula ainda não marcada
            // 0 --> Célula marcada com X
            // 1 --> Célula colorida
            std::vector<std::vector<int>> nonograma(n_linhas, std::vector<int>(n_colunas, -1));

            // Pré-processamento: percorre as colunas, e marca os quadrados que são certos de ocorrer

            pre_processamento(colunas, nonograma);


            // Algoritmo Heurístico :D

            // Enquanto não estiver completo:
            //      Escolher a proxima linha
            //      Fazer 10 construcoes para a linha corretamente sem se preocupar com as colunas
            //      Escolher a linha construída que menos causa conflito com as colunas

            std::vector<int> sequencia_linhas;

            for(int i{0}; i < n_linhas; ++i){
                sequencia_linhas.push_back(i);
            }
            std::random_shuffle(sequencia_linhas.begin(), sequencia_linhas.end());

            // Enquanto não estiver completo:
            for(int k{0}; k < n_linhas; ++k){
                // Indice da linha atual
                int i = sequencia_linhas[k];
                //Preencher a linha

                
                int quadrados_vazios = n_colunas - soma_linhas[i]; // Número de quadrados vazios necessários nessa linha
                if(quadrados_vazios < 0){
                    std::cerr << "Nonograma sem solução1! Impossível preencher linha " << i << "\n";
                    return 1;
                }

                int combinao = combinacao(quadrados_vazios + 1, linhas[i].size()); // Número máximo de possíveis alocações dos blocos
                
                int tamanho = std::min(combinao, 100);
                std::vector<std::vector<bool>> linhas_construidas;

                if(soma_linhas[i] != 0){
                    
                    std::vector<int> numeros_sorteados;
                    std::vector<bool> vetor_sorteado;

                    for(int j{1}; j <= combinao; ++j){
                        numeros_sorteados.push_back(j);
                    }
                    std::random_shuffle(numeros_sorteados.begin(), numeros_sorteados.end());

                    int index_sorteado = 0;
                    //std::cout << "Combinacoes = " << combinao << std::endl;
                    for(int j{0}; j < tamanho; ++j){

                        int sorteado = numeros_sorteados[index_sorteado];

                        // Converte o número sorteado à respectiva alocação de blocos
                        vetor_sorteado = convert_number(sorteado, quadrados_vazios, linhas[i].size(), n_colunas, linhas[i]);

                        if(linha_valida(vetor_sorteado, nonograma[i])){
                            linhas_construidas.push_back(vetor_sorteado);
                        }

                        if(j == tamanho - 1 && linhas_construidas.empty()){
                            linhas_construidas.push_back(vetor_sorteado);
                        }

                        ++index_sorteado;

                        if(index_sorteado >= combinao){
                            break;
                        }
                    }

                    if(linhas_construidas.empty()){
                        std::cerr << "Nonograma sem solução2! Impossível preencher linha " << i << "\n";
                        return 1;
                    }



                }
                // std::cout << "batata4" << std::endl;

                std::vector<bool> linha_escolhida;
                if(soma_linhas[i] == 0){
                    linha_escolhida = std::vector<bool>(n_colunas);
                }
                else{

                    linha_escolhida = escolher_linha(linhas_construidas, linhas, colunas, nonograma, i, k);

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
            // for(int i{0}; i < n_linhas; ++i){
            //     for(int j{0}; j < n_colunas; ++j){
            //         if(nonograma[i][j] == 1){
            //             std::cout << "O";
            //         }
            //         else{
            //             std::cout << " "; 
            //         }
            //     }
            //     std::cout << std::endl;
            // }

            int objetivo_final = funcao_objetivo(linhas, colunas, nonograma);
            // std::cout << std::endl << "Funcao objetivo final: " << objetivo_final << std::endl;




            auto end = std::chrono::steady_clock::now();
            /// ============================== TIMER TERMINA ===============================================
            auto diff( end - start );

            elapsed_time_mean += ( std::chrono::duration <double, std::milli> (diff).count() );
            media_objetivo += objetivo_final;
            if(objetivo_final == 0){
                n_sucessos++;
            }
            if(objetivo_final < objetivo_min){
                objetivo_min = objetivo_final;
            }
            if(objetivo_final > objetivo_max){
                objetivo_max = objetivo_final;
            }
        }

        elapsed_time_mean = elapsed_time_mean/N_REPETITIONS;
        media_objetivo = media_objetivo/N_REPETITIONS;
        double media_sucessos = n_sucessos/N_REPETITIONS;
        

        out_file << "# Medição do tempo do teste "<< times << ": " << std::to_string(elapsed_time_mean) << "ms com média de objetivo de " << media_objetivo << " e n de sucessos de "<< n_sucessos <<" e funcao minima de " << objetivo_min <<" e funcao maxima de " << objetivo_max << "\n";
    }

    out_file.close();


    return EXIT_SUCCESS;

}