wget https://github.com/cgreen-devs/cgreen/releases/download/1.6.1/cgreen-1.6.1-x86_64-linux-gnu.deb
sudo dpkg -i ./cgreen-1.6.1-x86_64-linux-gnu.deb
cgreen-runner --version
lcov --version
gcov --version
gem install coveralls-lcov
