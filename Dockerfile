FROM ubuntu:22.04 AS builder
WORKDIR /

# Add dependencies
RUN apt-get update
RUN apt-get install -y --no-install-recommends \
    git \
    ca-certificates \
    g++ \
    cmake \
    make \
    mesa-common-dev \
    qt6-base-dev \
    x11-utils \
    x11-xserver-utils \
    xvfb
RUN rm -rf /var/lib/apt/lists/*

# Copy CodeAstra into the container
COPY . /CodeAstra
WORKDIR /CodeAstra

# Run dockerized tests script
RUN chmod +x ./run_dockerized_tests.sh

CMD ["./run_dockerized_tests.sh"]
