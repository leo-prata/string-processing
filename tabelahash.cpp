#include "tabelahash.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <unordered_map>

using namespace std;

tabelahash::tabelahash(size_t tamanho)
{
    this->tamanho = tamanho;

    tabela = new registro *[tamanho];
    for (size_t i = 0; i < tamanho; ++i)
    {
        tabela[i] = nullptr;
    }
}

tabelahash::~tabelahash()
{
    for (size_t i = 0; i < tamanho; ++i)
    {
        if (tabela[i] != nullptr)
        {
            delete tabela[i];
        }
    }
    delete[] tabela;
}

// Função Rabin-Karp
int *tabelahash::rabin_karp(const std::string &text, const std::string &pattern, int &count)
{
    count = 0;
    int n = text.length();
    int m = pattern.length();

    if (m > n)
        return nullptr;

    const int base = 256;
    const int prime = 101;

    long long h = 1;
    for (int i = 0; i < m - 1; ++i)
    {
        h = (h * base) % prime;
    }

    long long pattern_hash = 0;
    long long window_hash = 0;
    for (int i = 0; i < m; ++i)
    {
        pattern_hash = (base * pattern_hash + pattern[i]) % prime;
        window_hash = (base * window_hash + text[i]) % prime;
    }

    int *temp_positions = new int[n];
    int temp_count = 0;

    for (int i = 0; i <= n - m; ++i)
    {
        if (pattern_hash == window_hash)
        {
            bool match = true;
            for (int j = 0; j < m; ++j)
            {
                if (text[i + j] != pattern[j])
                {
                    match = false;
                    break;
                }
            }
            if (match)
            {
                temp_positions[temp_count++] = i;
            }
        }
        if (i < n - m)
        {
            window_hash = (base * (window_hash - text[i] * h) + text[i + m]) % prime;
            if (window_hash < 0)
                window_hash += prime;
        }
    }

    int *positions = new int[temp_count];
    memcpy(positions, temp_positions, temp_count * sizeof(int));
    delete[] temp_positions;
    count = temp_count;
    return positions;
}

registro **tabelahash::busca_rabin_karp(const std::string &pattern, int &count)
{
    count = 0;
    registro **temp_resultados = new registro *[tamanho];

    for (size_t i = 0; i < tamanho; ++i)
    {
        if (tabela[i] != nullptr)
        {
            int pos_count;
            int *pos = rabin_karp(tabela[i]->get_nome(), pattern, pos_count);
            if (pos_count > 0)
            {
                temp_resultados[count++] = tabela[i];
                delete[] pos;
            }
        }
    }

    registro **resultados = new registro *[count];
    std::memcpy(resultados, temp_resultados, count * sizeof(registro *));
    delete[] temp_resultados;
    return resultados;
}

// Função Boyer-Moore
int tabelahash::boyer_moore(const string &text, const string &pattern)
{
    // Tabela de caracteres ruins
    unordered_map<char, int> bad_char;
    int pattern_len = pattern.length();
    int text_len = text.length();

    // Pré-processamento: preenche a tabela de caracteres ruins
    for (int i = 0; i < pattern_len; i++)
    {
        bad_char[pattern[i]] = i;
    }

    int shift = 0; // Deslocamento do padrão em relação ao texto
    while (shift <= (text_len - pattern_len))
    {
        int j = pattern_len - 1;

        // Compara o padrão com o texto da direita para a esquerda
        while (j >= 0 && pattern[j] == text[shift + j])
        {
            j--;
        }

        // Se o padrão foi encontrado
        if (j < 0)
        {
            return shift; // Retorna a posição onde o padrão foi encontrado
        }
        else
        {
            // Calcula o deslocamento com base na heurística do caractere ruim
            char bad_char_in_text = text[shift + j];
            int bad_char_shift = bad_char.find(bad_char_in_text) != bad_char.end() ? max(1, j - bad_char[bad_char_in_text]) : j + 1;
            shift += bad_char_shift;
        }
    }

    return -1; // Padrão não encontrado
}

void tabelahash::imprime_tabela()
{
    for (size_t i = 0; i < tamanho; ++i)
    {

        if (tabela[i] != nullptr)
        {
            cout << i << ": ";
            tabela[i]->imprime();
        }
        else
            cout << i << ": NULL" << endl;
    }
}

void tabelahash::imprime_top_x(size_t x)
{
    if (x < 0 || x > this->tamanho)
    {
        cout << "Valor de x invalido" << endl;
        return;
    }

    registro **tabelaAux = new registro *[this->tamanho];
    for (size_t i = 0; i < this->tamanho; i++)
    {
        tabelaAux[i] = this->tabela[i];
    }

    for (size_t i = 0; i < x; i++)
    {
        size_t mais_ocorrencias = -1;

        for (size_t j = 0; j < this->tamanho; j++)
        {
            if (tabelaAux[j] != nullptr && (mais_ocorrencias == -1 || tabelaAux[j]->get_ocorrencias() > tabelaAux[mais_ocorrencias]->get_ocorrencias()))
            {
                mais_ocorrencias = j;
            }
        }

        if (mais_ocorrencias != -1 && tabelaAux[mais_ocorrencias] != nullptr)
        {
            tabelaAux[mais_ocorrencias]->imprime();
            tabelaAux[mais_ocorrencias]->set_ocorrencias(0);
        }
    }

    delete[] tabelaAux;
}

size_t tabelahash::hash_function(const string &chave)
{
    unsigned long long hashValue = 0;
    unsigned long long base = 31;
    unsigned long long mod = 1e9 + 7;

    for (char c : chave)
    {
        hashValue = (hashValue * base + static_cast<unsigned long long>(c)) % mod;
    }
    return hashValue % tamanho;
}

size_t tabelahash::hash_function_2(const string &chave)
{
    unsigned long long hashValue = 0;
    unsigned long long base = 37;
    unsigned long long mod = 1e9 + 9;

    for (char c : chave)
    {
        hashValue = (hashValue * base + static_cast<unsigned long long>(c)) % mod;
    }
    return (hashValue % (tamanho - 1)) + 1;
}

void tabelahash::insere(string &nome, unsigned ano, unsigned ocorrencias)
{
    size_t hash1 = hash_function(nome);
    size_t hash2 = hash_function_2(nome);
    size_t pos = hash1;

    for (size_t i = 0; i < tamanho; ++i)
    {
        if (tabela[pos] == nullptr)
        {
            tabela[pos] = new registro(nome, ano, ocorrencias);
            tabela[pos]->set_posicao(pos);
            return;
        }
        pos = (hash1 + i * hash2) % tamanho;
    }
    cout << "Erro: tabela cheia, nao foi possivel inserir \"" << nome << "\"." << endl;
}

registro *tabelahash::acha_registro_hash(string nome, unsigned ano, unsigned ocorrencias)
{
    size_t hash1 = hash_function(nome);
    size_t hash2 = hash_function_2(nome);
    size_t pos = hash1;

    for (size_t i = 0; i < tamanho; ++i)
    {
        if (tabela[pos] == nullptr)
        {
            return nullptr;
        }

        if (tabela[pos]->get_nome() == nome &&
            tabela[pos]->get_ano() == ano &&
            tabela[pos]->get_ocorrencias() == ocorrencias)
        {
            return tabela[pos];
        }
        pos = (hash1 + i * hash2) % tamanho;
    }
    return nullptr;
}

registro *tabelahash::acha_registro_boyer(const string &nome, unsigned ano, unsigned ocorrencias)
{
    size_t hash1 = hash_function(nome);
    size_t hash2 = hash_function_2(nome);
    size_t pos = hash1;

    for (size_t i = 0; i < tamanho; ++i)
    {
        if (tabela[pos] == nullptr)
        {
            return nullptr;
        }

        // Usa Boyer-Moore para buscar o padrão no nome do registro
        if (boyer_moore(tabela[pos]->get_nome(), nome) != -1 &&
            tabela[pos]->get_ano() == ano &&
            tabela[pos]->get_ocorrencias() == ocorrencias)
        {
            return tabela[pos];
        }
        pos = (hash1 + i * hash2) % tamanho;
    }
    return nullptr;
}

registro *tabelahash::pega_registro(string nome, unsigned ano, unsigned ocorrencias)
{
    size_t hash1 = hash_function(nome);
    size_t hash2 = hash_function_2(nome);
    size_t pos = hash1;

    for (size_t i = 0; i < tamanho; ++i)
    {
        if (tabela[pos] == nullptr)
        {
            return nullptr;
        }

        if (tabela[pos]->get_nome() == nome &&
            tabela[pos]->get_ano() == ano &&
            tabela[pos]->get_ocorrencias() == ocorrencias)
        {
            registro *encontrado = tabela[pos];
            tabela[pos] = nullptr;
            return encontrado;
        }
        pos = (hash1 + i * hash2) % tamanho;
    }
    return nullptr;
}
