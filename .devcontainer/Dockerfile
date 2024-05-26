FROM rockylinux:9
RUN dnf -y update && dnf clean all
RUN dnf install -y epel-release
RUN dnf install -y nodejs npm unzip git gcc rpm-build rpm-devel rpmlint make diffutils patch rpmdevtools gcc-c++ neovim
RUN git clone https://github.com/longlodw/nvim.git /root/.config/nvim && cd /root/.config/nvim && git checkout cpp
ARG USER
ARG UID
RUN useradd -m $USER -u $UID
RUN mkdir /home/$USER/.config && cp -r /root/.config/nvim /home/$USER/.config/nvim && chown -R $USER:$USER /home/$USER/.config
USER $USER
RUN rpmdev-setuptree
