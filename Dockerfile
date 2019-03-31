FROM alpine:edge

RUN apk add --no-cache gcc clang yasm make xorriso mtools syslinux
RUN mkdir /root/osdev

WORKDIR /root/osdev
CMD time make all
