#include <netinet/in.h>
#include <arpa/inet.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int16MultiArray.h"
#include "std_msgs/Float64MultiArray.h"
#include "pkg.hpp"


int main(int argc, char **argv)
{
    ros::init(argc, argv, "tcp2topic");
    ros::NodeHandle n;
    ros::Publisher pose_publisher = n.advertise<std_msgs::Float64MultiArray>("/arm_close_gamma", 10);
    ros::Publisher PWM_publisher = n.advertise<std_msgs::Int16MultiArray>("/close_gamma", 10);
    int magic_num=997;
    char buf_recv[1024]={};
    Tcp_Pkg *charpkg;
    sockaddr_in server_addr;
    int sock;

    memset(&server_addr, 0, sizeof(server_addr));  //每个字节都用0填充
    server_addr.sin_family = AF_INET;  //使用IPv4地址
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    server_addr.sin_port = htons(10000);  //端口

    if((sock= socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        ROS_ERROR("create socket error!");
        return 0;
    }

    if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)
    {
        ROS_ERROR("connect fail! open server first");
        return 0;
    }
    ROS_INFO("get server successful!");
    std_msgs::Int16MultiArray PWM_msg;
    std_msgs::Float64MultiArray pose_msg;
    while (ros::ok())
    {
        read(sock, buf_recv, sizeof(buf_recv));
        charpkg=(Tcp_Pkg *)buf_recv;
        if(charpkg->magic_num!=magic_num)
        {
            ROS_WARN("error message package! check fail");
        }
        else
        {
            pose_msg.data={charpkg->posx,charpkg->posy,charpkg->posz,charpkg->pitch,charpkg->timelen};
            PWM_msg.data={charpkg->closepwm,charpkg->betapwm};
            pose_publisher.publish(pose_msg);
            PWM_publisher.publish(PWM_msg);
        }
        ros::spinOnce();//use ctrl z to quit
        //loop_rate.sleep();
    }
    close(sock);
    return 0;
}