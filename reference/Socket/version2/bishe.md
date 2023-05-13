## 毕设
2.1 Block Diagram and System Overview
小车Turtlebots3系统
眼镜Hololens2系统
Driver Control系统
PC server控制系统

需要购买的东西：
广角摄像头。
操纵方向盘系统。

完成的流程：
* 完成视频传输：（实现小车视频传输给server）
使用Socket UDP协议进行视频实时传输(720p)。
使用Socket和TCP完成可靠的Server Client连接。
使用EmguCV程序包获取采集到的摄像头图像。
由于UDP数据包为64kbit，使用jpeg进行图像压缩。
使用UDP进行发送。
*参考链接：*https://blog.csdn.net/qq_42688495/article/details/102565452
* 完成server控制小车：
使用
目标： Server接受操纵系统的信号，（可能进行处理，可能不处理）发送给OpenCR开发版。OpenCR开发版处理收到的信号，分析后使用嵌入式ROS操作系统控制小车电机运动。具体实现待定。
使用Arduino IDE进行开发。
**Checkpoint1:实现到这里，就完成了屏幕控制，可以通过看电脑屏幕控制小车运动。**
* 完成server视频投射到Hololens眼镜，进行视频图像的角度矫正。
* 使用Unity在Hololens上添加Vitual效果，例如车的运动信息（转速，速度，油量）。如果有雷达或者深度测量仪器的话，当深度小于安全值，在固定区域弹出警示标志。
**Checkpoint2:基本完成远程驾驶任务**
* 如果还能完成到这里，可以使用Unity使更加身临其境，仿佛真的在开车一般，还能传输周围的人的声音，进行实时交流。但基本是画饼。

下面是需要的部件的介绍（直接从之前的proposal上copy的，用chat改写一下）：
2.2.1 Hololens2 MR Glasses
The Hololens2 MR glasses is a wearable mixed-reality device provided by Microsoft. It is powered by the Snapdragon 850 computing platform and Holographic Processing Unit 2.0. The device supports tapping and zooming the operating system in the air, enabling HoloLens to achieve significant performance improvements with low power consumption. With HoloLens 2, Microsoft has updated the way users interact with holograms. Using a new TOF depth sensor, combined with built-in AI and semantic understanding, HoloLens 2 allows users to directly manipulate holograms using the same gestures they use to interact with real-world objects.

2.3 Development Module
The development module is used to program the development board and the VR platform on the PC side and synthesize the information obtained from each hardware side to render scenes or make virtual responses. The development module is mainly a variety of software on the PC side, mainly used for program development and information processing.//纯jb废话
2.3.1 Arduino IDE
Arduino IDE is an open-source programming and development tool based on Processing IDE, with strong compatibility and support for running on mainstream operating systems. Arduino IDE software supports Nano, Mega, Esplora, Ethernet, Fio and many other development boards. In our design, Arduino IDE is mainly used for programming development boards such as Open CR on the cart.
2.3.2 Unity AR Processing Graphical User Interface (GUI)
Unity3D is currently one of the most popular development engines, supporting full cross-platform game development, including PC, mobile Android, iOS, console PS4, Switch and handheld PSV, 3DS, etc. For 3D development, Unity provides a more mature development platform. Several AR plug-ins in Unity support mainstream AR/MR platforms, which are suitable development tools. We mainly use our project for the Hololens2 part of the development.
2.3.3 ROS and Control GUI
Robot Operating System (ROS) is an open-source software development kit for robotics applications. ROS provides a standard software platform for developers in various industries, taking them from research and prototyping to deployment and production. In our projects, ROS is primarily used to control the movements of mechanical platforms.

2.3.4 PC
The PC terminal is the core of the development module, the running platform of each development software. It undertakes the integrated processing of data flow and control flow and the data communication at the hardware end, which is the pivot of the whole system operation. We will utilize the station of our laboratory.

2.4.1 TurtleBot 3 robot
TurtleBot3 is an official robot hardware platform built by ROS. It utilizes an Open CR board as its motherboard and has a 1600mAh rechargeable battery. This board supports Arduino IDE, which is very familiar to us. The main reasons that encourage us to choose this robot are its two advantages. Firstly, the robot is very extensible, so we can easily add components designed as mechanical parts to the robot and control it with a program. Secondly, the robot has a mature ecosystem, with many learning resources and high-quality codes available on the open-source platform, which can help us design and simulate the program more efficiently.
2.4.2 Camera
我们需要360度全景的摄像头来记录Turtlebot3周围的环境，在Unity中需要加入镜头的矫正来更好完成环境的还原。
2.4.3 Sensors
需要传感器记录车子的信息，包括：小车速度，小车电量，小车转速。
需要深度传感器实时监测小车距离周围的最近距离。


一些奇怪的方面：
后视镜的实现不是很清楚怎么实现合适？？？




\subsection{Block Diagram}
Break your design down into blocks and assign these blocks into subsystems.
\subsection{System Overview}
在这个项目中，我们将制造远程的驾驶系统。驾驶者将带着Hololens2眼镜，通过驾驶新系统远程驾驶小车。小车的摄像头将小车周围的环境信息传输给
PC端，PC将视频信息同步到Hololens2眼前。驾驶者通过驾驶系统，通过PC向小车控制板发送控制自信信号。小车分析控制信号通过OpenCR控制板控制
小车运动。
\subsection{Subsystem Requirements}

小车Turtlebots3系统
小车系统需要完成视频传输给Server。由于驾驶需要视频有较低的延时，UDP协议比TCP协议更适合我们的项目。我们计划使用Socket和UDP协议进行视频实时传输，目前计划使用720p的分辨率。
使用Socket和TCP完成可靠的Server，Client连接。使用OpenCV或者EmguCV库来获取采集的摄像头图像。并且，
由于UDP数据包为64kbit，我们打算使用jpeg进行图像压缩。在最后使用UDP协议进行发送。

PC server控制系统
PC server系统承担着最重要的信息交互功能。作为信号控制的桥梁。需要将Turtlebots3发送的视频信息解码，并且投射到Hololens2上。需要将
Driver Control系统的信号发送给Turtots系统，来控制小车。

眼镜Hololens2系统
Hololens2系统承载着驾驶者能获取的所有学习。Server将传输的视频信号投射在Hololens2上，这使得驾驶者能多角度地看到小车周围的环境。通过
Unity平台，可以讲小车的运行信息投射在Hololens2上来给驾驶者参考。

Driver Control系统
Driver Control是驾驶者的的控制系统，应该包含方向盘，加速器，刹车等部分。驾驶者通过使用相应控制部分，向PC Server发送控制信息，实现远程控制小车。

\subsection{Tolerance Analysis}
