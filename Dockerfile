FROM alpine:edge

RUN apk add --no-cache gcc yasm make xorriso mtools grub-bios grub-efi
RUN mkdir /root/osdev

WORKDIR /root/osdev
CMD time make all
