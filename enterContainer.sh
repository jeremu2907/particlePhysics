#!/bin/bash

docker run --rm --cidfile id \
       --name=engine \
       --net=host \
       -e DISPLAY=docker.for.mac.host.internal:0 \
       -v $(pwd):/root/proj \
       -v /tmp/.X11-unix:/tmp/.X11-unix \
       -v $HOME/.Xauthority:/root/.Xauthority:ro \
       -d jeremu2907/physics_engine:1 sleep infinity \
       # /usr/sbin/sshd
docker exec $(cat ./id) mkdir run/sshd
docker exec $(cat ./id) /usr/sbin/sshd
docker exec -it $(cat ./id) /bin/bash