#include "Manager.h"

Manager::Manager(std::string gitignore_path, std::string fragment_path)
{
    _gitignore_path = gitignore_path;
    load_gitignore_file();
    _load_fragments(fragment_path);
}

void Manager::load_gitignore_file()
{
    _gitignore_file = _read_file(_gitignore_path);
}

void Manager::write_gitignore_file()
{
    _write_file(_gitignore_path, _gitignore_file);
}

bool Manager::fragment_enabled(std::string name)
{
    const Fragment* fragment = get_fragment(name);
    return fragment != NULL;
}

void Manager::enable_fragment(std::string name)
{
    const Fragment* fragment = get_fragment(name);
    if (fragment == NULL)
    {
        return;
    }

    if (fragment->enabled())
    {
        disable_fragment(name);
    }

    _gitignore_file.reserve(_gitignore_file.length() + fragment->length());

    _gitignore_file.append(fragment->begin());
    _gitignore_file.append(fragment->ext_contents());
    _gitignore_file.append(fragment->end());
}

void Manager::disable_fragment(std::string name)
{
    const Fragment* fragment = get_fragment(name);
    if (fragment == NULL)
    {
        return;
    }

    _gitignore_file.erase(fragment->b_start(), fragment->length());
}

void Manager::toggle_fragment(std::string name)
{
    if (fragment_enabled(name))
    {
        disable_fragment(name);
    }
    else
    {
        enable_fragment(name);
    }
}

const Fragment* Manager::get_fragment(std::string name)
{
    std::map<std::string, Fragment*>::iterator it = _fragments.find(name);

    if (it != _fragments.end())
    {
        return it->second;
    }

    return NULL;
}

void Manager::_load_fragments(std::string l_path, std::string s_path)
{
    DIR* dir; dirent* pdir;
    //From my workspace
    dir = opendir(l_path.c_str());
    while ((pdir = readdir(dir)))
    {
        std::string l2_path(l_path);
        l2_path.append("/");
        l2_path.append(pdir->d_name);
        std::string s2_path(s_path);
        if (s_path.length() > 0)
            s2_path.append("/");
        s2_path.append(pdir->d_name);
        if (*(pdir->d_name) !=  '.')
        {
            if (pdir->d_type == DT_DIR)
            {
                _load_fragments(l2_path, s2_path);
            }
            else
            {
                int p = s2_path.find(".gitignore");
                if (p > -1)
                {
                    s2_path.replace(p, 10, "");
                    Fragment* f = new Fragment(s2_path, _read_file(l2_path), _gitignore_file);
                    _fragments.insert(std::pair<std::string, Fragment*>(s2_path, f));
                }
            }
        }
    }
    closedir(dir);
}

const std::string Manager::_read_file(std::string filename)
{
    std::ifstream infile(filename);
    std::string contents;

    infile.seekg(0, std::ios::end);
    contents.reserve(infile.tellg());
    infile.seekg(0, std::ios::beg);

    contents.assign((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
    return contents;
}

void Manager::_write_file(std::string filename, std::string contents)
{
    std::ofstream outfile(filename);
    outfile.write(contents.c_str(), contents.length());
}

std::map<std::string, Fragment*> Manager::fragment_list()
{
    return _fragments;
}
