#ifndef COMPRESSAO_HPP
#define COMPRESSAO_HPP

#include <string>

class compressao
{
public:
    std::string comprimir_rle(const std::string &s);
    void comprimir_arquivo_rle(const std::string &arquivo_original, const std::string &arquivo_comprimido, size_t limite = 0);

    std::string comprimir_lz78(const std::string &s);
    void comprimir_arquivo_lz78(const std::string &arquivo_original, const std::string &arquivo_comprimido, size_t limite = 0);
};

#endif // COMPRESSAO_HPP