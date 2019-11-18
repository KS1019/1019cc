FROM ubuntu:latest

RUN apt-get update && apt-get install -y gcc make git binutils libc6-dev python gdb
