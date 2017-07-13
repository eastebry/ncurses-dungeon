FROM ubuntu:xenial

RUN adduser --disabled-password --gecos '' ctf

RUN apt-get update && apt-get install -y xinetd gcc make libc6-dev-i386 libncurses5-dev

WORKDIR /home/ctf
ADD Makefile /home/ctf/Makefile
ADD *.h /home/ctf/
ADD *.c /home/ctf/
RUN make cast

ADD dungeon.xinetd /etc/xinetd.d/dungeon

USER root
EXPOSE 1338

CMD service xinetd restart && sleep infinity
