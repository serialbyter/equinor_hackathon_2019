# Workflow

## Project setup

We recommend cloning the repository made in the `Getting Started` documentation to your own PC. This way you can develop code in your favorite IDE. Running the script locally will provide you with syntax errors. However, since we don't have time to setup ROS on your own computer, testing your code in the simulator must be done on the VM. 

To transfer your code:

1. Ensure that you have pushed the changes you have made on your laptop.
2. SSH into your VM. 
3. Pull the latest code

and you're ready to test your code.

For minor code fixes we recommend adjusting the code directly in the VM. All the VM's come with a command line text editor called nano. To open a file with nano, simply write `nano filename`. You can navigate through the file using arrow keys and to save press `Ctrl` + `X` and follow the prompts. Remember to push the changes you make on the VM!

## Advanced Tools

Some tools which we recommend if you have used them before.

### Tmux

Having multiple terminals can be a pain. `tmux` is installed on all the VM's and can be started by simply writting `tmux`. This allows you to split your terminal into multiple terminals. Instead of connecting to the VM via SSH each time you need a new terminal you can instead split the terminal you have in one command. A `tmux` cheatsheet [can be found here](https://gist.github.com/MohamedAlaa/2961058).
