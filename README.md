# gim

This is a simple C++ program for managing multiple gitignore configurations.

You can use prebuilt snippets from https://github.com/github/gitignore, and/or you can create your own local snippets.

To get started with local snippets, create a directory and place one or more files in the directory containing gitignore fragments.  The fragment filename will be used to tell the gitignore manager what to enable or disable and should end in ".gitignore".

Using the gitignore manager is simple.  It has two primary commands: enable and disable. You can specify multiple fragment filenames.

For example, to enable a gitignore fragment called "C#":

$ gim enable staging

You can ask for the status of the available gitignore fragments by passing the --status flag:

$ gim status
+ staging
$

The first character of the status ouput will be ' ' if the fragment is not enabled, '+' if it is enabled, and '*' if it is enabled and has been modified in the .gitignore file.
