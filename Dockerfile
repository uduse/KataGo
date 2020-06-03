FROM nvidia/cuda:10.2-cudnn7-devel
RUN apt-get update && \
    apt-get -y install git gcc wget unzip zlib1g-dev libzip-dev \
    libboost-filesystem-dev ocl-icd-opencl-dev build-essential default-jre gconf2

# initialize directories
RUN git clone https://github.com/uduse/KataGo.git /KataGo
RUN git clone https://github.com/uduse/KataGo.git /KataGo_orig
RUN git clone https://github.com/uduse/gogui-twogtp-tournaments-setup /tournaments
RUN git clone https://github.com/Remi-Coulom/gogui.git /gogui

# katago somehow only works with this newer version of cmake
RUN wget https://github.com/Kitware/CMake/releases/download/v3.15.2/cmake-3.15.2.tar.gz \
    && tar -zxvf cmake-3.15.2.tar.gz \
    && cd cmake-3.15.2 \
    && ./bootstrap \
    && make -j$(nproc) \
    && make install

# KataGo with MMCTS implementation
WORKDIR /KataGo/cpp/
RUN git checkout mmcts/base
RUN cmake . -DBUILD_MCTS=1 -DUSE_BACKEND=CUDA && make -j$(nproc)

# KataGo without modifications
WORKDIR /KataGo_orig/cpp/
RUN git checkout master
RUN cmake . -DBUILD_MCTS=1 -DUSE_BACKEND=CUDA && make -j$(nproc)

# gogui setup
WORKDIR /gogui/
RUN sed 's/sudo apt install/apt-get install -y/g' ubuntu_setup.sh > temp.sh && sh temp.sh

WORKDIR /tournaments
CMD sh run.sh
