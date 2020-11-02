FROM debian:sid

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y clang yasm make cmake ninja-build genisoimage mtools syslinux isolinux
RUN mkdir /root/osdev

WORKDIR /root/osdev
#CMD time ninja
CMD bash -c 'time make all'
