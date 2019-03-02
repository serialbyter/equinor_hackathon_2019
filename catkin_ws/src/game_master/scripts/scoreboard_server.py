#!/usr/bin/env python
from __future__ import print_function

import os
import rospy
import requests
import pandas as pd

from std_srvs.srv import Empty, EmptyResponse

scorefile_path = os.path.join(os.path.dirname(__file__), '..')+"/scorefile1.txt"

def add_new_scores_to_db(req):

    #TODO team name service?
    team = "usr1"

    scores = pd.read_csv(scorefile_path, header=None,
                                names=["Timestamp", "Score", "Sent"])

    if not scores['Sent'].all():
        update = scores[scores['Sent']==0]['Score'].values.tolist()
        print(update)
        result = requests.post(
            'https://europe-west1-hackathon-af6d5.cloudfunctions.net/addScore',
                data = {'team':team, 'score':update})
    
        if result.status_code == 200:
            scores['Sent'].values[:] = 1
            scores.to_csv(scorefile_path, header=False, index=False)

    return EmptyResponse()

def main():
    #Init ROS node
    rospy.init_node('scoreboard_service')
    s = rospy.Service("/publish_scores", Empty, add_new_scores_to_db)
    rospy.spin()

if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        pass