#ifndef GENERAL_H
#define GENERAL_H

#include "enums.h"
#include <string>
#include <vector>

static const std::string DEFAULT_LOG_FILE_NAME = "application.log";
static const std::vector<Direction> DIRECTIONS = { Direction::South, Direction::West, Direction::North, Direction::East };
static const std::string DEFAULT_HOUSE_FILE_EXTENSION = ".house";
static const std::string DEFAULT_ALGORITHM_FILE_EXTENSION = ".so";
static const std::string DEFAULT_HOUSE_ARG = "-house_path=";
static const std::string DEFAULT_ALGORITHM_ARG = "-algo_path=";
static const std::string DEFAULT_NUM_THREADS_ARG = "-num_threads=";
static const std::string DEFAULT_SUMMARY_ARG = "-summary_only";
static const std::string DEFAULT_HOUSE_DIR_PATH = "./";
static const std::string DEFAULT_ALGORITHM_DIR_PATH = "./";
static const std::size_t DEFAULT_NUM_THREADS_VALUE = 10;
static const std::size_t THREAD_ERROR_CODE = 0;
static const std::string DEFAULT_SUMMARY_FILE_NAME = "summary.csv";
static const std::string DEFAULT_TIMEOUT_CHAR = "T";
static const std::string DEFAULT_FINISHED_CHAR = "F";

#endif
