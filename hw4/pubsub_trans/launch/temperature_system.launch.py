from launch import LaunchDescription
from launch_ros.actions import Node 

def generate_launch_description():
    return LaunchDescription([
        Node(
            package="pubsub_trans",
            executable="celsius_pub",
            name="celsius_pub"
        ),
        Node(
            package="pubsub_trans",
            executable="fahrenheit_sub",
            name="fahrenheit_sub",
            # 可选：绑定到特定CPU核心减少调度延迟
            ros_arguments=['--ros-args', '--log-level', 'info'],
            parameters=[{'use_sim_time': False}]
        )
    ])