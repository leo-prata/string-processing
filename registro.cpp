#include "registro.hpp"
using namespace std;

registro::registro(string nome, unsigned ano, unsigned ocorrencias) {
    this->nome = nome;
    this->ano = ano;
    this->ocorrencias = ocorrencias;

    _posicao = 0;
}

registro::~registro() {}

string registro::get_nome() {
    return nome;
}

unsigned registro::get_ano() {
    return ano;
}

unsigned registro::get_ocorrencias(){
    return ocorrencias;
}

size_t registro::get_posicao(){
    return _posicao;
}

void registro::set_posicao(size_t posicao) {
    _posicao = posicao;
}

void registro::set_ocorrencias(unsigned ocorrencias){
    this->ocorrencias = ocorrencias;
}

void registro::imprime(){
    cout << "Nome: " << nome << ", Ano: " << ano
         << ", Ocorrencias: " << ocorrencias
         << ", Posicao: " << _posicao << endl;
}