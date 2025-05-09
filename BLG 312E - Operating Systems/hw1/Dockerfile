FROM ubuntu:22.04

RUN apt update
RUN apt install openssh-server sudo -y
RUN useradd -rm -d /home/ubuntu -s /bin/bash -g root -G sudo -u 1000 test 
RUN usermod -aG sudo test
RUN service ssh start
RUN  echo 'test:test' | chpasswd

RUN apt update
RUN apt install build-essential -y
RUN apt install gdb -y

RUN apt update
RUN apt install valgrind -y

RUN apt update
RUN apt install git -y

RUN apt update
RUN apt install python3 -y
RUN apt install python3-pip -y
RUN sudo update-alternatives --install /usr/bin/pip pip /usr/bin/pip3 1

EXPOSE 22
CMD ["/usr/sbin/sshd","-D"]
