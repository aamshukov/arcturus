//........................................................
//  2019-2021 Underground Intelligence (UI) Lab Inc.     .
//  Arthur Amshukov, aamshukov@gmail.com                 .
//........................................................
#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#pragma once

BEGIN_NAMESPACE(core)

class statistics : public singleton<statistics>
{
    private:
        std::size_t my_number_of_lexems;
        std::size_t my_number_of_keywords;
        std::size_t my_number_of_identifiers;
        std::size_t my_number_of_numbers; // ;)
        std::size_t my_number_of_strings;
        std::size_t my_number_of_arrays;

        std::size_t my_number_of_functions;

        std::size_t my_number_of_cst_nodes;
        std::size_t my_number_of_ast_nodes;

        std::size_t my_number_of_quadruples;

        std::size_t my_number_of_basic_blocks;

    public:
                        statistics() = default;

        //?? performance timer

        std::size_t     number_of_lexems() const;
        std::size_t&    number_of_lexems();
};

inline std::size_t statistics::number_of_lexems() const
{
    return my_number_of_lexems;
}

inline std::size_t& statistics::number_of_lexems()
{
    return my_number_of_lexems;
}

END_NAMESPACE

#endif // __STATISTICS_H__
