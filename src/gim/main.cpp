#include <getopt.h>
#include <iostream>
#include <set>
#include <string>
#include "Manager.h"

static const char * optstring = "e:d:ish";
static struct option longopts[] = {
        { "enable",      required_argument, NULL, 'e' },
        { "disable",     required_argument, NULL, 'd' },
        { "interactive", no_argument,       NULL, 'i' },
        { "status",      no_argument,       NULL, 's' },
        { "help",        no_argument,       NULL, 'h' },
        { NULL,          0,                 NULL, 0 }
};

void usage(int code = 0)
{
    std::cout << "gim [options] <gitignore file> <gitignore parts directory>\n"
              << "  Options:\n"
              << "        --enable\n"
              << "        Enable one or more hostfile fragments.  This option may be specified multiple times.\n"
              << "        --disable\n"
              << "        Disable one or more hostfile fragments.  This option may be specified multiple times.\n"
              << "        --status\n"
              << "        Display a list of fragments and their status.  A '+' indicates the fragment is enabled.  A '*' indicates an enabled fragment has been changed in /etc/hosts.\n"
              << "        --interactive\n"
              << "        Presents an interactive list of hostfile fragment to enable or disable.\n";
    exit(code);
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

int main(int argc, char** argv)
{
    std::set<std::string> enabled;
    std::set<std::string> disabled;

    int ch, interactive, status;
    interactive = status = 0;
    while ((ch = getopt_long(argc, argv, optstring, longopts, NULL)) != -1) {
        switch (ch) {
            case 'e':
                enabled.insert(optarg);
                break;
            case 'd':
                disabled.insert(optarg);
                break;
            case 'i':
                interactive = 1;
                break;
            case 's':
                status = 1;
                break;
            case 'h':
                usage(1);
                break;
            default:
                usage(2);
        }
    }

    if (enabled.empty() && disabled.empty() && !status && !interactive)
    {
        usage(1);
    }

    Manager* manager = new Manager(argv[argc-2], argv[argc-1]);

    if (interactive) {
        std::cout << "Interactive not implemented.\n";
/*
        int modified = 0;

        while (
            std::string chosen = choose(
                "Select a hostfile fragment:",
                map {
                        fragment_status_line(*it);
                    }
                } manager->fragment_list
            )
        )
        {
            chosen =~ s/^..//;
            manager->toggle_fragment(chosen);
            modified = 1;
        }

        if (modified)
        {
            manager->write_hostfile();
        }
*/
    }
    else
    {
        for (std::set<std::string>::iterator it = disabled.begin(); it != disabled.end(); ++it)
        {
            std::cout << "Disabling " << *it << "\n";
            manager->disable_fragment(*it);
        }
        for (std::set<std::string>::iterator it = enabled.begin(); it != enabled.end(); ++it)
        {
            std::cout << "Enabling " << *it << "\n";
            manager->enable_fragment(*it);
        }

        if (!enabled.empty() || !disabled.empty())
        {
            std::cout << "Writing file\n";
            manager->write_gitignore_file();
        }

        if (status)
        {
            std::map<std::string, Fragment*> list = manager->fragment_list();
            for (std::map<std::string, Fragment*>::iterator it = list.begin(); it != list.end(); ++it)
            {
                std::cout << fragment_status_line(manager, it->first) << "\n";
            }
        }
    }
}
