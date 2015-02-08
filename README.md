# Damas
Checkers game coded in C, using ncurses lib

# Installing Ncurses

* CentOS/Fedora

```sh
# yum install ncurses-devel
```

* Debian based
```sh
# apt-get install libncurses5-dev
```

# Compile

```sh
$ git clone https://github.com/mmxxm/Damas.git
$ cd Damas
$ make
```

# Play
* Offline match

```
$ ./damas -o
```

* Connect to a server

```sh
$ ./damas -c <server_addr> -p <server_port>
``` 

* Be a server

```sh
$ ./damas -l <port-to-listen>
```

# Demo
[![Demo](http://img.youtube.com/vi/O4gsKmFGM20/0.jpg)](https://www.youtube.com/watch?v=O4gsKmFGM20)
