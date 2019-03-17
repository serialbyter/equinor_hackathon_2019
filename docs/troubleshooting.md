## Troubleshooting

### Line ending trouble for Windows users
When working on a Windows machine and running code on Linux, we are bound to run into line ending troubles. 
If you get an error message like `/usr/bin/env: ‘python\r’: No such file or directory`, then you can fix it by running the following command on the server in the source directory. 

```bash
dos2unix *
```
This will make sure all files in the directory have Unix style line endings, and should fix the error. 

### Program won't stop
If you have a program which can't be killed using `CTRL+c`, then it is most likely stuck in a loop somewhere. 
To kill it you should first stop the simulator, ie. the one started with `roslaunch techathon main.launch`. After 
that is stopped, run `ps -a` in a terminal to find the name of the process. `ps -a` lists a few processes, 
usually `bash`, `ps`, and then the program that won't stop; usually `python`. 

To stop it, first try `pkill python`. This will tell all python programs to stop. If this doesn't work we can force it
to stop by using `pkill -9 python`. This will tell the operating system to simply ignore the process. If your process 
has external resources (for instance if it is writing to file), then this can lead to dataloss and datacorruption. 

