# MQTT_Server 环境以及依赖

## 安装mosquitto库以及yaml-cpp库
```
apt-get update && apt-get install -y \
    g++ \
    cmake \
    libmosquitto-dev \
    libyaml-cpp-dev
```

## 在本机安装redis-server
```
apt-get install -y redis-server
apt-get install -y redis-tools
```
安装完毕后`redis-server --daemonize yes`在后台启动redis-server默认端口6379

## 手动安装jsoncpp以及cpp_redis库
```
cd third_party/jsoncpp
mkdir build
cd build
cmake .. && make 
make install
```

```
cd third_party/cpp_redis
mkdir build
cd build
cmake .. && make -j
make install
```


## 编译运行MQTTServer
```
cd MQTT_Server/build
cmake .. && make
cd ../bin
./MQTTServer
```

这时会显示传感器数据并出现设备uuid命名的txt文档；运行`redis-cli`，输入命令`keys *`然后根据设备uuid查看最新数据`get xxx`