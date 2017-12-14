# splunk-hec-client-cpp
Scaled Splunk HEC Client in morden C++ which supports 

1. HEC /raw endpoint
   * with ack enabled
   * without ack enabled
2. HEC /event endpoint
   * with ack enabled
   * without ack enable

# C++ version
C++14
 
# Direct dependent libs
1. [cpprestsdk](#https://github.com/Microsoft/cpprestsdk) for http client
2. [rapidjson](#https://github.com/Tencent/rapidjson) for JSON marshalling and unmarshalling
3. [spdlog](#https://github.com/gabime/spdlog) for internallogging
4. [boost](#http://www.boost.org/) for boost::any and boost::uuid

# Build
## Prerequistives

1. cmake
2. make
3. gcc 5.0 or clang 3.4
4. git 1.9 above


## Install dependencies 
### MacOs
Run the following command to install dependencies. Homebrew is a prerequistive.

```
# bash install_dep_libs-mac.sh
```

### Ubuntu Linux 
Run the following command to install dependencies

```
# bash install_dep_libs-ubuntu.sh
```

### Centos Linux 
Run the following command to install dependencies

```
# bash install_dep_libs-centos.sh
```

### Windows

Run the following command to install dependencies

```
# tbd
```

## Build

```
# git clone --recursive -j8 git@github.com:chenziliang/splunk-hec-client-cpp.git
# mkdir build && cd build && cmake ..
```

 


