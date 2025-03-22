#include "tabelahash.hpp"
#include "compressao.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

using namespace std;

struct Resultado
{
    string metodo;
    double tempo;
};

void salvar_resultados_csv(const string &nome_arquivo, const vector<Resultado> &resultados)
{
    ofstream arquivo(nome_arquivo);
    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir o arquivo: " << nome_arquivo << endl;
        return;
    }

    arquivo << "Metodo,Tempo(segundos)\n";

    for (const auto &resultado : resultados)
    {
        arquivo << resultado.metodo << "," << resultado.tempo << "\n";
    }

    arquivo.close();
    cout << "Resultados salvos em " << nome_arquivo << endl;
}

void carregar_csv(tabelahash &tabela, const string &arquivo_csv, size_t limite)
{
    ifstream file(arquivo_csv);
    if (!file.is_open())
    {
        cerr << "Erro ao abrir o arquivo: " << arquivo_csv << endl;
        return;
    }

    string linha;
    getline(file, linha); // Ignora o cabeçalho
    size_t count_inseridos = 0;

    while (getline(file, linha) && count_inseridos < limite)
    {
        stringstream ss(linha);
        string id, name, year, gender, count;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, year, ',');
        getline(ss, gender, ',');
        getline(ss, count, ',');

        tabela.insere(name, stoi(year), stoi(count));
        count_inseridos++;

        if (count_inseridos >= limite)
        {
            cout << "Limite de inserções atingido: " << limite << endl;
            break;
        }
    }

    file.close();
}

void buscar_com_rabin_karp(tabelahash &tabela)
{
    string padrao;
    cout << "Digite o padrão para busca (Rabin-Karp): ";
    cin.ignore();
    getline(cin, padrao);

    int count;
    registro **resultados = tabela.busca_rabin_karp(padrao, count);
    cout << "Encontrados: " << count << " registros\n";
    for (int i = 0; i < count; ++i)
    {
        resultados[i]->imprime();
    }
    delete[] resultados;
}

void buscar_com_boyer(tabelahash &tabela, const string &nome, unsigned ano, unsigned ocorrencias)
{
    registro *encontrado = tabela.acha_registro_boyer(nome, ano, ocorrencias);
    if (encontrado != nullptr)
    {
        cout << "Registro encontrado: ";
        encontrado->imprime();
    }
    else
    {
        cout << "Registro nao encontrado!" << endl;
    }
}

void buscar_com_hash(tabelahash &tabela, const string &nome, unsigned ano, unsigned ocorrencias)
{
    registro *encontrado = tabela.acha_registro_hash(nome, ano, ocorrencias);
    if (encontrado != nullptr)
    {
        cout << "Registro encontrado: ";
        encontrado->imprime();
    }
    else
    {
        cout << "Registro nao encontrado!" << endl;
    }
}

void exibir_menu()
{
    cout << "\n--- Menu de Busca ---\n";
    cout << "1. Buscar usando Boyer-Moore\n";
    cout << "2. Buscar usando Hash Normal\n";
    cout << "3. Imprimir Tabela Hash\n";
    cout << "4. Imprimir Top 10 Registros\n";
    cout << "5. Buscar usando Rabin-Karp\n";
    cout << "0. Sair\n";
    cout << "Escolha uma opção: ";
}

void executar_testes(tabelahash &tabela, compressao &compressao, const string &arquivo_csv, size_t limite)
{
    vector<Resultado> resultados;

    //! TESTE DO RABIN_KARP
    auto start = chrono::high_resolution_clock::now();

    // Teste a) Busca por padrão no início do arquivo com Rabin-Karp
    auto start_a = chrono::high_resolution_clock::now();

    string padrao_a = "Mar"; // Parte de "Mary", comum no início
    cout << "\nTeste a) Buscando por padrão '" << padrao_a << "' (Rabin-Karp):\n";
    int count_a;
    registro **resultados_a = tabela.busca_rabin_karp(padrao_a, count_a);
    cout << "Encontrados: " << count_a << " registros\n";
    for (int i = 0; i < count_a && i < 5; ++i)
    {
        resultados_a[i]->imprime();
    }
    if (count_a > 5)
        cout << "... e mais " << (count_a - 5) << " registros\n";
    delete[] resultados_a;
    auto end_a = chrono::high_resolution_clock::now();

    // Teste b) Busca por padrão no final dos 100 registros com Rabin-Karp
    auto start_b = chrono::high_resolution_clock::now();

    string padrao_b = "Zyr"; // Parte de "Zyrin", que aparece nos 100 primeiros
    cout << "\nTeste b) Buscando por padrão '" << padrao_b << "' (Rabin-Karp):\n";
    int count_b;
    registro **resultados_b = tabela.busca_rabin_karp(padrao_b, count_b);
    cout << "Encontrados: " << count_b << " registros\n";
    for (int i = 0; i < count_b && i < 5; ++i)
    {
        resultados_b[i]->imprime();
    }
    if (count_b > 5)
        cout << "... e mais " << (count_b - 5) << " registros\n";
    delete[] resultados_b;
    auto end_b = chrono::high_resolution_clock::now();

    // Teste c) Busca por padrão que não existe com Rabin-Karp
    auto start_c = chrono::high_resolution_clock::now();

    string padrao_c = "XYZ";
    cout << "\nTeste c) Buscando por padrão '" << padrao_c << "' (Rabin-Karp):\n";
    int count_c;
    registro **resultados_c = tabela.busca_rabin_karp(padrao_c, count_c);
    cout << "Encontrados: " << count_c << " registros\n";
    for (int i = 0; i < count_c; ++i)
    {
        resultados_c[i]->imprime();
    }
    delete[] resultados_c;
    auto end_c = chrono::high_resolution_clock::now();

    // Medir o tempo de execução
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> tempo_rabin = end - start;
    cout << "Tempo de execução Rabin Karp: " << tempo_rabin.count() << " segundos\n";

    chrono::duration<double> tempo_rabin_a = end_a - start_a;
    chrono::duration<double> tempo_rabin_b = end_b - start_b;
    chrono::duration<double> tempo_rabin_c = end_c - start_c;

    //! TESTE DO BOYER-MOORE
    start = chrono::high_resolution_clock::now();

    // Teste a) Busca por nome que está no início do arquivo
    start_a = chrono::high_resolution_clock::now();

    string nome_busca_a = "Mary";
    unsigned ano_busca_a = 1880;
    unsigned ocorrencias_busca_a = 7065;
    cout << "\nTeste a) Buscando por '" << nome_busca_a << "' (Boyer-Moore):\n";
    buscar_com_boyer(tabela, nome_busca_a, ano_busca_a, ocorrencias_busca_a);
    end_a = chrono::high_resolution_clock::now();

    // Teste b) Busca por nome que está no final do arquivo
    start_b = chrono::high_resolution_clock::now();

    string nome_busca_b = "Zyrin";
    unsigned ano_busca_b = 1880;
    unsigned ocorrencias_busca_b = 1414;
    cout << "\nTeste b) Buscando por '" << nome_busca_b << "' (Boyer-Moore):\n";
    buscar_com_boyer(tabela, nome_busca_b, ano_busca_b, ocorrencias_busca_b);
    end_b = chrono::high_resolution_clock::now();

    // Teste c) Busca por um trecho que não está no arquivo
    start_c = chrono::high_resolution_clock::now();

    string nome_busca_c = "João";
    unsigned ano_busca_c = 1880;
    unsigned ocorrencias_busca_c = 1000;
    cout << "\nTeste c) Buscando por '" << nome_busca_c << "' (Boyer-Moore):\n";
    buscar_com_boyer(tabela, nome_busca_c, ano_busca_c, ocorrencias_busca_c);
    end_c = chrono::high_resolution_clock::now();

    // Medir o tempo de execução
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> tempo_BM = end - start;
    cout << "Tempo de execução Boyer Moore: " << tempo_BM.count() << " segundos\n";

    chrono::duration<double> tempo_BM_a = end_a - start_a;
    chrono::duration<double> tempo_BM_b = end_b - start_b;
    chrono::duration<double> tempo_BM_c = end_c - start_c;

    //! Teste de Compressão RLE
    start = chrono::high_resolution_clock::now();

    cout << "\nTeste de Compressao RLE:\n";
    compressao.comprimir_arquivo_rle(arquivo_csv, "NationalNames_comprimido_rle.csv", limite);

    // Medir o tempo de execução
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> tempo_rle = end - start;
    cout << "Tempo de execução Compressao RLE: " << tempo_rle.count() << " segundos\n";

    //! Teste de Compressão LZ78
    start = chrono::high_resolution_clock::now();

    cout << "\nTeste de Compressao LZ78:\n";
    compressao.comprimir_arquivo_lz78(arquivo_csv, "NationalNames_comprimido_lz78.csv", limite);

    // Medir o tempo de execução
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> tempo_lz78 = end - start;
    cout << "Tempo de execução Compressao LZ78: " << tempo_lz78.count() << " segundos\n";

    // Salvar os resultados em um arquivo CSV
    resultados.push_back({"Rabin-Karp Geral", tempo_rabin.count()});
    resultados.push_back({"Rabin-Karp A", tempo_rabin_a.count()});
    resultados.push_back({"Rabin-Karp B", tempo_rabin_b.count()});
    resultados.push_back({"Rabin-Karp C", tempo_rabin_c.count()});

    resultados.push_back({"Boyer-Moore Geral", tempo_BM.count()});
    resultados.push_back({"Boyer-Moore A", tempo_BM_a.count()});
    resultados.push_back({"Boyer-Moore B", tempo_BM_b.count()});
    resultados.push_back({"Boyer-Moore C", tempo_BM_c.count()});

    resultados.push_back({"Compressao RLE", tempo_rle.count()});
    resultados.push_back({"Compressao LZ78", tempo_lz78.count()});

    salvar_resultados_csv("resultados.csv", resultados);
}

int main()
{
    tabelahash tabela(100);
    compressao compressao; // Instância da classe Compressao
    string arquivo_csv = "NationalNames.csv";
    size_t limite = 100; // Limitar a inserção a 100 registros

    // Carrega os dados do CSV para a tabela hash
    carregar_csv(tabela, arquivo_csv, limite);

    // Executa os testes automáticos
    executar_testes(tabela, compressao, arquivo_csv, limite);

    // Menu interativo
    int opcao;
    do
    {
        exibir_menu();
        cin >> opcao;

        switch (opcao)
        {
        case 1:
        {
            string nome;
            unsigned ano, ocorrencias;
            cout << "Digite o nome para busca (Boyer-Moore): ";
            cin >> nome;
            cout << "Digite o ano: ";
            cin >> ano;
            cout << "Digite o número de ocorrências: ";
            cin >> ocorrencias;
            buscar_com_boyer(tabela, nome, ano, ocorrencias);
            break;
        }
        case 2:
        {
            string nome;
            unsigned ano, ocorrencias;
            cout << "Digite o nome para busca (Hash Normal): ";
            cin >> nome;
            cout << "Digite o ano: ";
            cin >> ano;
            cout << "Digite o número de ocorrências: ";
            cin >> ocorrencias;
            buscar_com_hash(tabela, nome, ano, ocorrencias);
            break;
        }
        case 3:
            tabela.imprime_tabela();
            break;
        case 4:
            tabela.imprime_top_x(10);
            break;
        case 5:
            buscar_com_rabin_karp(tabela);
            break;
        case 0:
            cout << "Saindo...\n";
            break;
        default:
            cout << "Opção inválida! Tente novamente.\n";
            break;
        }
    } while (opcao != 0);

    return 0;
}