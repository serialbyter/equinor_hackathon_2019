# How to use the simulator

## Step 1: Access Azure Server

TODO


## Step 2: Open a SSH terminal to your server

TODO

get IP address or DNS name from azure. Use
```bash
ssh techathon@<ip/dns>
```


## Step 3: Start the simulator

First we need to make sure all modules are compiled. This needs to be done once since we have several c-plus-plus modules. The following commands will compile the simulator. 
```bash
cd catkin_ws
catkin_make
```

After this is done, we need to set some environment variables. This is done by running the following command from inside `catkin_ws`. 
```bash
source devel/setup.bash
```

The simulator is started through roslaunch. Run the following command

``` bash
roslaunch techathon main.launch
```
This will start the physics engine, set up sensor emulation, load in the drone models and start a web server with which we can view it. 


## Step 4: Access the simulation viewport in a web browser

In order to access the viewport we need either the public IP of the server or a domain name which resolves to it. On the servers page in Azure we can get both. The simulator runs on port 8080, and to view it we open a web browser and go to the link
`http://<IP address/application domain name here>:8080`.

For instance if we use the domain name `techathon.northeurope.cloudapp.azure.com`, we would open
![http://techathon.northeurope.cloudapp.azure.com:8080](http://techathon.northeurope.cloudapp.azure.com:8080). 


When the page has loaded (it takes a few seconds), you should see a 3D rendered world where you can move around.
![Gazebo Web](resources/gzweb.jpg)

## Step 5: Running your own program

Assuming your program is launched by running `main.py` in the `techathon` package, then it would be launched by running
```bash
rosrun techathon main.py
```
You should run you own program in a separate terminal window as this gives the greatest level of access to debug prints. This can either be done by starting a new SSH session for every terminal you need, or by using tmux (**recommended**). 

The techathon packag has few example programs to get you started.
Look at ![examples](examples.md) and try them out!

## Step 6: Stopping the simulator when you're done

The simulator can be stopped like any other foreground linux process by `Ctrl-C` on your keyboard. 
