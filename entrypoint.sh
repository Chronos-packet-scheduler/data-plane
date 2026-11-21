#!/bin/sh
# Mount the filesystems needed to load BPF programs and read bpf_printk output.
# Requires --privileged; without it the mounts are skipped with a warning.

mount_fs() {
    type=$1
    target=$2
    mountpoint -q "$target" && return
    mount -t "$type" "$type" "$target" 2>/dev/null \
        || echo "warning: could not mount $type at $target (run with --privileged to load programs)" >&2
}

mount_fs bpf /sys/fs/bpf
mount_fs tracefs /sys/kernel/tracing

exec "$@"
