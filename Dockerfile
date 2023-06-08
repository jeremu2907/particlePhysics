FROM ubuntu:latest

ENV HOME /root

RUN apt-get update && apt-get -y --no-install-recommends install \
    build-essential \ 
    cmake \
    wget \
#     openssh-server \
#     vim \
#     iproute2

#Downloading and building SDL2
# RUN cd ${HOME} && \
#     wget --no-check-certificate -v \
#         https://github.com/libsdl-org/SDL/releases/download/release-2.26.4/SDL2-2.26.4.tar.gz && \
#     tar -xf SDL2-2.26.4.tar.gz && \
#     cd SDL2-2.26.4 && \
#     ./configure && \
#     make && \
#     make install
#install sdl2
RUN apt install libsdl2-dev libsdl2-2.0-0 -y

RUN cd ${HOME} && \
    mkdir proj
COPY . ${HOME}/proj
RUN cd ${HOME}/proj && \
    make

# RUN mkdir /run/sshd && \
#     ssh-keygen -A && \
#     sed -i "s/^.*PasswordAuthentication.*$/PasswordAuthentication no/" /etc/ssh/sshd_config && \
#     sed -i "s/^.*X11Forwarding.*$/X11Forwarding yes/" /etc/ssh/sshd_config && \
#     sed -i "s/^.*X11UseLocalhost.*$/X11UseLocalhost no/" /etc/ssh/sshd_config

# CMD ["/root/proj/particlePhysics"]
