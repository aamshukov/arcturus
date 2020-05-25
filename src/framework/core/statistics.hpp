//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#pragma once

BEGIN_NAMESPACE(core)

class statistics : public singleton<statistics>
{
    public:
        using size_type = std::size_t;

    private:
        size_type   my_number_of_lexems;
        size_type   my_number_of_keywords;
        size_type   my_number_of_identifiers;
        size_type   my_number_of_numbers; // ;)
        size_type   my_number_of_strings;
        size_type   my_number_of_arrays;

        size_type   my_number_of_cst_nodes;
        size_type   my_number_of_ast_nodes;

        size_type   my_number_of_quadrotuples;

        size_type   my_number_of_basic_blocks;

    public:
        statistics() = default;

        //?? performance timer
};

END_NAMESPACE

#endif // __STATISTICS_H__
