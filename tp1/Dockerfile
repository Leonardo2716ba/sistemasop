FROM ubuntu:latest

RUN apt-get update && apt-get upgrade -y

RUN apt-get install -y gcc vim nano

RUN grep --version || apt-get install -y grep

RUN mkdir /leonardo

WORKDIR /leonardo

RUN echo "Leonardo de Souza Gomes - 21.2.4054 \nTurma 11" > LEONARDO.TXT

RUN mkdir /leonardo/PGR1

RUN mkdir /leonardo/PGR2
