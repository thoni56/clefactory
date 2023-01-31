wget https://github.com/cgreen-devs/cgreen/releases/download/1.6.0/cgreen-1.6.0_amd64.deb
sudo apt install ./cgreen-1.6.0_amd64.deb
cgreen-runner --version
find /usr -name cgreen.h
lcov --version
gcov --version
gem install coveralls-lcov
