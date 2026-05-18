from launch_ros.substitutions import FindPackageShare
from launch_ros.actions import Node
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import PathJoinSubstitution

def generate_launch_description():
    return LaunchDescription([
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource([
                PathJoinSubstitution([
                    FindPackageShare('stonefish_ros2'),
                    'launch',
                    'stonefish_simulator.launch.py'
                ])
            ]),
            launch_arguments = {
                'simulation_data' : PathJoinSubstitution([FindPackageShare('stonefish_sim'), 'data']),
                'scenario_desc' : PathJoinSubstitution([FindPackageShare('stonefish_sim'), 'scenarios', 'boxy.scn']),
                'simulation_rate' : '300.0',
                'window_res_x' : '1200',
                'window_res_y' : '800',
                'rendering_quality' : 'high'
            }.items()
        ),
        Node(
            package='stonefish_sim',
            executable='tf_handler_node',
            name='stonefish_tf_handler',
            parameters=[
                {'world_frame': 'odom'},
                {'robot_id': 'boxy'},
                {'odom_topic': '/boxy/odometry'},
            ]
        ),
    ])