
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>

#define ENGINE_VERSION_STR      "Navaro strsub Demo v '" __DATE__ "'"

#define OPTION_ID_HELP              4
#define OPTION_ID_VERBOSE           6
#define OPTION_ID_LIST              7
#define OPTION_ID_CONFIG_FILE       8
#define OPTION_COMMENT_MAX          256

struct option opt_parm[] = {
    { "help",no_argument,0,OPTION_ID_HELP },
    { "config",required_argument,0,OPTION_ID_CONFIG_FILE },
    { 0,0,0,0 },
};

char *              opt_file = 0;
char *              opt_config_file = 0;


void
usage(char* comm)
{
    printf (
        "usage:\n"
        "  %s to compile and start an Engine Machine definition file.\n\n"
        "  %s <file> [OPTIONS]\n"
        "    <file>                file.\n"
        "    --help                Shows this message.\n"
        "    --config              Configuration file or \"registry\" (default file.cfg).\n"
        ,
        ENGINE_VERSION_STR,
        comm);
    exit (0);
}

static char *   get_config_file(void) ;


int
main(int argc, char* argv[])
{
    char c;
    int opt_index = 0;
    int32_t res ;
    printf (ENGINE_VERSION_STR) ;
    printf ("\r\n\r\n") ;

    /*
     * Parse the command line parameters.
     */
    while ((c = getopt_long (argc, (char *const *) argv, "-h", opt_parm, &opt_index)) != -1) {
        switch (c) {
        case 1:
            opt_file = optarg;
            break;

        case 'h':
        case OPTION_ID_HELP:
            usage (argv[0]);
            return 0;

        case OPTION_ID_CONFIG_FILE:
            opt_config_file = optarg ;
            break ;

         }

    }


 
     return 0;
}

static char *
get_config_file (void)
{
    static char config_file[FILENAME_MAX+4] ;
    memset (config_file, 0, FILENAME_MAX) ;

    if (opt_config_file) {
        strncpy(config_file, opt_config_file, FILENAME_MAX-1);

    }
    else if (opt_file) {

        strncpy(config_file,opt_file,FILENAME_MAX-1);

        char *end = config_file + strlen(config_file);

        while (end > config_file && *end != '.') {
            --end;
        }

        if (end > config_file) {
            *end = '\0';
        }

        strcat (config_file, ".cfg") ;


    }

    return (char*) config_file ;
}

