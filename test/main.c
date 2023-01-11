
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

static STRSUB_HANDLER_T		_strsub_config ;
static STRSUB_HANDLER_T		_strsub_date ;
static STRSUB_HANDLER_T		_strsub_date2 ;
static STRSUB_HANDLER_T 	_strsub_consolewriter ;
#pragma GCC diagnostic ignored  "-Wmissing-braces"
static STRSUB_INSTANCE_T 	_strsub_consolewriter_inst = {'\\', {"##", 0, 0}, {0}} ;

static int32_t 		strsub_config_cb (STRSUB_REPLACE_CB cb, const char * str, size_t len, uint32_t offset, uintptr_t arg) ;
static int32_t		strsub_date_cb (STRSUB_REPLACE_CB cb, const char * str, size_t len,  uint32_t offset, uintptr_t arg) ;
static int32_t		strsub_consolewriter_cb (STRSUB_REPLACE_CB cb, const char * str, size_t len,  uint32_t offset, uintptr_t arg) ;



static int32_t 		consolewriter_cb (const char * str, uint32_t len, uint32_t offset, uintptr_t arg) ;


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

    uint32_t offset ;
    uint32_t len ;
    uint32_t dstlen ;
    char * newwtext ;

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

    len = strlen (opt_text) + 1;
    dstlen = strsub_parse_get_dst_length (0, opt_text, len) + 1 ;
    newwtext = malloc(dstlen) ;
    if (newwtext) {
        offset = strsub_parse_string_to (0, opt_text, len, newwtext, dstlen) ;
        newwtext[offset] = '\0' ;
        printf (newwtext) ;
        printf ("\r\n") ;

    } else {
    	printf ("%s", "out of memory!\r\n")  ;
    	return 0 ;

    }

    strsub_install_handler (&_strsub_consolewriter_inst, StrsubToken1,
    			&_strsub_consolewriter, strsub_consolewriter_cb) ;
	offset = strsub_parse (&_strsub_consolewriter_inst,
			consolewriter_cb, newwtext, strlen(newwtext), 0) ;


	printf ("\r\n%d characters written\r\n", offset) ;


	free (newwtext) ;
    return 0;
}



static char
*read_string(char const *desired_name, size_t len) {
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
        if (0 == strncmp(pname, desired_name, len)) {
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

int32_t
strsub_consolewriter_cb (STRSUB_REPLACE_CB cb, const char * str, size_t len,
		uint32_t offset, uintptr_t arg)
{
	int32_t res = 0 ;
	int ch ;

	if (*str == '#') {
		return cb(str, 1, offset,  arg) ;
	}

	if (sscanf(str, "%u", &ch) == 1) {

		printf ("%c", (char)ch) ;

	}

	return res ;
}

int32_t
consolewriter_cb (const char * str, uint32_t len,
									uint32_t offset, uintptr_t arg)
{
	for (int i=0; i<len; i++) {
		putchar ((int)str[i]) ;

	}

	return len ;
}


