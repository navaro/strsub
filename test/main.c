
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <time.h>
#include "../src/strsub.h"

#define ENGINE_VERSION_STR      "Navaro strsub Demo v '" __DATE__ "'"

#define OPTION_ID_HELP              4
#define OPTION_ID_CONFIG_FILE       8

struct option opt_parm[] = {
    { "help",no_argument,0,OPTION_ID_HELP },
    { "config",required_argument,0,OPTION_ID_CONFIG_FILE },
    { 0,0,0,0 },
};

static char *              opt_text = 0;
static char *              opt_config_file = "default.cfg" ;

static STRSUB_HANDLER_T     _strsub_config ;
static STRSUB_HANDLER_T     _strsub_date ;
static STRSUB_HANDLER_T     _strsub_date2 ;
static STRSUB_HANDLER_T     _strsub_consolewriter ;
#pragma GCC diagnostic ignored  "-Wmissing-braces"
static STRSUB_INSTANCE_T    _strsub_consolewriter_inst = {'\\', {"##", 0, 0}, {0}} ;

static int32_t      strsub_config_cb (STRSUB_REPLACE_CB cb, const char * str, size_t len, uint32_t offset, uintptr_t arg) ;
static int32_t      strsub_date_cb (STRSUB_REPLACE_CB cb, const char * str, size_t len,  uint32_t offset, uintptr_t arg) ;
static int32_t      strsub_consolewriter_cb (STRSUB_REPLACE_CB cb, const char * str, size_t len,  uint32_t offset, uintptr_t arg) ;



static int32_t      consolewriter_cb (const char * str, uint32_t len, uint32_t offset, uintptr_t arg) ;


void
usage(char* comm)
{
    printf (
        "usage:\n"
        "  %s to compile and start an Engine Machine definition file.\n\n"
        "  %s <text> [OPTIONS]\n"
        "    <text>               Text to parse.\n"
        "    --help                Shows this message.\n"
        "    --config              Configuration file.\n"
        ,
        ENGINE_VERSION_STR,
        comm);
    exit (0);
}



int
main(int argc, char* argv[])
{
    char c;
    int opt_index = 0;
    uint32_t len ;
    uint32_t dstlen ;
    char * newwtext ;

    /*
     * Parse the command line parameters.
     */
    while ((c = getopt_long (argc, (char *const *) argv, "-h", opt_parm, &opt_index)) != -1) {
        switch (c) {
        case 1:
            opt_text = optarg;
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

    if (!opt_text) {
        /*
         * No text to parse. Exit.
         */
        usage (argv[0]);
        return 0;

    }


    /*
     * Example 1:  Parse a string from a source to a destination replacing text
     *             from the configuration file and the [date] string.
     *
     *             We use for both replacement handlers StrsubToken1. For example
     *             purposes we also put the date using StrsubToken2 (%%).
     */
    strsub_install_handler (0, StrsubToken1, &_strsub_config, strsub_config_cb) ;
    strsub_install_handler (0, StrsubToken1, &_strsub_date, strsub_date_cb) ;
    strsub_install_handler (0, StrsubToken2, &_strsub_date2, strsub_date_cb) ;

    /*
     * Example 1
     */

    /*
     * Determine the length of the source and destination strings.
     */
    len = strlen (opt_text) ;
    dstlen = strsub_parse_get_dst_length (0, opt_text, len) + 1 ;
    /*
     * Allocate memory for the destination string.
     */
    newwtext = malloc(dstlen) ;
    if (newwtext) {
        /*
         * Parse the source string to the destination string replacing all
         * delimited text.
         */
        strsub_parse_string_to (0, opt_text, len, newwtext, dstlen) ;

        /*
         * First process example 2 before we print the result.
         */

    } else {
        printf ("out of memory!\r\n")  ;
        return 0 ;

    }

    /*
     * Example 2:  Use the output of example 1. This time we simply write
     *             the string directly to the console. The string is
     *             parsed and all ASCII key codes delimited with '#' delimiters
     *             are printed directly to the console in the callback handler.
     *             This function can be used to execute other commands also
     *             like changing the font etc.
     *
     *             All other characters are written console in the
     *             consolewriter_cb() function.
     */

    /*
     * Install a handler for the new instance of strsub to
     * _strsub_consolewriter_inst. This handler writes  the string to the
     * console but replace all integers delimited with '#' with their
     * ASCII character.
     */
    strsub_install_handler (&_strsub_consolewriter_inst, StrsubToken1,
                &_strsub_consolewriter, strsub_consolewriter_cb) ;
    /*
     * Parse the string using the new instance of strsub.
     */
    strsub_parse (&_strsub_consolewriter_inst,
                consolewriter_cb, newwtext, strlen(newwtext), 0) ;
    /*
     * We dont need to print the result because it is
     * already done by consolewriter_cb().
     */

    printf ("\r\n") ;



    strsub_uninstall_handler (0, StrsubToken2, &_strsub_date2) ;
    strsub_uninstall_handler (0, StrsubToken1, &_strsub_config) ;
    strsub_uninstall_handler (0, StrsubToken1, &_strsub_date) ;
    strsub_uninstall_handler (&_strsub_consolewriter_inst, StrsubToken1,
                &_strsub_consolewriter) ;


    free (newwtext) ;
    return 0;
}


/**
 * @brief   lookup a key-value pair in a text file
 * @note    key need not be zero terminated
 * @post    returned value must be freed with free()
 * @param[in] key   key to find in the config file
 * @param[in] len   length of key
 * @return          allocated value for the key. NULL if not found
 */
static char
*read_string(char const *key, size_t len) {
    char name[128];
    char val[128];
    char * str = NULL ;
    FILE * fp;
    fp = fopen(opt_config_file, "r");
    if (fp == NULL) {
        return NULL ;

    }

    while (fscanf(fp, "%127[^=]=%127[^\n]%*c", name, val) == 2) {
        char* pname = &name[0] ;
        while (isspace((int)*pname)) pname++ ;
        if (0 == strncmp(pname, key, len)) {
            char* pval = &val[0] ;
            while (isspace((int)*pval)) pval++ ;
            str = strdup(pval);
            char * end = str + strlen(str) - 1;
            while((end >= str) && (isspace((int)*end))) {
                *end = '\0' ;
                end-- ;
            }
            break ;

        }

    }
    fclose(fp);

    return str;
}

/**
 * @brief   strsub callback handler.
 * @note    will replace text with the value if found as key-value pair in
 *          the config file.
 */
int32_t
strsub_config_cb(STRSUB_REPLACE_CB cb, const char * str, size_t len,
                    uint32_t offset, uintptr_t arg)
{
    int32_t res = -1 ;

    char *temp = read_string(str, len);
    if (temp) {
        int32_t dstlen = strlen(temp) ;
        res = cb (temp, dstlen, offset, arg) ;
        free(temp);

    }

    return res ;
}

/**
 * @brief   strsub callback handler.
 * @note    will replace "date" with the todays date.
 */
int32_t
strsub_date_cb(STRSUB_REPLACE_CB cb, const char * str, size_t len,
                    uint32_t offset, uintptr_t arg)
{
    int32_t res = -1 ;

    if (strncmp(str, "date", len) == 0) {

        char tmp[64] ;
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        snprintf(tmp, sizeof(tmp), "%d-%02d-%02d %02d:%02d:%02d",
              tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
              tm.tm_hour, tm.tm_min, tm.tm_sec);

        res = cb (tmp, strlen(tmp), offset, arg) ;
    }

    return res ;
}

/**
 * @brief   strsub callback handler.
 * @note    Print ASSCII for key code. Can be used to execute other commands
 *          like changing the font etc.
 */
int32_t
strsub_consolewriter_cb (STRSUB_REPLACE_CB cb, const char * str, size_t len,
        uint32_t offset, uintptr_t arg)
{
    int32_t res = 0 ;
    int ch ;

    if (*str == '#') {
        /* special case: here "##" also escapes and writes one "#" */
        return cb(str, 1, offset,  arg) ;
    }

    if (sscanf(str, "%u", &ch) == 1) {

        printf ("%c", (char)ch) ;

    }

    return res ;
}

/**
 * @brief   Callback handler.
 * @note    Write all characters to the console.
 */
int32_t
consolewriter_cb (const char * str, uint32_t len,
                                    uint32_t offset, uintptr_t arg)
{
    for (int i=0; i<len; i++) {
        putchar ((int)str[i]) ;

    }

    return len ;
}


