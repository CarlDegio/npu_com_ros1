//
// Created by npu on 2022/7/2.
//

#ifndef NPU_COM_ROS1_PKG_HPP
#define NPU_COM_ROS1_PKG_HPP

struct Tcp_Pkg
{
    double posx=0;
    double posy=15;
    double posz=15;
    double pitch=0;
    double timelen=3000;
    short closepwm=200;
    short betapwm=500;
    int magic_num=997;
};
#endif //NPU_COM_ROS1_PKG_HPP

