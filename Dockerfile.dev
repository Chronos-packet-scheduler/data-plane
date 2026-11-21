FROM ubuntu:24.04

WORKDIR /workspace

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install --no-install-recommends -y \
        clang \
        gcc \
        llvm \
        libbpf-dev \
        libxdp-dev \
        linux-headers-generic \
        linux-tools-generic \
        iproute2 \
        iputils-ping \
        make \
        pkg-config \
        xdp-tools \
    && ln -s "$(gcc -print-multiarch)/asm" /usr/include/asm \
    && rm -rf /var/lib/apt/lists/*

COPY entrypoint.sh /usr/local/bin/entrypoint.sh

ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]
CMD ["/bin/bash"]
