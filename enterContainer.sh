#!/bin/bash

docker run --cidfile id -d engine:1 sleep infinity

docker exec -it $(cat ./id) /bin/bash