//..............................
// UI Lab Inc. Arthur Amshukov .
//..............................
#ifndef __IR_SYMBOL_H__
#define __IR_SYMBOL_H__

#pragma once

BEGIN_NAMESPACE(symtable)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

template <typename Token>
class symbol : private noncopyable
{
    public:
        using token_type = Token;
        using symbol_type = std::shared_ptr<symbol<token_type>>;

        using codepoints_type = std::basic_string<datum_type>;

        using index_type = int32_t;
        using size_type = std::size_t;

        using value_type = std::variant<int8_t,
                                        uint8_t,
                                        int16_t,
                                        uint16_t,
                                        int32_t,
                                        uint32_t,
                                        int64_t,
                                        uint64_t,
                                        float,
                                        double,
                                        void*,
                                        datum_type,
                                        codepoints_type>;

        using type_type = std::shared_ptr<type>;

        enum class flagss : uint64_t
        {
            clear       = 0x0000,
            reg         = 0x0001, // if the symbol's value is in a register
            referenced  = 0x0002, // mark variable as referenced
            reachable   = 0x0004  // mark variable as reachable
        };

        DECLARE_ENUM_OPERATORS(flagss)

        using flags_type = flagss;

        using metadata_type = std::unordered_map<string_type, value_type>;

        using counter_type = counter;

    protected:
        codepoints_type         my_name;

        token_type              my_token;               // link with content
        value_type              my_value;               // inffered value if any, might be integer value, real value or
                                                        // identifier (correlated with name)
        type_type               my_type;

        string_type             my_machine_type; //??

        size_type               my_offset;              // runtime offset

        size_type               my_size;                // runtime size in bytes, might be aligned
        size_type               my_bitsize;             // runtime size in bits
        bool                    my_align_required; //??

        //class_type              my_storage_class;     //??

        flags_type              my_flags;               // flags

        metadata_type           my_metadata;            // custom attributes

        std::size_t             my_ssa_id;              // 0 - unassigned, 1+
        static counter_type     my_ssa_counter;

        static counter_type     my_tmp_counter;

    public:
                                symbol();
                               ~symbol();

        const codepoints_type&  name() const;
        codepoints_type&        name();

        const token_type&       token() const;
        token_type&             token();

        const value_type&       value() const;
        value_type&             value();

        const type_type&        type() const;
        type_type&              type();

        std::size_t             ssa_id() const;
        std::size_t&            ssa_id();

        std::size_t             offset() const;
        std::size_t&            offset();

        std::size_t             size() const;
        std::size_t&            size();

        flags_type              flags() const;
        flags_type&             flags();

        const metadata_type&    metadata() const;
        metadata_type&          metadata();

        static symbol_type      get_new_temporary();
};

template <typename Token>
typename symbol<Token>::counter_type symbol<Token>::my_tmp_counter;

template <typename Token>
symbol<Token>::symbol()
                   : my_ssa_id(0),
                     //??my_type(type_type::kind_type::unknown_type),
                     my_offset(0),
                     my_size(0),
                     my_bitsize(0),
                     my_flags(flags_type::clear)
{
}

template <typename Token>
symbol<Token>::~symbol()
{
}

template <typename Token>
inline const typename symbol<Token>::codepoints_type& symbol<Token>::name() const
{
    return my_name;
}

template <typename Token>
inline typename symbol<Token>::codepoints_type& symbol<Token>::name()
{
    return my_name;
}

template <typename Token>
inline const typename symbol<Token>::token_type& symbol<Token>::token() const
{
    return my_token;
}

template <typename Token>
inline typename symbol<Token>::token_type& symbol<Token>::token()
{
    return my_token;
}

template <typename Token>
inline const typename symbol<Token>::value_type& symbol<Token>::value() const
{
    return my_value;
}

template <typename Token>
inline typename symbol<Token>::value_type& symbol<Token>::value()
{
    return my_value;
}

template <typename Token>
inline const typename symbol<Token>::type_type& symbol<Token>::type() const
{
    return my_type;
}

template <typename Token>
inline typename symbol<Token>::type_type& symbol<Token>::type()
{
    return my_type;
}

template <typename Token>
inline std::size_t symbol<Token>::ssa_id() const
{
    return my_ssa_id;
}

template <typename Token>
inline std::size_t& symbol<Token>::ssa_id()
{
    return my_ssa_id;
}

template <typename Token>
inline std::size_t symbol<Token>::offset() const
{
    return my_offset;
}

template <typename Token>
inline std::size_t& symbol<Token>::offset()
{
    return my_offset;
}

template <typename Token>
inline std::size_t symbol<Token>::size() const
{
    return my_size;
}

template <typename Token>
inline std::size_t& symbol<Token>::size()
{
    return my_size;
}

template <typename Token>
inline typename symbol<Token>::flags_type symbol<Token>::flags() const
{
    return my_flags;
}

template <typename Token>
inline typename symbol<Token>::flags_type& symbol<Token>::flags()
{
    return my_flags;
}

template <typename Token>
inline const typename symbol<Token>::metadata_type& symbol<Token>::metadata() const
{
    return my_metadata;
}

template <typename Token>
inline typename symbol<Token>::metadata_type& symbol<Token>::metadata()
{
    return my_metadata;
}

template <typename Token>
typename symbol<Token>::symbol_type symbol<Token>::get_new_temporary()
{
    symbol_type result;

    auto num = my_tmp_counter.value();

    if(num < 999999)
    {
        datum_type t [] = { '0', '0', '0', '0', '0', '0', '~', '$', '_', 'T', 'M', 'P', '_', '$', '~' }; // 15 + 0 = 16

        char b [] = { '0', '0', '0', '0', '0', '0' };

        auto n = array_size(b);

        _itoa_s(num, b, n, 10);

        for(auto k = 0; k < n; k++)
        {
            t[k] = b[k];
        }

        result = factory::create<symbol<token_type>>();

        (*result).name() = t;
    }

    return result;
}

END_NAMESPACE

#endif // __IR_SYMBOL_H__
