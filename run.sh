#! /bin/bash

docker run --rm \
    -u 1000:1000 \
    -v "$(pwd):/tmp/task_Yadro" \
    dev:task_Yadro &&\
    bash -c \
    "/tmp/task_Yadro/cmake_build/src/shr_mem_writer/shr_mem_writer | \
    /tmp/task_Yadro/cmake_build/src/shr_mem_reader/shr_mem_reader"