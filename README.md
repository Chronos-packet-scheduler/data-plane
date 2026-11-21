# data-plane

eBPF/XDP examples, built and run inside a Linux container.

## Start the environment

```sh
docker compose up -d --build     # build the image and start the container
docker compose exec dev bash     # open a shell (repeat in more terminals as needed)
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
