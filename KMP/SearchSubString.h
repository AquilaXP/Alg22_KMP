#pragma once

#include "Utilites.h"

// Поиск с использованием "конечного автомата"
template< std::vector< std::vector< size_t > > (*T)( const std::string_view ) >
std::vector< size_t > FAMatcher( const std::string_view text, const std::string_view pattern )
{
    auto delta = T( pattern );
    std::vector< size_t > matches;
    size_t q = 0;
    for( size_t i = 0; i < text.size(); ++i )
    {
        q = delta[q][text[i]];
        if( q == pattern.size() )
        {
            matches.push_back( i - pattern.size() + 1 );
        }
    }

    return matches;
}

// КМП
std::vector< size_t > KMP( const std::string_view text, const std::string_view pattern )
{
    std::vector< size_t > matches;

    auto table = ComputePrefix( pattern );
    size_t q = 0;
    for( size_t i = 0; i < text.size(); ++i )
    {
        while( q > 0 && pattern[q] != text[i] )
        {
            q = table[q - 1];
        }
        if( pattern[q] == text[i] )
        {
            q += 1;
        }
        if( q == pattern.size() )
        {
            matches.emplace_back( i - pattern.size() + 1 );
            q = table[q - 1];
        }
    }

    return matches;
}

// Z - алгоритм
std::vector< size_t > Zalg( const std::string_view text, const std::string_view pattern )
{
    std::string ss = std::string( pattern ) + "@" + std::string( text );
    auto table = ComputePrefix( ss );
    std::vector< size_t > matches;
    for( size_t i = 0; i < table.size(); ++i )
    {
        if( table[i] == pattern.size() )
            matches.push_back( i - pattern.size() * 2 );
    }

    return matches;
}