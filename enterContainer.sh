#!/bin/bash

docker run --rm --cidfile id \
       -e DISPLAY=host.docker.internal:0 \
       --net host \
       -v $(pwd):/root/proj \
       -v /tmp/.X11-unix:/tmp/.X11-unix \
       -d engine:1 sleep infinity \

docker exec -it $(cat ./id) /bin/bash