//
// Created by daniel on 03/09/19.
//

#ifndef APP_ECOMMERCE_LOG_H
#define APP_ECOMMERCE_LOG_H

#include <stdio.h>


/**
 * Logger levels
 */
typedef enum {
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR,
} LogLevel;

#define DEFAULT_LOG_LEVEL INFO
#define TIME_STR_LEN 25
#define LOG_FOLDER "logs/"

struct stLogger {
    FILE * logfile_debug;
    FILE * logfile_info;
    FILE * logfile_warning;
    FILE * logfile_error;
    LogLevel current_log_level;
};

extern struct stLogger * default_logger;

struct stLogger * get_default_logger();
void init_logger(struct stLogger* logger, LogLevel log_level);
void log_debug(struct stLogger* logger, char* msg);
void log_info(struct stLogger* logger, char* msg);
void log_warning(struct stLogger* logger, char* msg);
void log_error(struct stLogger* logger, char* msg);

#endif //APP_ECOMMERCE_LOG_H
