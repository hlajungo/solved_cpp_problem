#!/bin/bash

if [[ -z $1 ]]; then
  echo "A dir name argument required."
  echo "For exmaple, d103 will generate dir \"d103\" and d103/d103.cpp ."
fi

mkdir $1
cd $1
bash ~/scripts/genCodeRun.sh
bash ~/scripts/gen_cpp_template.sh $1.cpp
