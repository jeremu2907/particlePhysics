FROM ubuntu:latest

ENV HOME /root

RUN apt-get update && apt-get -y --no-install-recommends install \
    build-essential \ 
    cmake \
    wget \
    openssh-server

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

# RUN mkdir /run/sshd && \
#     ssh-keygen -A && \
#     sed -i "s/^.*PasswordAuthentication.*$/PasswordAuthentication no/" /etc/ssh/sshd_config && \
#     sed -i "s/^.*X11Forwarding.*$/X11Forwarding yes/" /etc/ssh/sshd_config && \
#     sed -i "s/^.*X11UseLocalhost.*$/X11UseLocalhost no/" /etc/ssh/sshd_config

# CMD ["/root/proj/particlePhysics"]