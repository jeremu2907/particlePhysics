FROM gcc:latest

COPY . /usr/src/engine
COPY /Library/Frameworks/SDL2.framework /Library/Frameworks

WORKDIR /usr/src/engine

RUN
