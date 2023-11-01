FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y gcc make git binutils libc6-dev python3.6 gdb vim
