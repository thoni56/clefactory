wget https://github.com/cgreen-devs/cgreen/releases/download/1.6.1/cgreen-1.6.1-amd64.deb
sudo dpkg -i ./cgreen-1.6.1-amd64.deb
cgreen-runner --version
find /usr -name cgreen.h
lcov --version
gcov --version
gem install coveralls-lcov
