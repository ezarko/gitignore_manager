#include <filesystem>
#include <fstream>
//#include <streambuf>
#include <sys/types.h>
#include <sys/dir.h>
#include <dirent.h>
#include <string>
#include <map>
#include "Fragment.h"

class Manager
{
public:
    Manager(std::string gitignore_path, std::string snippet_path);
    void load_gitignore_file();
    void write_gitignore_file();
    bool fragment_enabled(std::string name);
    void enable_fragment(std::string name);
    void disable_fragment(std::string name);
    void toggle_fragment(std::string name);
    const Fragment* get_fragment(std::string name);
    std::map<std::string, Fragment*> fragment_list();

private:
    void _load_fragments(std::string l_path, std::string s_path = "");
    void _load_fragment();
    const std::string _read_file(std::string filename);
    void _write_file(std::string filename, std::string contents);

    std::string _gitignore_path;
    std::string _gitignore_file;
    std::map<std::string, Fragment*> _fragments;
};
