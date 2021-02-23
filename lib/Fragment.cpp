#include "Fragment.h"

Fragment::Fragment (std::string name, std::string contents, std::string gitignore_file)
{
    _b_length = 10 + name.length();
    _begin.reserve(_b_length);
    _begin = "# BEGIN: ";
    _begin += name.c_str();
    _begin += "\n";
    _b_start = gitignore_file.find(_begin);

    _e_length = 8 + name.length();
    _end.reserve(_e_length);
    _end = "# END: ";
    _end += name.c_str();
    _end += "\n";
    _e_start = gitignore_file.find(_end);

    _ext_contents = contents;
    if ((_b_start != std::string::npos) && (_e_start != std::string::npos) && (_b_start < _e_start))
    {
        _found = true;
        _ic_start = _b_start + _b_length;
        _ic_length = _e_start - _ic_start;
        _int_contents = gitignore_file.substr(_ic_start, _ic_length);
    }
    else
    {
        _found = false;
        _ic_length = 0;
        _ic_start = std::string::npos;
    }

    _length = _b_length + _ic_length + _e_length;
}

size_t const Fragment::b_length() const
{
    return _b_length;
}

std::string const Fragment::begin() const
{
    return _begin;
}

size_t const Fragment::b_start() const
{
    return _b_start;
}

std::string const Fragment::ext_contents() const
{
    return _ext_contents;
}

std::string const Fragment::int_contents() const
{
    return _int_contents;
}

size_t const Fragment::ic_length() const
{
    return _ic_length;
}

size_t const Fragment::ic_start() const
{
    return _ic_start;
}

size_t const Fragment::e_length() const
{
    return _e_length;
}

std::string const Fragment::end() const
{
    return _end;
}

size_t const Fragment::e_start() const
{
    return _e_start;
}

size_t const Fragment::length() const
{
    return _length;
}

bool const Fragment::enabled() const
{
    return _found;
}

bool const Fragment::modified() const
{
    return _ext_contents.compare(_int_contents) != 0;
}
