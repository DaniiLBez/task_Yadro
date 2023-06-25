FROM gcc:11.3.0 as build

RUN apt-get update && apt-get install -y\
    cmake \
    clang \
    clang-format \
    clang-tidy \
    cmake \
    cppcheck \
    gdb \
    iwyu \
    lld \
    lldb \
    make \
    ninja-build

ARG UID=1000
RUN useradd -m -u ${UID} -s /bin/bash builder