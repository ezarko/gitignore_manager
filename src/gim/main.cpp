#include <getopt.h>
#include <iostream>
#include <set>
#include <string>
#include "Manager.h"

#define VERSION "0.2"

static const char * optstring = "vgi:";
static struct option longopts[] = {
    { "version",     no_argument,       NULL, 'v' },
    { "global",      no_argument,       NULL, 'g' },
    { "ignore-file", required_argument, NULL, 'i' },
    { NULL,          0,                 NULL, 0 }
};

void usage()
{
    std::cout << "usage: gim [--version] [--global] [--ignore-file=<name>] <command> [<args>]\n"
              << "\n"
              << "These are Gim commands used in various situations:\n"
              << "\n"
              << "   enable    Enable one or more gitignore fragments.\n"
              << "   disable   Disable one or more gitignore fragments.\n"
              << "   status    Display a list of fragments and their status.\n"
              << "             A '+' indicates the fragment is enabled.\n"
              << "             A '*' indicates an enabled fragment has been changed.\n"
              << "   help      This message.\n";
    exit(0);
}

std::string fragment_status_line(Manager* manager, std::string name)
{
    const Fragment* f = manager->get_fragment(name);
    std::string line;
    line += f->enabled() ? ( f->modified() ? "*" : "+" ) : " ";
    line += " ";
    line += name;
    return line;
}

std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

int main(int argc, char** argv)
{
    int ch;
    std::string gitignore_file = ".gitignore";
    std::string gitignore_dir;

    while ((ch = getopt_long(argc, argv, optstring, longopts, NULL)) != -1) {
        switch (ch) {
            case 'v':
                std::cout << "gim version " << VERSION << /* " (Apple Git-128)" << */ "\n";
                exit(0);
            case 'g':
                gitignore_file = exec("git config --global core.excludesfile");
                gitignore_file.pop_back();
                break;
            case 'i':
                gitignore_file = optarg;
                break;
            default:
                usage();
        }
    }

    if (optind == argc)
    {
        usage();
    }
    gitignore_dir = exec("git config --global core.excludesdir");
    gitignore_dir.pop_back();
    Manager* manager = new Manager(gitignore_file, gitignore_dir);
    if (strcmp(argv[optind], "enable") == 0)
    {
        for (int i = optind+1; i < argc; ++i)
        {
            std::cout << "Enabling " << argv[i] << "\n";
            manager->enable_fragment(argv[i]);
        }
        std::cout << "Writing file\n";
        manager->write_gitignore_file();
    }
    else if (strcmp(argv[optind], "disable") == 0)
    {
        for (int i = optind+1; i < argc; ++i)
        {
            std::cout << "Disabling " << argv[i] << "\n";
            manager->disable_fragment(argv[i]);
        }
        std::cout << "Writing file\n";
        manager->write_gitignore_file();
    }
    else if (strcmp(argv[optind], "status") == 0)
    {
        std::map<std::string, Fragment*> list = manager->fragment_list();
        for (std::map<std::string, Fragment*>::iterator it = list.begin(); it != list.end(); ++it)
        {
            std::cout << fragment_status_line(manager, it->first) << "\n";
        }
    }
    else
    {
        if (strcmp(argv[optind], "help") != 0)
        {
            std::cerr << "gim: unrecognized command '" << argv[optind] << "'\n";
        }
        usage();
    }
}
