#include <bits/stdc++.h>

// ================================ Parâmetros para a meta-heurística no gral ================================

// Limite de iterações da busca
int  LIMIT_ITERATIONS = 10;
// Define se usaremos ou não path-relinking a cada 5 iterações
bool SET_PATH_RELINKING = true;

// ===========================================================================================================

// ================================ Parâmetros da busca tabu ================================

// Limite de iterações para a busca local na vizinhanca;
int LIMIT_LOCAL_ITERATIONS = 60;
// Define se escolhemos o primeiro vizinho melhor que a solução corrente, ou
// se analisamos todos os vizinhos antes de realizar uma escolha
bool USE_PRIMEIRO_VIZINHO_MELHOR = true;
// Define se sairemos de uma busca na vizinhança caso fiquemos "presos" em uma solução por muito tempo
bool USE_NUMERO_REPETICOES_PERMITIDA = true;
// Caso fiquemos "presos" esse número de iterações em uma mesma solução, paramos a busca na vizinhança
int NUMERO_REPETICOES_PERMITIDA = 40;

// ==========================================================================================

// ================================ Parâmetros da medição de tempo ================================

int TEST_REPETITIONS = 1;

// ================================================================================================

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

std::vector<std::pair<int, int>> calcula_pos_blocos_linha(std::vector<std::vector<int>>const & nonograma, int const &  idx_linha){

    std::vector<std::pair<int, int>> pos_blocos_linha;
    int n_colunas = nonograma[0].size();
    bool continuidade = false;
    int idx_bloco = -1;
    for(int i{0}; i < n_colunas; ++i){

        if(nonograma[idx_linha][i] == 1){

            if(!continuidade){
                
                continuidade = true;
                ++idx_bloco;
                pos_blocos_linha.push_back({i, i});
            }
            else{
                pos_blocos_linha[idx_bloco].second++;
            }
        }
        else{
            continuidade = false;
        }
    }

    return pos_blocos_linha;
}

// Funcao objetivo calculada durante o algoritmo
int funcao_objetivo(std::vector<std::vector<int>>linhas,
                std::vector<std::vector<int>>colunas,
                std::vector<std::vector<int>>nonograma,
                std::vector<bool>linha_analisada, 
                int idx_linha_analisada,
                int linhas_preenchidas){
    
    if(linhas_preenchidas == linhas.size()){
        linhas_preenchidas--;
    }

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

bool is_linha_igual(const std::vector<int>& linha1, const std::vector<bool>& linha2){
    
    int tam_linha = linha1.size();
    for(int i{0}; i < tam_linha; ++i){
        if(linha1[i] != (int)linha2[i]){
            return false;
        }
    }

    return true;
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

struct ListaTabu{
    int tamanho_lista;
    int current_index;
    bool full;
    std::vector<int> storage;

    ListaTabu(int tamanho){
        tamanho_lista = tamanho;
        current_index = -1;
        full = false;
        storage = std::vector<int>(tamanho);
    }

    void insert(int item){
        if(current_index == tamanho_lista - 1){
            full = true;
        }

        current_index = (current_index + 1) % tamanho_lista;
        storage[current_index] = item;
    }

    bool isTabu(int item){
        int last_pos = full ? tamanho_lista : current_index + 1;
        for(int i{0}; i < last_pos; ++i){
            if(storage[i] == item){
                return true;
            }
        }
        return false;
    }

    void reset(){
        current_index = -1;
        full = false;
    }

};

void busca_tabu(std::vector<std::vector<int>>& nonograma, int &melhor_objetivo_global,
                std::vector<std::vector<int>>linhas, std::vector<int> soma_linhas,
                std::vector<std::vector<int>>colunas, std::vector<int> soma_colunas,
                std::vector<int> combinacoes_linhas){

    int n_linhas = nonograma.size();
    int n_colunas = nonograma[0].size();
    int tamanho_lista = floor( sqrt( (double)n_linhas ) );
    
    ListaTabu lista_tabu(tamanho_lista);

    std::vector<std::vector<int>> nonograma_inicial = nonograma;
    std::vector<std::vector<int>> nonograma_vizinho;
    std::vector<std::vector<int>> melhor_nonograma_local;
    int best_linha_vizinho;
    int melhor_objetivo_local;
    int current_objetivo;

    int reps_melhor_solucao = 0;


    for(int i{0}; i < LIMIT_LOCAL_ITERATIONS; ++i){

        // lista_tabu.reset();
        melhor_objetivo_local = INT_MAX;

        std::vector<int> sequencia_linhas;

        for(int i{0}; i < n_linhas; ++i){
            sequencia_linhas.push_back(i);
        }
        std::random_shuffle(sequencia_linhas.begin(), sequencia_linhas.end());


        int linha_atual;
        // Gerar os vizinhos e escolher o melhor
        for(int numero_vizinho{0}; numero_vizinho < n_linhas; ++numero_vizinho){
            
            linha_atual = sequencia_linhas[numero_vizinho];
            nonograma_vizinho = nonograma_inicial;

            // Fazer 100 construcoes para a linha corretamente sem se preocupar com as colunas

            // Escolher a linha construída que menos causa conflito com as colunas
            int quadrados_vazios = n_colunas - soma_linhas[linha_atual]; // Número de quadrados vazios necessários nessa linha
            int combinao = combinacoes_linhas[linha_atual];
            int tamanho = std::min(combinao, 10);
            std::vector<std::vector<bool>> linhas_construidas;

            if(soma_linhas[linha_atual] != 0){
                
                std::vector<int> numeros_sorteados;
                std::vector<bool> vetor_sorteado;

                for(int j{1}; j <= combinao; ++j){
                    numeros_sorteados.push_back(j);
                }
                std::random_shuffle(numeros_sorteados.begin(), numeros_sorteados.end());

                int index_sorteado = 0;
                for(int j{0}; j < tamanho; ++j){

                    int sorteado = numeros_sorteados[index_sorteado];

                    // Converte o número sorteado à respectiva alocação de blocos
                    vetor_sorteado = convert_number(sorteado, quadrados_vazios, linhas[linha_atual].size(), n_colunas, linhas[linha_atual]);

                    if(linha_valida(vetor_sorteado, nonograma[linha_atual])){
                        if(!is_linha_igual(nonograma_inicial[linha_atual], vetor_sorteado)){

                            linhas_construidas.push_back(vetor_sorteado);
                        }
                    }

                    if(j == tamanho - 1 && linhas_construidas.empty()){
                        linhas_construidas.push_back(vetor_sorteado);
                    }

                    ++index_sorteado;

                    if(index_sorteado >= combinao){
                        break;
                    }
                }



            }


            std::vector<bool> linha_escolhida(n_colunas);
            if(soma_linhas[linha_atual] == 0){
                linha_escolhida = std::vector<bool>(n_colunas);
            }
            else{

                linha_escolhida = escolher_linha(linhas_construidas, linhas, colunas, nonograma, linha_atual, n_linhas);

            }
            // Escolhe a linha com menor conflito


            // Preenche o nonograma com a linha escolhida
            for(int j{0}; j < n_colunas; ++j){
                if(linha_escolhida[j]){

                    nonograma_vizinho[linha_atual][j] = 1;
                }
                else{
                    nonograma_vizinho[linha_atual][j] = 0;
                }
            }



            // GEREI O VIZINHO!!! Agora, vejo se ele é tabu e se é melhor que o melhor
            current_objetivo = funcao_objetivo(linhas, colunas, nonograma_vizinho);

            // Se o melhor da vizinhanca
            if( lista_tabu.isTabu(linha_atual) && current_objetivo < melhor_objetivo_global ){
                if(current_objetivo < melhor_objetivo_local){

                    melhor_objetivo_local = current_objetivo;
                    melhor_nonograma_local = nonograma_vizinho;
                    best_linha_vizinho = linha_atual;
                }
            }
            else if(current_objetivo < melhor_objetivo_local){
                melhor_objetivo_local = current_objetivo;
                melhor_nonograma_local = nonograma_vizinho;
                best_linha_vizinho = linha_atual;
            }

            if(USE_PRIMEIRO_VIZINHO_MELHOR){

                // Se achou algo melhor que o global, PARE IMEDIATAMENTE
                if(current_objetivo < melhor_objetivo_global){
                    break;
                }
            }


        }

        lista_tabu.insert(best_linha_vizinho);
        if(melhor_objetivo_local < melhor_objetivo_global){
            melhor_objetivo_global = melhor_objetivo_local;
            nonograma = melhor_nonograma_local;
            reps_melhor_solucao = 0;
        }
        else{
            reps_melhor_solucao++;
        }

        // Caso estivermos "presos" em uma solução por muito tempo, paramos de olhar os vizinhos
        if(USE_NUMERO_REPETICOES_PERMITIDA && reps_melhor_solucao >= NUMERO_REPETICOES_PERMITIDA){
            break;
        }


        nonograma_inicial = melhor_nonograma_local;


        if(melhor_objetivo_global == 0){
            break;
        }
    }




}

std::vector<std::vector<int>> path_relinking_dois_a_dois(std::vector<std::vector<int>>const & nonograma1,
                                                         std::vector<std::vector<int>>const & nonograma2, int objetivo_nonograma2,
                                                         std::vector<std::vector<int>>const & linhas, std::vector<std::vector<int>>const & colunas){
    
    std::vector<std::vector<int>> nonograma_relink = nonograma1;
    int objetivo_relink;
    std::vector<std::vector<int>> nonograma_resultante = nonograma2;
    int objetivo_resultante = objetivo_nonograma2;
    int n_linhas = nonograma1.size();
    int n_colunas = nonograma1[0].size();

    std::vector<std::pair<int, int>> pos_blocos1;
    std::vector<std::pair<int, int>> pos_blocos2;
    
    for(int i{0}; i < n_linhas; ++i){

        int n_blocos = linhas[i].size();
        pos_blocos1 = calcula_pos_blocos_linha(nonograma1, i);
        pos_blocos2 = calcula_pos_blocos_linha(nonograma2, i);

        if(pos_blocos1.size() == 0){
            continue;
        }

        // Copia os blocos da i-esima linha de nonograma2 para nonograma_relink, um por um, e calcula a funcao objetivo em cada passo
        for(int bloco_atual{0}; bloco_atual < n_blocos; ++bloco_atual){

            int blocos_a_mudar = 1;
            while(pos_blocos1[bloco_atual + blocos_a_mudar - 1].second < pos_blocos2[bloco_atual + blocos_a_mudar - 1].second){
                
                if(bloco_atual + blocos_a_mudar >= n_blocos){
                    break;
                }
                else if(pos_blocos1[bloco_atual + blocos_a_mudar].first > pos_blocos2[bloco_atual + blocos_a_mudar - 1].second + 1){
                    break;
                }
                else{
                    blocos_a_mudar++;
                }

            }

            // Apaga os blocos anteriores em nonograma_relink
            for(int j{0}; j < blocos_a_mudar; ++j){
                for(int k{pos_blocos1[bloco_atual+j].first}; k <= pos_blocos1[bloco_atual+j].second; ++k){
                    nonograma_relink[i][k] = 0;
                }
            }

            // Copia os blocos escolhidos para nonograma_relink
            for(int j{0}; j < blocos_a_mudar; ++j){
                for(int k{pos_blocos2[bloco_atual+j].first}; k <= pos_blocos2[bloco_atual+j].second; ++k){
                    nonograma_relink[i][k] = 1;
                }
            }

            bloco_atual += blocos_a_mudar - 1;


            // Calcula a funcao objetivo apos a copia do(s) bloco(s)
            objetivo_relink = funcao_objetivo(linhas, colunas, nonograma_relink);

            if(objetivo_relink < objetivo_resultante){
                objetivo_resultante = objetivo_relink;
                nonograma_resultante = nonograma_relink;
            }

        }
    }

    return nonograma_resultante;
}

std::vector<std::vector<int>> path_relinking(std::vector<std::vector<std::vector<int>>>& nonogramas, std::vector<int>objetivo_nonogramas,
                                             std::vector<std::vector<int>>linhas, std::vector<int> soma_linhas,
                                             std::vector<std::vector<int>>colunas, std::vector<int> soma_colunas){
    
    std::vector<std::vector<int>> nonograma_result;

    std::vector<std::vector<std::vector<int>>> nonogramas_to_be_chosen = nonogramas;
    std::vector<std::pair<int, int>> objetivo_and_idx;

    for(int i{0}; i < nonogramas.size(); ++i){
        objetivo_and_idx.push_back({objetivo_nonogramas[i], i});
    }

    int idx1, idx2;
    int c1, c2;
    // Fazer path relinking dois a dois até ter só um restante
    for(int i{0}; i < nonogramas.size() - 1; ++i){
        int dif_min = INT_MAX;
        std::sort(objetivo_and_idx.begin(), objetivo_and_idx.end());


        for(int j{0}; j < nonogramas.size() - 1 - i; ++j){
            if(abs(objetivo_and_idx[j].first - objetivo_and_idx[j+1].first) < dif_min){
                dif_min = abs(objetivo_and_idx[j].first - objetivo_and_idx[j+1].first);
                idx1 = objetivo_and_idx[j].second;
                idx2 = objetivo_and_idx[j+1].second;
                c1 = j;
                c2 = j+1;
            }
        }

        nonograma_result = path_relinking_dois_a_dois(nonogramas_to_be_chosen[idx2], nonogramas_to_be_chosen[idx1], objetivo_nonogramas[idx1], linhas, colunas);
        nonogramas_to_be_chosen.push_back(nonograma_result);

        objetivo_and_idx[c1].first = INT_MAX;
        objetivo_and_idx[c2].first = INT_MAX;
        objetivo_and_idx.push_back({funcao_objetivo(linhas, colunas, nonograma_result) ,nonogramas_to_be_chosen.size() - 1});




    }

    


    return nonograma_result;
}

int main(){
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

    std::string outputs[12] = {
        "outputs/outdancer.txt",
        "outputs/outcat.txt",
        "outputs/outskid.txt",
        "outputs/outbucks.txt",
        "outputs/outedge.txt",
        "outputs/outsmoke.txt",
        "outputs/outknot.txt",
        "outputs/outswing.txt",
        "outputs/outmum.txt",
        "outputs/outdicap.txt",
        "outputs/outtragic.txt",
        "outputs/outmerka.txt",
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

    for(int times = 0; times < 12; ++times){
        


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

        /// ============================== RODAR OS TESTES PARA A INSTANCIA ================================================
        double elapsed_time_mean = 0;
        double time_min = MAXFLOAT;
        double time_max = -1;
        double media_objetivo = 0;
        int objetivo_min = INT_MAX;
        int objetivo_max = -1;

        for(int run{0}; run < TEST_REPETITIONS; ++run){

            
            /// ============================== TIMER COMEÇA ================================================
            auto start = std::chrono::steady_clock::now();


            // Matriz do nonograma
            // Cada célula contém um inteiro (-1, 0 ou 1)
            // -1 --> Célula ainda não marcada
            // 0 --> Célula marcada com X
            // 1 --> Célula colorida
            std::vector<std::vector<int>> nonograma(n_linhas, std::vector<int>(n_colunas, -1));
            int objetivo_atual = INT_MAX;

            // Vetor de nonogramas encontrados durante a execução. Salva os últimos 5 nonogramas calculados, a fim de fazer o pathrelinking entre eles.
            std::vector<std::vector<std::vector<int>>> nonogramas(5, std::vector<std::vector<int>>(n_linhas, std::vector<int>(n_colunas, -1)));
            std::vector<int> objetivo_nonogramas(5, INT_MAX);

            std::vector<std::vector<int>> nonograma_best(n_linhas, std::vector<int>(n_colunas, -1));
            int objetivo_best = INT_MAX;

            // Vetor para armazenar o numero de combinacoes maximo de cada linha
            std::vector<int> combinacoes_linhas(n_linhas);


            // Pré-processamento: percorre as colunas, e marca os quadrados que são certos de ocorrer


            std::vector<std::vector<int>> nonograma_init(n_linhas, std::vector<int>(n_colunas, -1));
            pre_processamento(colunas, nonograma_init);

            // Algoritmo MetaHeurístico :D

            // Enquanto não atingir critério de parada:
            //      Construir solução inicial (aplicar heurística)
            //      Realizar busca na vizinhança para atingir um mínimo local
            //      Se for o caso, realizar PathRelinking com as últimas 5 soluções encontradas
            //      Comparar o nonograma encontrado com o melhor até agora encontrado, e mudar caso necessário.

            for(int current_iteration = 0; current_iteration < LIMIT_ITERATIONS; current_iteration++){

                nonograma = nonograma_init;


                // Algoritmo Heurístico :D

                // Enquanto não estiver completo:
                //      Escolher a proxima linha
                //      Fazer 100 construcoes para a linha corretamente sem se preocupar com as colunas
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
                    combinacoes_linhas[i] = combinao;
                    
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


                // Busca na vizinhança
                objetivo_atual = funcao_objetivo(linhas, colunas, nonograma);

                busca_tabu(nonograma, objetivo_atual, linhas, soma_linhas, colunas, soma_colunas, combinacoes_linhas);

                // Salvar o nonograma encontrado
                nonogramas[current_iteration % 5] = nonograma;

                objetivo_atual = funcao_objetivo(linhas, colunas, nonograma);
                objetivo_nonogramas[current_iteration % 5] = objetivo_atual;



                // Se current_iteration % 5 == 4, fazer o path_relinking
                if(SET_PATH_RELINKING){

                    if(current_iteration % 5 == 4){
                        nonograma = path_relinking(nonogramas, objetivo_nonogramas, linhas, soma_linhas, colunas, soma_colunas);
                    }
                }

                // Atualizar o melhor global

                if(objetivo_atual < objetivo_best){
                    nonograma_best = nonograma;
                    objetivo_best = objetivo_atual;
                }

                if(objetivo_best == 0){
                    break;
                }

            }

            auto end = std::chrono::steady_clock::now();
            /// ============================== TIMER TERMINA ================================================
            auto diff( end - start );
            int objetivo_final = funcao_objetivo(linhas, colunas, nonograma_best);

            auto tempo_gasto = std::chrono::duration <double, std::milli> (diff).count();

            media_objetivo += objetivo_final;
            if(tempo_gasto < time_min){
                time_min = tempo_gasto;
            }

            if(tempo_gasto > time_max){
                time_max = tempo_gasto;
            }

            elapsed_time_mean += ( tempo_gasto );

            if(objetivo_final < objetivo_min){
                objetivo_min = objetivo_final;
            }
            if(objetivo_final > objetivo_max){
                objetivo_max = objetivo_final;
            }

            if(run == TEST_REPETITIONS - 1){

                out_file.open (outputs[times]);

                // Print do nonograma resultante
                for(int i{0}; i < n_linhas; ++i){
                    for(int j{0}; j < n_colunas; ++j){
                        if(nonograma_best[i][j] == 1){
                            out_file << "O";
                        }
                        else{
                            out_file << " "; 
                        }
                    }
                    out_file << std::endl;
                }

                
            }

        }


        /// ============================== FIM TESTES TERMINA ================================================

        

        elapsed_time_mean = (double)elapsed_time_mean/(double)TEST_REPETITIONS;
        media_objetivo = (double)media_objetivo/(double)TEST_REPETITIONS;

        out_file << "# Teste de tempo para " << testCases[times] << (SET_PATH_RELINKING? " COM" : " SEM") << " Path Relinking\n";
        out_file << "# Tempo medio : " << std::to_string(elapsed_time_mean) << "ms | Tempo min: " << std::to_string(time_min) << "ms | Tempo max: " << std::to_string(time_max) <<"ms | Média objetivo: " << media_objetivo <<" | Min objetivo: " << objetivo_min <<" | Max objetivo: " << objetivo_max << "\n\n";
    
        out_file.close();
    }

    return 0;
}