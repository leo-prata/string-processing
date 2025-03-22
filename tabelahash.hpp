#ifndef TABELA_HASH_HPP
#define TABELA_HASH_HPP

#include "registro.hpp"
#include <vector>

using namespace std;

class tabelahash
{
private:
    size_t tamanho;
    registro **tabela;

    size_t hash_function(const string &chave);
    size_t hash_function_2(const string &chave);

    int *rabin_karp(const std::string &text, const std::string &pattern, int &count);

public:
    tabelahash(size_t tamanho);
    ~tabelahash();

    void imprime_tabela();
    void imprime_top_x(size_t x);

    void insere(string &nome, unsigned ano, unsigned ocorrencias);

    int boyer_moore(const string &text, const string &pattern);

    registro *pega_registro(string nome, unsigned ano, unsigned ocorrencias);
    registro *acha_registro_hash(string nome, unsigned ano, unsigned ocorrencias);
    registro *acha_registro_boyer(const string &nome, unsigned ano, unsigned ocorrencias);

    registro **busca_rabin_karp(const std::string &pattern, int &count);
};

#endif /* TABELA_HASH_HPP */