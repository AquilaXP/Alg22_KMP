#pragma once

#include <string>
#include <vector>
#include <string_view>

// Строит "конечный автомат" O( m^3 ), m = pattern.size()
std::vector< std::vector< size_t > > ComputeDelta( const std::string_view pattern );

// Строит "конечный автомат" O( m * sizeAlphabet ), m = pattern.size()
std::vector< std::vector< size_t > > ComputeDeltaFast( const std::string_view pattern );

// Расчет префиксов совпадающих с суффиксом
std::vector< size_t > ComputePrefix( const std::string_view pattern );