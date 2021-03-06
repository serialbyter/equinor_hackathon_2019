#!/usr/bin/env python
from __future__ import print_function

import os
import rospy
import requests
import pandas as pd

from std_srvs.srv import Empty, EmptyResponse, Trigger, TriggerRequest

scorefile_path = os.path.join(os.path.dirname(__file__), '..')+"/scorefile.txt"
team = "default-usr1"

def add_new_scores_to_db(req):
    scores = pd.read_csv(scorefile_path, header=None,
                                names=["Timestamp", "Map", "Score", "Sent"])

    if not scores['Sent'].all():
        update = scores[scores['Sent']==0]
        for map_name in update['Map'].unique():

            new_scores = update[update['Map']==map_name]['Score'].values.tolist()
            for score in new_scores:
                result = requests.post( 
                    'https://europe-west1-hackathon-af6d5.cloudfunctions.net/addScore',
                        data = {'Map':map_name, 'Team':team, 'Score':score})

                if result.status_code != 200:
                    return EmptyResponse()
            

    if result.status_code == 200:
        scores['Sent'].values[:] = 1
        scores.to_csv(scorefile_path, header=False, index=False)

    return EmptyResponse()

def main():
    global team
    #Init ROS node
    rospy.init_node('scoreboard_service')
    rospy.wait_for_service("/teamname")
    teamname_service = rospy.ServiceProxy("/teamname", Trigger)
    while True:
        res = teamname_service(TriggerRequest())
        if res.success:
            team = res.message
            break
    
    rospy.loginfo("Scoreboard server ready for team: " + team)

    s = rospy.Service("/publish_scores", Empty, add_new_scores_to_db)
    rospy.spin()

if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass
