#ifndef REGISTRO_HPP
#define REGISTRO_HPP

#include <string>
#include <iostream>

using namespace std;

class registro {
private:
    string nome;
    unsigned ano;
    unsigned ocorrencias;
    size_t _posicao;

public:
    registro(string nome, unsigned ano, unsigned ocorrencias);
    ~registro();

    void set_posicao(size_t posicao);
    void set_ocorrencias(unsigned ocorrencias);
    
    string get_nome();
    unsigned get_ano();
    unsigned get_ocorrencias();
    size_t get_posicao();
    void imprime();
};

#endif /* REGISTRO_HPP */