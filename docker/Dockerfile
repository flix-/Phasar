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
COPY ./utils/install-llvm-5.0.1.sh /opt/

# build and install llvm
RUN ./opt/install-llvm-5.0.1.sh $(nproc) /opt/

RUN apt-get -y install locales lcov vim && \
    locale-gen en_US.UTF-8

# copy source code
ADD . /opt/framework

# build framework
RUN cd /opt/framework && \
    mkdir build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j $(nproc) && \
    make install

WORKDIR /opt
RUN git clone https://github.com/flix-/environment-variable-tracing.git && \
    mkdir build && \
    cd build && \
#    cmake -DCMAKE_BUILD_TYPE=Debug ../environment-variable-tracing && \
    cmake -DCMAKE_BUILD_TYPE=Release ../environment-variable-tracing && \
    make -j $(nproc)

RUN sed -i "/^my \$PHASAR_BIN/s/=.*$/= '\/usr\/local\/bin\/phasar';/" environment-variable-tracing/Tools/run-analysis.pl && \
    sed -i "/^my \$PLUGIN/s/=.*$/= '\/opt\/build\/IFDSEnvironmentVariableTracing\/libIFDSEnvironmentVariableTracing.so';/" environment-variable-tracing/Tools/run-analysis.pl && \
    cp environment-variable-tracing/Tools/* /usr/local/bin

RUN apt-get -y install cpanminus python-pip

RUN cpanm Clone

RUN apt-get -y remove lcov && \
    wget https://github.com/linux-test-project/lcov/releases/download/v1.14/lcov-1.14.tar.gz && \
    tar xvfz lcov-1.14.tar.gz && \
    cd lcov-1.14 && \
    make install

RUN git clone https://github.com/travitch/whole-program-llvm && \
    cd whole-program-llvm && \
    pip2 install -e .

RUN echo "export LLVM_COMPILER=clang" >> ~/.bashrc && \
    echo "export LLVM_COMPILER_PATH=/usr/local/bin" >> ~/.bashrc

COPY ./docker/README /opt/

