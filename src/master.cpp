#include <master.h>
#include <localization.h>
#include <control.h>
#include <memory>

#include "ros/ros.h"
#include "tf/transform_broadcaster.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"

#include "scout_msgs/ScoutStatus.h"

unique_ptr<Local> local(new Local());
unique_ptr<Control> control(new Control());
unique_ptr<Scout> scout(new Scout());

void ScoutCallback(const scout_msgs::ScoutStatus::ConstPtr& msg){
    scout->setAngVel(msg->angular_velocity);    
}

void LiDARPoseCallback(const nav_msgs::Odometry::ConstPtr& msg){ // LiDAR odometry
    // set scout's position info
    scout->setPosition_x(msg->pose.pose.position.x);
    scout->setPosition_y(msg->pose.pose.position.y);
    // set scout's yaw info
    tf::Quaternion q(msg->pose.pose.orientation.x, msg->pose.pose.orientation.y, msg->pose.pose.orientation.z, msg->pose.pose.orientation.w );
    tf::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);
    yaw = (yaw * (180 / M_PI));
    double tmp = fmod(yaw, 360.0);
    if (tmp < 0) scout->setHeading(tmp + 360);
    else scout->setHeading(tmp);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "master_node");
    ros::NodeHandle nh;
    geometry_msgs::Twist ctrl_cmd_msg;
    ros::Publisher ctrl_cmd_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    ros::Subscriber scout_sub = nh.subscribe("/scout_status", 10, ScoutCallback);
    ros::Subscriber lidar_pose_sub = nh.subscribe("/Odometry", 10, LiDARPoseCallback);
    vector<vector<double>> globalPath = local->run();

    while (ros::ok())
    {   
        local->findClosestWaypoint(scout.get());
        cout << "master gp : " << globalPath.size() << endl;
        double angvel = control->run(globalPath, scout.get());
        ctrl_cmd_msg.linear.x = 0.8;
        ctrl_cmd_msg.angular.z = angvel;
        cout << "master ang vel : " << angvel << endl;
        ctrl_cmd_pub.publish(ctrl_cmd_msg);
        ros::spinOnce();
    }
    
    return 0;
}