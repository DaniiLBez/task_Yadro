#!/bin/bash
mkdir -m 777 -p cmake_build

docker build -t dev:task_Yadro -f Dockerfile .

docker run --rm -it \
    -u 1000:1000 \
    -v "$(pwd):/tmp/task_Yadro" \
    dev:task_Yadro \
    cmake -DCMAKE_BUILD_TYPE=DEBUG \
    -G Ninja \
    -S /tmp/task_Yadro \
    -B /tmp/task_Yadro/cmake_build 

docker run --rm -it \
    -u 1000:1000 \
    -v "$(pwd):/tmp/task_Yadro" \
    dev:task_Yadro \
    cmake --build /tmp/task_Yadro/cmake_build