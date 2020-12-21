# Shortest_Path_Algorithms
 一个实现了多种最短路径搜索算法的代码库，包括主流的Dijkstra及其变种，以及多种最短路径索引算法
# 具体有哪些算法？
* 经典Dijkstra
* 使用堆优化的Dijkstra
* A* 算法 （使用欧式距离作为启发函数）

# 数据集要求与路网存储形式
数据集描述：路网数据集为文本文件（map_xxxx.txt），且为边集数组的形式进行存储，格式为：
```
节点A的ID     节点B的ID     A与B相连边的距离
```
举例如下：
```
0 1 0.002025
0 6 0.005952
1 2 0.01435
2 3 0.012279
3 4 0.011099
5 6 0.006157
5 7 0.001408
5 8 0.012008
7 265 0.003213
8 298 0.005382
9 10 0.01294
```

如果使用A* 算法，还需要路网的经纬度数据集文件（lonlat_xxx.txt），默认北纬，所以不用加N，格式如下：
```
节点ID 经度(longitude) 纬度(latitude)
```
举例如下：
```
0 -121.904167 41.974556
1 -121.902153 41.974766
2 -121.896790 41.988075
3 -121.889603 41.998032
4 -121.886681 42.008739
5 -121.915062 41.970314
6 -121.910088 41.973942
7 -121.916199 41.969482
8 -121.903198 41.968456
9 -122.553284 41.895241
10 -122.542992 41.903084
```
# 我要怎么在我的代码里面调用
## 项目结构
```
Shortest_Distance_Algorithms
│   main.cpp
│   CMakeLists.txt
│   README.md
│─ dataset
│   │ lonlat_california.txt
│   │ lonlat_maine.txt
│   │ map_california.txt
│   │ map_demo.txt
│   │ map_maine.txt
│─ algorithms
    │ graph.cpp
    │ graph.h
    │ Dijkstra.cpp
    │ Dijkstra.h
```
你需要将algorithm文件夹拷贝到你的项目main代码同一目录下。其中，graph.cpp和graph.h是读取路网文件的工具，以及路网数据结构的定义。
你可以选择
1. 将文件路径提供给void read_file(string file_path)函数，这个函数在graph.h中定义，在graph.cpp中实现。读取的路网将存储在全局变量unordered_map<unsigned int, set<Road> > graph中。然后使用算法类的构造函数传入graph来进行初始化。
2. 自己读取路网文件后将路网存在unordered_map<unsigned int, set<Road> > 类型的变量var中，然后使用该变量来初始化一个算法类实例。