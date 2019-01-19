FROM alpine:edge

RUN apk add --no-cache gcc yasm make xorriso mtools grub-bios
RUN mkdir /root/osdev

WORKDIR /root/osdev
CMD make all
