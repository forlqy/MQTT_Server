From ubuntu:20.04

WORKDIR /MQTTServer

RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    libmosquitto-dev \
    libjsoncpp-dev \
    libyaml-cpp-dev


copy . /MQTTServer

WORKDIR ./third_party/cpp_redis

RUN mkdir build

WORKDIR ./build

RUN cmake .. && make -j && make install

WORKDIR ../
RUN rm -r build


WORKDIR ../jsoncpp

RUN mkdir build

WORKDIR ./build

RUN cmake .. && make && make install

WORKDIR ../
RUN rm -r build


WORKDIR ../../build
RUN rm -r ./*
RUN cmake ..

RUN make -j

EXPOSE 5555

CMD ../bin/MQTTServer
