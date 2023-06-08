#!/bin/bash

# @author Alexander Hinze
# @since 08/06/2023

/media/data/develop/llvm-project/build/bin/clang-format -i -style=file --verbose ./include/kstd/*.hpp
/media/data/develop/llvm-project/build/bin/clang-format -i -style=file --verbose ./test/*.cpp
