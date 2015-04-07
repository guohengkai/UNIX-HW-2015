/*************************************************************************
    > File Name: csv_map_reducer.h
    > Author: Guo Hengkai
    > Description: CSV Map Reducer class definition for homework 7B
    > Created Time: Sun 05 Apr 2015 05:01:06 PM CST
 ************************************************************************/
#ifndef SEVEN_B_CSV_MAP_REDUCER_H_
#define SEVEN_B_CSV_MAP_REDUCER_H_

#include <string>
#include "../6A/csv_handler.h"

namespace ghk
{
class CsvMapReducer
{
public:
    CsvMapReducer(int split_num): split_num_(split_num) {}
    bool MapReduce(const std::string &input_name,
                   const std::string &output_name);

    inline int split_num() const { return split_num_; }
    inline void set_split_num(int split_num) { split_num_ = split_num; }
private:
    bool SplitCsvFile(const std::string &input_name) const;
    int split_num_;
    CsvHandler handler_;
};
}  // namespace ghk

#endif  // SEVEN_B_CSV_MAP_REDUCER_H_
