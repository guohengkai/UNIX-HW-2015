/*************************************************************************
    > File Name: csv_handler.cpp
    > Author: Guo Hengkai
    > Description: CSV file handler class implementation for homework 6A
    > Created Time: Sat 28 Mar 2015 05:13:31 PM CST
 ************************************************************************/
#include "csv_handler.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace ghk
{
CsvHandler::CsvHandler(string &file_name)
{

}

bool CsvHandler::Save(string &file_name) const
{
    return false;
}

bool CsvHandler::Load(string &file_name)
{
    return false;
}

bool CsvHandler::Print() const
{
    return false;
}

bool CsvHandler::Save(FILE *file, FileType type) const
{
    return false;
}

bool CsvHandler::Load(FILE *file, FileType type)
{
    return false;
}

FileType CsvHandler::GetFileTypeFromName(string &file_name) const
{
    return FileType::CSV;
}

}  // namespace ghk
