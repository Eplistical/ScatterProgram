#!/bin/bash

FILE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd )"

# arguments: $1 infile $2 jobtype others:forword to python
CMD=`python3 $FILE_DIR/scatter_gen_cmd.py --infile $1 --jobtype $2 `
shift 2
CMD="$CMD $@"
eval "$CMD"
