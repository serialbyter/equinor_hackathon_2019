#!/usr/bin/env python

from __future__ import print_function

import time

import rospy
from mavros_msgs.msg import PositionTarget, State, ParamValue
from mavros_msgs.srv import CommandBool, SetMode, ParamSet
from geometry_msgs.msg import PoseStamped
from std_msgs.msg import Header

pt = PositionTarget()
_DEFAULT = pt.IGNORE_VX | pt.IGNORE_VY | pt.IGNORE_VZ | pt.IGNORE_AFX | pt.IGNORE_AFY | pt.IGNORE_AFZ | pt.IGNORE_YAW_RATE
_TAKEOFF = 0x1000
_LAND = 0x2000
_LOITER = 0x3000
_IDLE = 0x4000
del pt

class Drone(object):
    def __init__(self):
        # Communication variables
        self._last_pose = PoseStamped()
        self._setpoint_msg = PositionTarget()
        self._setpoint_msg.type_mask = _DEFAULT
        self._last_state = State()

        # ROS Communication
        self._setpoint_pub = rospy.Publisher("mavros/setpoint_raw/local",
                                   PositionTarget, queue_size=1)
        self._pose_sub = rospy.Subscriber("mavros/local_position/pose",
                                    PoseStamped, self._pose_cb)

        self._state_sub = rospy.Subscriber('mavros/state',
                                           State, self._state_cb)


        # wait for mavros to start publishing
        rospy.logdebug("Waiting for MAVROS to start")
        rospy.wait_for_message("mavros/local_position/pose", PoseStamped)

        # Make drone less aggressive
        rospy.wait_for_service("mavros/param/set")
        mavparam = rospy.ServiceProxy('mavros/param/set', ParamSet)
        mavparam("MC_PITCH_P", ParamValue(0, 2.0)).success
        mavparam("MC_ROLL_P", ParamValue(0, 2.0)).success
        mavparam("MPC_VEL_XY_MAX", ParamValue(0, 4.0)).success

        # Send a few setpoint to make MAVROS happy
        rate = rospy.Rate(20.0)
        for _ in range(40):
            self._publish_setpoint(None)
            rate.sleep()


        rospy.Timer(rospy.Duration(0.05), self._publish_setpoint)
        rospy.loginfo("Drone initialized")


    # ----- API ------
    def activate(self):
        """
        Arm the drone and place into offboard.
        """
        arming = rospy.ServiceProxy('mavros/cmd/arming', CommandBool)
        offboard = rospy.ServiceProxy('mavros/set_mode', SetMode)
        rate = rospy.Rate(1)
        while not rospy.is_shutdown():
            arming(True)
            offboard(0, "OFFBOARD")
            if self._last_state.mode == "OFFBOARD" and self._last_state.armed:
                break

            rate.sleep()

    def takeoff(self):
        """
        Send takeoff setpoints at current position.
        """
        self._setpoint_msg.position.x = self._last_pose.pose.position.x
        self._setpoint_msg.position.y = self._last_pose.pose.position.y
        self._setpoint_msg.position.z = 5.0
        self._setpoint_msg.type_mask = _DEFAULT #| _TAKEOFF

    def set_target(self, x, y, yaw):
        """
        Set the position and yaw target.
        """
        self._setpoint_msg.position.x = x
        self._setpoint_msg.position.y = y
        self._setpoint_msg.yaw = yaw
        self._setpoint_msg.type_mask = _DEFAULT

    def get_pose(self):
        """
        Return pose as geometry_msgs/Pose
        """
        return self._last_pose.pose


    # --- Internal functions -----
    def _pose_cb(self, posestamped):
        self._last_pose = posestamped

    def _state_cb(self, state):
        self._last_state = state

    def _publish_setpoint(self, event):
        self._setpoint_msg.header.stamp = rospy.Time.now()
        self._setpoint_msg.header.frame_id = "map"
        self._setpoint_pub.publish(self._setpoint_msg)


