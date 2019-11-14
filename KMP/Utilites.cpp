#include "Utilites.h"



std::vector< std::vector< size_t > > ComputeDelta( const std::string_view pattern )
{
    // Размер алфавита
    const size_t sizeAlphabet = { 256 };
    std::vector< std::vector< size_t > > delta;
    delta.resize( pattern.size() + 1, std::vector< size_t >( sizeAlphabet, 0 ) );

    for( size_t q = 0; q <= pattern.size(); ++q )
    {
        // Проходим по алфавиту
        for( size_t c = 0; c < sizeAlphabet; ++c )
        {
            size_t k = q + 1;
            if( q == pattern.size() || c != pattern[q] )
            {
                k -= 1;
                while( k > 0 )
                {
                    k -= 1;
                    // конец prefix не совпал с концом suffix, нет смысла и остальное проверять
                    if( pattern[k] != c )
                        continue;

                    std::string_view prefix( pattern.data(), k );
                    std::string_view suffix( pattern.data() + q - k, k );
                    if( prefix == suffix )
                    {
                        k += 1;
                        break;
                    }
                }
            }

            delta[q][c] = k;
        }
    }

    return delta;
}

std::vector< std::vector< size_t > > ComputeDeltaFast( const std::string_view pattern )
{
    // Получаем таблицу префиксов
    auto table = ComputePrefix( pattern );

    // Размер алфавита
    const size_t sizeAlphabet = { 256 };
    std::vector< std::vector< size_t > > delta;
    delta.resize( pattern.size() + 1, std::vector< size_t >( sizeAlphabet, 0 ) );

    for( size_t q = 0; q <= pattern.size(); ++q )
    {
        for( size_t c = 0; c < sizeAlphabet; ++c )
        {
            size_t k = q + 1;
            if( q == pattern.size() || c != pattern[q] )
            {
                k -= 1;
                if( k > 0 )
                {
                    // выполняем поиск как в KMP
                    size_t maxPrefix = table[k - 1];
                    while( maxPrefix > 0 && pattern[maxPrefix] != c )
                    {
                        maxPrefix = table[maxPrefix - 1];
                    }
                    if( pattern[maxPrefix] == c )
                    {
                        k = maxPrefix + 1;
                    }
                    else
                    {
                        k = 0;
                    }
                }
            }

            delta[q][c] = k;
        }
    }

    return delta;
}

std::vector< size_t > ComputePrefix( const std::string_view pattern )
{
    std::vector< size_t > table( pattern.size(), 0 );
    size_t k = 0;
    for( size_t q = 1; q < pattern.size(); ++q )
    {
        while( k > 0 && pattern[k] != pattern[q] )
        {
            k = table[k - 1];
        }
        if( pattern[k] == pattern[q] )
            k += 1;
        table[q] = k;
    }
    return table;
}
