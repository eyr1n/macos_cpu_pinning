# How to use CPU pinning on Apple Silicon

## Install the development kernel

### Create a custom boot object

```sh
KDK=/Library/Developer/KDKs/KDK_26.3.1_25D2128.kdk
KERNEL=$KDK/System/Library/Kernels/kernel.development.t8132
kmutil create \
  -n boot \
  -a arm64e \
  -V development \
  -B ~/boot.kc  \
  -k $KERNEL \
  -r /System/Library/Extensions/ \
  -r /System/Library/DriverExtensions \
  -x $(kmutil inspect -V release --no-header | awk '{print " -b "$1; }')
```

### Install the custom boot object

Boot into Recovery Mode

```sh
csrutil disable
kmutil configure-boot -v /Volumes/Macintosh\ HD -c /Volumes/Macintosh\ HD/Users/rin/boot.kc.development
reboot
```

### Add a boot argument

```sh
sudo nvram boot-args="enable_skstb=1"
```

### Reboot and check the running kernel

```sh
uname -v
sysctl kern.osbuildconfig
sysctl kern.sched_thread_bind_cpu
```

## Test

```sh
clang++ -DACCELERATE_NEW_LAPACK bind_cpu_id.cpp -framework Accelerate -o bind_cpu_id
sudo ./bind_cpu_id
```
