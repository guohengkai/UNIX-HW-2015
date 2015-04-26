/*************************************************************************
    > File Name: csv_ipc_map_reducer.h
    > Author: Guo Hengkai
    > Description: CSV Map Reducer class definition using IPC for homework 8A
    > Created Time: Sat 25 Apr 2015 11:29:51 PM CST
 ************************************************************************/
#ifndef EIGHT_A_CSV_IPC_MAP_REDUCER_H_
#define EIGHT_A_CSV_IPC_MAP_REDUCER_H_

#include <string>

namespace ghk
{
class CsvIpcMapReducer
{
public:
    CsvIpcMapReducer(int split_num): split_num_(split_num) {}
    bool MapReduce(const std::string &input_name,
                   const std::string &output_name);

    inline int split_num() const { return split_num_; }
    inline void set_split_num(int split_num) { split_num_ = split_num; }
private:
    int split_num_;

};
}  // namespace ghk

#endif  // EIGHT_A_CSV_IPC_MAP_REDUCER_H_

