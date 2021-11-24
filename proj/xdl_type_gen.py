#!/usr/bin/env python3

# It's expected that this file will be run from within the proj directory, if
# you run it from elsewhere things may break.
# (Why on earth are you running this tho?)

import json
from pathlib import Path


def main():
    with open("xdl_types.json", "r") as f:
        type_dict = json.load(f)

    names_file_path = Path(__file__).parents[1] / "src/util/datatype1.cc"
    types_file_path = Path(__file__).parents[1] / "src/util/datatype1.hh"

    with open(names_file_path, "w") as names, open(types_file_path, "w") as types:
        types.write(
            """\
// This file was generated by a python script.
// Do not edit this file... YOUR CHANGES WILL GET OVERWRITTEN...
// Edit proj/xdl_types.json instead.
#pragma once

#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

/*
   The list of XDL types specific to this application.
 */
enum class DataType {\n"""
        )

        names.write(
            """\
// This file was generated by a python script.
// Do not edit this file... YOUR CHANGES WILL GET OVERWRITTEN...
// Edit proj/xdl_types.json instead.

#include "util/datatype1.hh"

 /*
   The list of XDL types specific to this application.
   Note: When a comment says 'n byte length' it means that the type
         has 2^(8*n) elements
 */
using namespace std;

const char* DataTypeNames[] = {\n"""
        )

        for type_name, name in type_dict.items():
            types.write(f"  {type_name},\n")
            names.write(f'    "{name["name"]}",  //{name["comment"]}\n')

        types.write("};\n")
        names.write("};\n")

        names.write(
            """\n
unordered_map<string,DataType> mapnames;
void loadmap(){
  for (int i = 0; i < int(DataType::ENUM_SIZE); i++)
    mapnames[DataTypeNames[i]] = (DataType)i;
}"""
        )

        types.write(
            """\n
extern const char* DataTypeNames[];
extern std::unordered_map<std::string, DataType> mapnames;
extern void loadmap();"""
        )


if __name__ == "__main__":
    main()
