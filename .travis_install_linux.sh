# Cgreen
wget https://github.com/cgreen-devs/cgreen/releases/download/1.6.1/cgreen-1.6.1-x86_64-linux-gnu.deb
sudo dpkg -i ./cgreen-1.6.1-x86_64-linux-gnu.deb
cgreen-runner --version
# Coverage
lcov --version
gcov --version
gem install coveralls-lcov
# libclang
#wget https://github.com/llvm/llvm-project/releases/download/llvmorg-15.0.6/clang+llvm-15.0.6-x86_64-linux-gnu-ubuntu-18.04.tar.xz
#tar xf clang+llvm-15.0.6-x86_64-linux-gnu-ubuntu-18.04.tar.xz
#mv clang+llvm-15.0.6-x86_64-linux-gnu-ubuntu-18.04 /opt
#ls /opt
apt install libclang-15-dev
