//........................................................
// Underground Intelligence (UI) Lab Inc. Arthur Amshukov.
//........................................................
#ifndef __IR_SYMBOL_H__
#define __IR_SYMBOL_H__

#pragma once

BEGIN_NAMESPACE(symtable)

USINGNAMESPACE(core)
USINGNAMESPACE(frontend)

class symbol : private noncopyable
{
    public:
        using token_type = token<token_traits>;
        using symbol_type = std::shared_ptr<symbol>;

        using id_type = std::size_t;

        using index_type = int32_t;

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

        enum class flag : uint64_t
        {
            clear       = 0x0000,
            reg         = 0x0001, // if the symbol's value is in a register
            referenced  = 0x0002, // mark variable as referenced
            reachable   = 0x0004, // mark variable as reachable
            constant    = 0x0008, // symbol represents 'const' value
            readonly    = 0x0010  // symbol represents 'readonly' value
        };

        DECLARE_ENUM_OPERATORS(flag)

        using flags_type = flag;

        using metadata_type = std::unordered_map<string_type, value_type>;

        using counter_type = counter;

    protected:
        id_type                 my_id;
        codepoints_type         my_name;

        token_type              my_token;               // link with content
        value_type              my_value;               // inffered value if any, might be integer value, real value or
                                                        // identifier (correlated with name)
        type_type               my_type;

        string_type             my_machine_type; //??

        std::size_t             my_offset;              // runtime offset

        std::size_t             my_size;                // runtime size in bytes, might be aligned
        std::size_t             my_bitsize;             // runtime size in bits
        bool                    my_align_required; //??

        //class_type              my_storage_class;     //??

        flags_type              my_flags;               // flags

        metadata_type           my_metadata;            // custom attributes

        static counter_type     my_tmp_counter;

    public:
                                symbol(const id_type& id);
                               ~symbol();

        const id_type&          id() const;
        id_type&                id();

        const codepoints_type&  name() const;
        codepoints_type&        name();

        const token_type&       token() const;
        token_type&             token();

        const value_type&       value() const;
        value_type&             value();

        const type_type&        type() const;
        type_type&              type();

        //std::size_t             ssa_id() const;
        //std::size_t&            ssa_id();

        std::size_t             offset() const;
        std::size_t&            offset();

        std::size_t             size() const;
        std::size_t&            size();

        flags_type              flags() const;
        flags_type&             flags();

        const metadata_type&    metadata() const;
        metadata_type&          metadata();

        static symbol_type      get_new_temporary();

        string_type             to_string() const;
};

inline typename symbol::counter_type symbol::my_tmp_counter;

struct symbol_lt_key_comparator
{
    using symbol_type = std::shared_ptr<symbol>;
    bool operator() (const symbol_type& lhs, const symbol_type& rhs) const
    {
        return (*lhs).id() < (*rhs).id();
    }
};

struct symbol_eq_key_comparator
{
    using symbol_type = std::shared_ptr<symbol>;
    bool operator() (const symbol_type& lhs, const symbol_type& rhs) const
    {
        return (*lhs).id() == (*rhs).id();
    }
};

struct symbol_hash
{
    using symbol_type = std::shared_ptr<symbol>;
    std::size_t operator () (const symbol_type& symbol) const
    {
        std::size_t result = (*symbol).id();
        result ^= combine_hash(result);
        return result;
    }
};

inline symbol::symbol(const id_type& id)
             : my_id(id),
               my_offset(0),
               my_size(0),
               my_bitsize(0),
               my_flags(flags_type::clear)
{
}

inline symbol::~symbol()
{
}

inline const typename symbol::id_type& symbol::id() const
{
    return my_id;
}

inline typename symbol::id_type& symbol::id()
{
    return my_id;
}

inline const typename codepoints_type& symbol::name() const
{
    return my_name;
}

inline typename codepoints_type& symbol::name()
{
    return my_name;
}

inline const typename symbol::token_type& symbol::token() const
{
    return my_token;
}

inline typename symbol::token_type& symbol::token()
{
    return my_token;
}

inline const typename symbol::value_type& symbol::value() const
{
    return my_value;
}

inline typename symbol::value_type& symbol::value()
{
    return my_value;
}

inline const typename symbol::type_type& symbol::type() const
{
    return my_type;
}

inline typename symbol::type_type& symbol::type()
{
    return my_type;
}

inline std::size_t symbol::offset() const
{
    return my_offset;
}

inline std::size_t& symbol::offset()
{
    return my_offset;
}

inline std::size_t symbol::size() const
{
    return my_size;
}

inline std::size_t& symbol::size()
{
    return my_size;
}

inline typename symbol::flags_type symbol::flags() const
{
    return my_flags;
}

inline typename symbol::flags_type& symbol::flags()
{
    return my_flags;
}

inline const typename symbol::metadata_type& symbol::metadata() const
{
    return my_metadata;
}

inline typename symbol::metadata_type& symbol::metadata()
{
    return my_metadata;
}

inline typename symbol::symbol_type symbol::get_new_temporary()
{
    symbol_type result;

    auto num = my_tmp_counter.value();

    if(num < 999999)
    {
        datum_type t [] = { '0', '0', '0', '0', '0', '0', '~', '$', '_', 'T', 'M', 'P', '_', '$', '~', 0 }; // 15 + 0 = 16

        char b [] = { '0', '0', '0', '0', '0', '0' };

        auto n = array_size(b);

        _itoa_s(static_cast<int>(num), b, n, 10);

        for(auto k = 0; k < n; k++)
        {
            t[k] = b[k];
        }

        result = factory::create<symbol>(num);

        (*result).name() = t;
    }

    return result;
}

inline string_type symbol::to_string() const
{
    string_type result;

    result = text::codepoints_to_string(my_name.c_str(), my_name.size());

    return result;
}

END_NAMESPACE

#endif // __IR_SYMBOL_H__
