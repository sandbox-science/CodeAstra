#!/usr/bin/env bash

docker build -t "code-astra:tester" .
docker run --name "code-astra-tester" "code-astra:tester"
docker rm "code-astra-tester"
