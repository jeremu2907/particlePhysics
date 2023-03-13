#!/bin/bash

docker run --rm --cidfile id \
       -v $(pwd):/root/proj \
       -d engine:1 sleep infinity
    #    -e DISPLAY=$DISPLAY
    #    -v engine:1 sleep infinity

docker exec -it $(cat ./id) /bin/bash