# data-plane

eBPF/XDP programs written in C (`bpf/`).

- `Dockerfile` builds the production image (`ghcr.io/chronos-packet-scheduler/data-plane`), published on every push to `main` and on `v*` tags.
- `Dockerfile.dev` + `compose.yaml` give a privileged dev container for experimenting with `examples/`.

## Start the environment

```sh
docker compose up -d --build
docker compose exec dev bash
```

## Build the agent

The agent only builds on Linux, so build it inside the dev container:

```sh
docker compose exec dev bash
cd /workspace
cargo build
```

## Build & test

Terminal 1 (inside the container):

```sh
clang -O2 -g -Wall -target bpf -c example.c -o example.o   # compile
ip link set lo xdpgeneric obj example.o sec xdp_pass        # attach to lo
cat /sys/kernel/tracing/trace_pipe                          # watch bpf_printk output
```

Terminal 2 (on the host), generate traffic on `lo`:

```sh
docker compose exec dev ping -c 3 127.0.0.1
```

Detach and stop:

```sh
ip link set lo xdpgeneric off    # inside the container
docker compose down              # on the host
```