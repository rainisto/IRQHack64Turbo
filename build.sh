#!/bin/bash
# give --no-cache as parameter if you want to force rebuild
BASE=$(pwd)
docker build $1 -t irqhack64 .
docker run -v $BASE/output:/flash -t irqhack64
