# Build using
# $> docker build -f docker/Dockerfile -t <lovely-name> .
# in the root path of the repository.
#

FROM ubuntu:16.04
MAINTAINER Ben Hermann <Ben.Hermann@uni-paderborn.de>

ENV LANG=en_US.UTF-8

RUN apt-get update

# general dependencies
RUN apt-get -y install zlib1g-dev libncurses5-dev sqlite3 libsqlite3-dev libmysqlcppconn-dev bear python3 doxygen graphviz

# boost dependencies
RUN apt-get -y install wget gcc g++

# build and install boost
RUN wget https://dl.bintray.com/boostorg/release/1.66.0/source/boost_1_66_0.tar.gz && \
	tar xvf boost_1_66_0.tar.gz && \
	cd boost_1_66_0/ && \
	./bootstrap.sh && \
	./b2 install

# llvm dependencies
RUN apt-get -y install software-properties-common python-software-properties subversion sudo texinfo

# framework dependencies
RUN apt-get -y install make libcurl4-gnutls-dev git

# build and install cmake
RUN wget https://github.com/Kitware/CMake/releases/download/v3.13.2/cmake-3.13.2.tar.gz && \
    tar xavf cmake-3.13.2.tar.gz && \
    cd cmake-3.13.2 && \
    ./bootstrap && \
    make && \
    make install

# copy llvm install script
ADD ./utils/install-llvm-5.0.1.sh /opt

# build and install llvm
RUN ./opt/install-llvm-5.0.1.sh $(nproc) ~/

# copy source code
ADD . /opt/framework

# build framework
RUN cd /opt/framework && \
    mkdir build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j $(nproc)

