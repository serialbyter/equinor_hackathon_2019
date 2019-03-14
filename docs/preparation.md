# Prerequisites and preparations
All team members are expected to bring a computer with some software prepared in advance.
Some skills will also be necessary in order to complete the challenge, but not all
members of a team need to have all skills. 

## Computer setup
The following section describes what your computer needs to run the simulator and interact with it. The simulation environment is cloud based so you do not need a powerful computer or many specialized tools. 

In order to participate you need the following:
- Git
- SSH
- Network access
- VPN client
- Modern web browser (Firefox or Chrome recommended)
- Github account

Below we describe how to get Git and and SSH on the different platforms. 

### Windows

To get Git for windows, go to the [official download page](https://git-scm.com/downloads) and follow the installation guide.
For ssh we recommend using PuTTY wich can be installed from the [official website](https://www.putty.org/).

### Mac OS

If you don't have Git, simply run `git status` in a terminal and you will get an install wizard for setting up git. Installing SSH shouldn't be necessary.

### Linux

On most computers, Git and SSH will already by enabled and set up.
To verify that they are installed, open up a terminal window and run the following commands.
```
ssh
git --version
```
If you get `command not found` when running `ssh`, run the command `sudo apt install openssh-client`.
If you get `command not found` when running `git --version`, run the command `sudo apt install git`. 

On Red-Hat/CentOS use `yum` instead of apt in the above commands.

## Necessary skills
The challenge does not require very advanced programming skills, but some skills
will be needed in order to solve the challenge:
- Basic Git skills: Fork/clone a repo, commit, push. Check out the [simple Git guide](http://git.huit.harvard.edu/guide/) for an overview if you're unfamiliar. Note the guide contains alot more than is needed.
- Basic Python programming skills: Know python syntax, be able to write simple programs and run them. Note that the simulator requires __Python 2.7__. 
- Be able to use of SSH and connect to a remote computer using the terminal. This is explained in the [getting started guide](getting_started.md). 
- Basic shell skills. Open a terminal window, navigate in the file system, run commands. We will be using a server running Linux with only commandline access. See this [blogpost](https://maker.pro/linux/tutorial/basic-linux-commands-for-beginners) for the most important commands. For simple file editing in the shell we recommend `nano`. Nano is a basic simple file editor, great for making small changes to a file. See the guide on [efficient workflows](workflow.md) for more advanced tips and tricks. 

## Next step
After following the above steps, your development environment should be ready. Proceed to the [getting started](getting_started.md) guide to learn how to use the simulator. 
