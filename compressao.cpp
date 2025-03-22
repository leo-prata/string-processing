#include "compressao.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

string compressao::comprimir_rle(const string &s)
{
    string resultado = "";
    int n = s.length();
    int i = 0;

    while (i < n)
    {
        char c = s[i];
        int count = 1;
        i++;
        while (i < n && s[i] == c)
        {
            count++;
            i++;
        }
        resultado += c;
        resultado += to_string(count);
    }
    return resultado;
}

void compressao::comprimir_arquivo_rle(const string &arquivo_original, const string &arquivo_comprimido, size_t limite)
{
    ifstream in(arquivo_original);
    ofstream out(arquivo_comprimido);
    if (!in.is_open() || !out.is_open())
    {
        cerr << "Erro ao abrir arquivos" << endl;
        return;
    }

    string linha;
    size_t count = 0;
    size_t tamanho_original = 0;
    size_t tamanho_comprimido = 0;

    while (getline(in, linha))
    {
        if (limite > 0 && count >= limite)
            break;
        string comprimida = comprimir_rle(linha);
        out << comprimida << endl;
        tamanho_original += linha.length();
        tamanho_comprimido += comprimida.length();
        count++;
    }

    // aqui eu ajusto para incluir os caracteres de nova linha ('\n')
    tamanho_original += count;
    tamanho_comprimido += count;

    in.close();
    out.close();

    double taxa_compressao = static_cast<double>(tamanho_original) / tamanho_comprimido;
    cout << "Tamanho original: " << tamanho_original << " bytes" << endl;
    cout << "Tamanho comprimido: " << tamanho_comprimido << " bytes" << endl;
    cout << "Taxa de compressao: " << taxa_compressao << endl;
}

string compressao::comprimir_lz78(const string &s)
{ // Lempel-Ziv 78
    string resultado = "";
    unordered_map<string, int> dicionario;
    int proximo_codigo = 1;
    string atual = "";

    for (char c : s)
    {
        string proxima = atual + c;
        if (dicionario.find(proxima) != dicionario.end())
        {
            atual = proxima;
        }
        else
        {
            if (atual.empty())
            {
                resultado += "0," + string(1, c) + ";";
            }
            else
            {
                resultado += to_string(dicionario[atual]) + "," + string(1, c) + ";";
            }
            dicionario[proxima] = proximo_codigo++;
            atual = "";
        }
    }

    // Processa o último caractere, se houver
    if (!atual.empty())
    {
        resultado += to_string(dicionario[atual]) + ";";
    }

    return resultado;
}

void compressao::comprimir_arquivo_lz78(const string &arquivo_original, const string &arquivo_comprimido, size_t limite)
{
    ifstream in(arquivo_original);
    ofstream out(arquivo_comprimido);
    if (!in.is_open() || !out.is_open())
    {
        cerr << "Erro ao abrir arquivos" << endl;
        return;
    }

    string linha;
    size_t count = 0;
    size_t tamanho_original = 0;
    size_t tamanho_comprimido = 0;

    while (getline(in, linha))
    {
        if (limite > 0 && count >= limite)
            break;
        string comprimida = comprimir_lz78(linha);
        out << comprimida << endl;
        tamanho_original += linha.length();
        tamanho_comprimido += comprimida.length();
        count++;
    }

    // Ajusta para incluir os caracteres de nova linha ('\n')
    tamanho_original += count;
    tamanho_comprimido += count;

    in.close();
    out.close();

    double taxa_compressao = static_cast<double>(tamanho_original) / tamanho_comprimido;
    cout << "Tamanho original: " << tamanho_original << " bytes" << endl;
    cout << "Tamanho comprimido: " << tamanho_comprimido << " bytes" << endl;
    cout << "Taxa de compressao: " << taxa_compressao << endl;
}