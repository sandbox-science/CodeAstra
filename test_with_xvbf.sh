#!/bin/bash -e

# Start Xvfb server
Xvfb :99 -screen 0 1024x768x24 &
export DISPLAY=:99

# Run tests
make test
