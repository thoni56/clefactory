# Cgreen
wget https://github.com/cgreen-devs/cgreen/releases/download/1.6.1/cgreen-1.6.1-x86_64-linux-gnu.deb
sudo dpkg -i ./cgreen-1.6.1-x86_64-linux-gnu.deb
cgreen-runner --version
# Coverage
lcov --version
gcov --version
gem install coveralls-lcov
# libclang
wget https://github.com/llvm/llvm-project/releases/download/llvmorg-15.0.6/clang+llvm-15.0.6-x86_64-linux-gnu-ubuntu-18.04.tar.xz
tar xvf clang+llvm-15.0.6-x86_64-linux-gnu-ubuntu-18.04.tar.xz
mv clang+llvm-15.0.6-x86_64-linux-gnu-ubuntu-18.04 /opt
echo /opt/clang+llvm-15.0.6-x86_64-linux-gnu-ubuntu-18.04/lib > /etc/ld.so.conf.d/llvm-15.0.6.conf
ldconfig
