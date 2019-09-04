//
// Created by daniel on 03/09/19.
//

#include "log.h"
#include <stdlib.h>
#include <time.h>

struct stLogger * default_logger = NULL;

struct stLogger * get_default_logger()
{
    if (default_logger == NULL) {
        default_logger = (struct stLogger*) malloc(sizeof(struct stLogger));
        init_logger(default_logger, DEFAULT_LOG_LEVEL);
    }
    return default_logger;
}

void init_logger(struct stLogger* logger, LogLevel log_level)
{
    logger->current_log_level = log_level;
    logger->logfile_debug = fopen( LOG_FOLDER "debug.log", "a");
    if (logger->logfile_debug == NULL) {
        perror("Error while opening log file");
    }
    logger->logfile_info = logger->logfile_debug;
    logger->logfile_warning = fopen( LOG_FOLDER "error.log", "a");
    if (logger->logfile_warning == NULL) {
        perror("Error while opening log file");
    }
    logger->logfile_error = logger->logfile_warning;
}

void get_time_str(char* buffer)
{
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char time_format[] = "%F %T";
    strftime(buffer, TIME_STR_LEN, time_format, timeinfo);
}

void log_general(FILE * logfile, const char* loglevel, char* msg) {

    if (logfile == NULL)
        return;

    char time_str[TIME_STR_LEN];
    char log_format[] = "[%s %s] %s\n";
    get_time_str(time_str);

    fprintf(logfile, log_format, time_str, loglevel, msg);
}

void log_debug(struct stLogger* logger, char* msg)
{
    if (logger == NULL)
        logger = get_default_logger();

    if (logger->current_log_level > DEBUG)
        return;

    log_general(logger->logfile_debug, "DEBUG", msg);
}

void log_info(struct stLogger* logger, char* msg)
{
    if (logger == NULL)
        logger = get_default_logger();

    if (logger->current_log_level > INFO)
        return;

    log_general(logger->logfile_info, "INFO", msg);
}

void log_warning(struct stLogger* logger, char* msg)
{
    if (logger == NULL)
        logger = get_default_logger();

    if (logger->current_log_level > WARNING)
        return;

    log_general(logger->logfile_warning, "WARNING", msg);
}

void log_error(struct stLogger* logger, char* msg)
{
    if (logger == NULL)
        logger = get_default_logger();

    if (logger->current_log_level > ERROR)
        return;

    log_general(logger->logfile_error, "ERROR", msg);
}
