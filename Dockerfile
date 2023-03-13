FROM ubuntu:latest

ENV HOME /root

RUN apt-get update && apt-get -y --no-install-recommends install \
    build-essential \ 
    cmake \
    wget

#Downloading and building SDL2
RUN cd ${HOME} && \
    wget --no-check-certificate -v \
        https://github.com/libsdl-org/SDL/releases/download/release-2.26.4/SDL2-2.26.4.tar.gz && \
    tar -xf SDL2-2.26.4.tar.gz && \
    cd SDL2-2.26.4 && \
    ./configure && \
    make && \
    make install

RUN cd ${HOME} && \
    mkdir proj

COPY . ${HOME}/proj

RUN cd ${HOME}/proj && \
    cmake . && \
    make

CMD ["/root/proj/particlePhysics"]