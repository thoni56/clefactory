## Code

TBD.

### Useful fragments and ideas

#### Tips for reading file modification times

(ChatGPT)

The fastest way to get the latest modification time for a large number
of files in C99 depends on the operating system and the number of
files being processed. However, some common techniques include:

- Using the stat() function: This function provides information about
  a file, including its modification time. You can call this function
  in a loop for each file, which can be time-consuming for a large
  number of files.

- Using the readdir() function: This function is used to read the
  contents of a directory. You can use it to get the list of files in
  a directory and then call stat() for each file to get its
  modification time. This approach can be faster than calling stat()
  for each file directly, but it is still relatively slow for a large
  number of files.

- Using the dirent.h header file: This header file provides a more
  optimized version of readdir() called readdir_r(). This function is
  reentrant and can be used in multi-threaded applications, making it
  a faster option for processing a large number of files.

- Using multithreading: You can use multithreading to split the
  processing of a large number of files into multiple threads. Each
  thread can process a portion of the files, which can significantly
  improve the processing speed.

Ultimately, the best approach will depend on the specific requirements
and constraints of your application, such as the number of files being
processed, the available resources, and the desired processing speed.
