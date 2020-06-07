FROM nvidia/cuda:10.2-cudnn7-devel
RUN apt-get update \
    && apt-get -y install vim git gcc wget unzip npm zlib1g-dev libzip-dev \
    libboost-filesystem-dev ocl-icd-opencl-dev build-essential default-jre gconf2
RUN npm install -g gnomon

# katago somehow only works with this newer version of cmake
RUN wget https://github.com/Kitware/CMake/releases/download/v3.15.2/cmake-3.15.2.tar.gz \
    && tar -zxf cmake-3.15.2.tar.gz \
    && cd cmake-3.15.2 \
    && ./bootstrap > /dev/null 2>&1 \
    && make -j$(nproc) > /dev/null 2>&1 \
    && make install > /dev/null 2>&1

# initialize directories
COPY . /KataGo
COPY .git /KataGo/.git
COPY . /KataGo_orig
COPY .git /KataGo_orig/.git
RUN git clone https://github.com/uduse/gogui-twogtp-tournaments-setup /tournaments
RUN git clone https://github.com/Remi-Coulom/gogui.git /gogui

# KataGo with MMCTS implementation
WORKDIR /KataGo/cpp/
RUN cmake . -DBUILD_MCTS=1 -DUSE_BACKEND=CUDA && make -j$(nproc)

# KataGo without modifications
WORKDIR /KataGo_orig/cpp/
RUN git fetch && git checkout master
RUN cmake . -DBUILD_MCTS=1 -DUSE_BACKEND=CUDA && make -j$(nproc)

# gogui setup
WORKDIR /gogui/
RUN sed 's/sudo apt install/apt-get install -y/g' ubuntu_setup.sh > temp.sh \
    && sh temp.sh \
    && ./install.sh

WORKDIR /tournaments
