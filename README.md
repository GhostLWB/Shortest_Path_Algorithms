# Shortest_Path_Algorithms
 一个实现了多种最短路径搜索算法的代码库，包括主流的Dijkstra及其变种，以及多种最短路径索引算法
# 具体有哪些算法？
* 经典Dijkstra
* 使用堆优化的Dijkstra
* A* 算法 （使用欧式距离作为启发函数）
* G Tree算法 （使用multi graph partition 作为子图划分解决方案）
* （我还在写 没写完 没时间搞这个啦）

# 算法性能如何
下图是在california路网（21,048个node）上随机进行100条最短路径查询的查询平均耗时.
![performance](https://jiantuku-liwenbin.oss-cn-shanghai.aliyuncs.com/%E6%9D%82/performance.jpg)
# 数据集要求与路网存储形式
数据集描述：路网数据集为文本文件（dataset/map_XXXX/edge.txt），且为边集数组的形式进行存储，格式为：
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

如果使用A* 算法，还需要路网的经纬度数据集文件（dataset/map_XXXX/lonlat.txt），默认北纬，所以不用加N，格式如下：
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
Shortest_Distance_Algorithms/
│   main.cpp  # 主函数入口
│   CMakeLists.txt # 如果你使用cmake进行编译，编辑它
│   README.md # 也就是本文件
|─  cbuild/ # 如果你用cmake进行编译，建议cbuild目录在这里
|   |
│─ dataset/ # 存放路网数据集
│   │─ map_california/
│   │   | category.txt # 节点的类型（POI）
│   │   | distance.txt # 预计算的点到点的最短距离，方便在多次构建同一个路网给G Tree的时候省去最短路径计算的过程
│   │   | edge.txt # 路网的边
│   │   | lonlat.txt # 路网的点的经纬度
│   │─ map_maine/
│   │   | category.txt
│   │   | distance.txt
│   │   | edge.txt
│   │   | lonlat.txt
│   │─ map_mississippi/
│   │   | category.txt
│   │   | distance.txt
│   │   | edge.txt
│   │   | lonlat.txt
│   │─ map_NA/
│   │   | category.txt
│   │   | distance.txt
│   │   | edge.txt
│   │   | lonlat.txt
│   │─ map_OL/
│   │   | category.txt
│   │   | distance.txt
│   │   | edge.txt
│   │   | lonlat.txt
│   │─ map_SF/
│   │   | category.txt
│   │   | distance.txt
│   │   | edge.txt
│   │   | lonlat.txt
│   │─ map_test/
│   │   | category.txt
│   │   | distance.txt
│   │   | edge.txt
│   │   | lonlat.txt
│   │─ map_TG/
│   │   | category.txt
│   │   | distance.txt
│   │   | edge.txt
│   │   | lonlat.txt
│─ algorithms/
    │ Graph.cpp
    │ Graph.h
    │ Dijkstra.cpp
    │ Dijkstra.h
    │ GraphPartition.h
    │ GraphPartition.cpp
    │ GTree.h
    │ GTree.cpp
    │ configure.h # 参数配置文件 主要存放当前使用的路网数据集的路径
    │ A_Star.h
    │ A_Star.cpp
```

你需要将algorithm文件夹拷贝到你的项目main代码同一目录下。其中，graph.cpp和graph.h是读取路网文件的工具，以及路网数据结构的定义。
你可以选择
1. 将路径文件放到dataset文件夹中，然后使用Graph类实例化路网对象，使用该对象实例化Dijkstra等算法类对象。
1. 将你自己的路网数据结构传入Dijkstra等算法类的构造函数中进行初始化，不过你需要将你的路网构建成Graph类，或者对Dijkstra等算法类做一些席细微的调整（这并不多，主要是获取邻接节点和路网大小等地方需要调整）
