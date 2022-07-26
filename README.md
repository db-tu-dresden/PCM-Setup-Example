# PCM-Setup-Example

## Compile the PCM-Benchmark

First, make sure that you have cmake 3.10 (or newer) installed on your system and configure the CMakeLists.txt to meet 
your needs. Check if the right compiler flags are set for your system and don't forget to link the libpcm.so library to 
your executable (`target_link_libraries(<excetutable-name> libpcm.so)`).

Execute the below command to initialze cmake.
```
cmake .
```

And build your executable. The important linker information (to find pcm) are automatically set by cmake.
```
cmake --build . --target <executable-name>
```

The compiled programm can be found in the diectory `bin`.

## Execute the PCM-Benchmark

Before executing the excecutable, make sure execute
```
sudo modprobe msr
```

PCM needs to root-access to work properly. So you need to run the exceutable as root.
```
sudo bin/<executable-name>
```
