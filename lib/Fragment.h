#pragma once
#include <string>

class Fragment
{
public:
    Fragment (std::string name, std::string contents, std::string gitignore_file);

    size_t      const b_length() const;
    std::string const begin() const;
    size_t      const b_start() const;

    std::string const ext_contents() const;
    std::string const int_contents() const;
    size_t      const ic_length() const;
    size_t      const ic_start() const;

    size_t      const e_length() const;
    std::string const end() const;
    size_t      const e_start() const;

    size_t      const length() const;
    bool        const enabled() const;
    bool        const modified() const;

private:
    size_t _b_length;
    std::string _begin;
    size_t _b_start;

    std::string _ext_contents;
    std::string _int_contents;
    size_t _ic_length;
    size_t _ic_start;

    size_t _e_length;
    std::string _end;
    size_t _e_start;

    bool _found;
    size_t _length;
};
