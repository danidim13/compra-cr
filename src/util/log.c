//
// Created by daniel on 03/09/19.
//

#include "log.h"
#include "stdlib.h"
#include "time.h"

struct stLogger * default_logger = NULL;

struct stLogger * get_default_logger()
{
    if (default_logger == NULL) {
        default_logger = malloc(sizeof(struct stLogger));
        init_logger(default_logger, DEFAULT_LOG_LEVEL);
    }
    return default_logger;
}

void init_logger(struct stLogger* logger, LogLevel log_level)
{
    logger->current_log_level = log_level;
    logger->logfile_debug = fopen("./logs/debug.log", "a");
    logger->logfile_info = logger->logfile_debug;
    logger->logfile_warning = fopen("./logs/error.log", "a");
    logger->logfile_error = logger->logfile_warning;
}

get_time_str(char* buffer)
{
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char time_format[] = "%F %T";
    strftime(buffer, TIME_STR_LEN, time_format, timeinfo);
}

void log_debug(struct stLogger* logger, char* msg)
{
    if (logger == NULL)
        logger = get_default_logger();

    if (logger->current_log_level > DEBUG)
        return;

    char time_str[TIME_STR_LEN];
    char log_format[] = "[%s %s] %s\n";
    get_time_str(time_str);

    fprintf(logger->logfile_debug, log_format, time_str, "DEBUG", msg);
}

void log_info(struct stLogger* logger, char* msg)
{
    if (logger == NULL)
        logger = get_default_logger();

    if (logger->current_log_level > INFO)
        return;

    char time_str[TIME_STR_LEN];
    char log_format[] = "[%s %s] %s\n";
    get_time_str(time_str);

    fprintf(logger->logfile_debug, log_format, time_str, "INFO", msg);
}

void log_warning(struct stLogger* logger, char* msg)
{
    if (logger == NULL)
        logger = get_default_logger();

    if (logger->current_log_level > WARNING)
        return;

    char time_str[TIME_STR_LEN];
    char log_format[] = "[%s %s] %s\n";
    get_time_str(time_str);

    fprintf(logger->logfile_warning, log_format, time_str, "WARNING", msg);
}

void log_error(struct stLogger* logger, char* msg)
{
    if (logger == NULL)
        logger = get_default_logger();

    if (logger->current_log_level > ERROR)
        return;

    char time_str[TIME_STR_LEN];
    char log_format[] = "[%s %s] %s\n";
    get_time_str(time_str);

    fprintf(logger->logfile_error, log_format, time_str, "ERROR", msg);
}
