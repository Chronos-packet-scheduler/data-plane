FROM ubuntu:24.04 AS bpf
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update \
    && apt-get install --no-install-recommends -y clang llvm gcc libbpf-dev \
    && ln -s "$(gcc -print-multiarch)/asm" /usr/include/asm \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /src
COPY bpf/ bpf/
RUN mkdir -p /out && for f in bpf/*.c; do \
        clang -O2 -g -Wall -target bpf -c "$f" -o "/out/$(basename "${f%.c}").o"; \
    done

FROM rust:1-bookworm AS agent
WORKDIR /src
COPY Cargo.toml Cargo.lock* ./
COPY src/ src/
RUN cargo build --release

FROM gcr.io/distroless/cc-debian12
COPY --from=bpf /out/ /opt/chronos/bpf/
COPY --from=agent /src/target/release/chronos-agent /chronos-agent
ENTRYPOINT ["/chronos-agent"]
