/*
    This file is part of command-not-found.
    Copyright (C) 2011 Matthias Maennich <matthias@maennich.net>

    command-not-found is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    command-not-found is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with command-not-found.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <getopt.h>
#include <exception>
#include <iostream>
#include <string>
#include <boost/filesystem.hpp>

#include "db.h"
#include "config.h"

namespace bf = boost::filesystem;
using namespace cnf;
using namespace std;

static struct args_t {
    string catalog;
    string package_path;
    string database_path;
    int verbosity;
    bool mirror;
    bool truncate;
} args;

static const char* OPT_STRING = "p:c:mtd:vh?";

static const struct option LONG_OPTS[] = {
    {"database-path", required_argument, NULL, 'd'},
    {"catalog", required_argument, NULL, 'c'},
    {"mirror", no_argument, NULL, 'm'},
    {"truncate", no_argument, NULL, 't'},
    {"package-path", required_argument, NULL, 'p'},
    {"verbose", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},
    {NULL, no_argument, NULL, 0}
};

void usage(){
    cout << "       *** " << PROGRAM_NAME << " " << VERSION_LONG << " ***           \n"
            "Usage:                                                                 \n"
            "   cnf-populate -p <path> ( -c <catalog> | -m ) [ -d <path> ]          \n"
            "                                                                       \n"
            "Options:                                                               \n"
            " --help            -? -h     Show this help and exit                   \n"
            " --verbose         -v        Display verbose output                    \n"
            "                                                                       \n"
            " --package-path    -p        Set the path containing the packages      \n"
            " --catalog         -c        Set the catalog name to index (e.g. core) \n"
            " --mirror          -m        Scan mirror structure and detect catalogs \n"
            " --truncate        -t        Truncate the catalog before indexing      \n"
            " --database-path   -d        Customize the database lookup path        \n"
            "                             default is " << DATABASE_PATH << "        \n"
         << endl;
    exit(1);
}

int main(int argc, char** argv) {

    args.database_path = DATABASE_PATH;
    args.catalog = "";
    args.mirror = false;
    args.package_path = "";
    args.verbosity = 0;

    int opt(0), long_index(0);

    opt = getopt_long(argc, argv, OPT_STRING, LONG_OPTS, &long_index);
    while (opt != -1) {
        switch (opt) {
            case 'd':
                args.database_path = optarg;
                break;
            case 'c':
                args.catalog = optarg;
                break;
            case 'p':
                args.package_path = optarg;
                break;
            case 'm':
                args.mirror = true;
                break;
            case 't':
                args.truncate = true;
                break;
            case 'v':
                args.verbosity++;
                break;
            case 'h':
            case '?':
                usage();
                break;
            default:
                break;
        }
        opt = getopt_long(argc, argv, OPT_STRING, LONG_OPTS, &long_index);
    }

    if (argc - optind != 0) {
        usage();
    }

    if (args.package_path.empty()) {
        usage();
    }

    if (args.mirror && !args.catalog.empty()) {
        usage();
    }

    if (!bf::is_directory(args.package_path)) {
        cerr << "Not a valid package path: " << args.package_path << endl << endl;
        usage();
        return 1;
    }

    if (args.mirror) {
        populate_mirror(args.package_path, args.database_path, args.truncate,
                        args.verbosity);
    } else {
        populate(args.package_path, args.database_path, args.catalog,
                 args.truncate, args.verbosity);
    }
    return 0;
}
