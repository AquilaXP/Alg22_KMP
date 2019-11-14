#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <tuple>

#include "SearchSubString.h"

// Тестирование производительности
class Testing
{
public:
    void Load( const std::string& path )
    {
        m_cases.clear();

        std::ifstream istr( path );
        if( !istr.is_open() )
            throw std::runtime_error( "not open file " + path );

        std::string line;
        std::getline( istr, line );
        while( std::getline( istr, line ) && !line.empty() )
        {
            const char sep = '\t';
            auto endText = line.find( sep );
            if( endText == std::string::npos )
                throw std::runtime_error( "incorect file, have not column text" );
            std::string text = line.substr( 0, endText );

            auto endPattern = line.find( sep, endText + 1 );
            if( endPattern == std::string::npos )
                throw std::runtime_error( "incorect file, have not column pattern" );
            std::string pattern = line.substr( endText + 1, endPattern - endText - 1 );

            std::string matches = line.substr( endPattern );
            std::istringstream ss( matches );
            std::vector< size_t > numbers;
            size_t n = 0;
            while( ss >> n )
            {
                numbers.emplace_back( n );
            }
            m_cases.emplace_back( std::make_tuple( std::move( text ), std::move( pattern ), std::move( numbers ) ) );
        }
    }

    template< class Func >
    void Execute( Func func, const std::string& name )
    {
        using namespace std;
        cout << name << '-';

        auto b = chrono::high_resolution_clock::now();
        size_t i = 0;
        for( auto&[text, pattern, etalon] : m_cases )
        {
            ++i;
            vector< size_t > res = func( text, pattern );
            if( res != etalon )
            {
                cout << "Fail\n";
                return;
            }
        }
        auto e = chrono::high_resolution_clock::now();
        cout << "Ok" << endl;
        cout << "time = " << chrono::duration_cast<chrono::milliseconds>( e - b ).count() << '\n';
    }
private:
    std::vector< std::tuple< std::string, std::string, std::vector< size_t > > > m_cases;
};

int main()
{
    Testing test;
    test.Load( "string_matching_test_cases_31272_751472-57251-751472.tsv" );
    test.Execute( FAMatcher<ComputeDelta>, "FAMatcherM3" );
    test.Execute( FAMatcher<ComputeDeltaFast>, "FAMatcherFast" );
    test.Execute( KMP, "KMP" );
    test.Execute( Zalg, "Zalg" );

    return 0;
}


