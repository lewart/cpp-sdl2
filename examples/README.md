# cpp-sdl2 examples

This folder contains a series of short programs that demonstrate this library for two purposes : 

 - Being able to test that the wrappers are working properly
 - Show some canonical use cases of theses wrapers

## Folder content 

 - **general** : A program that calls a number of functionalities from the API
 - **dll** : A dynamic library called "my_dll", and a program that loads it and call functions through cpp-sdl2. 0% platform specific code here
 - **opengl** : A program that display one triangle on a dark blue background. Used to demonstrate how to initialize painlessly a GL window anc context with cpp-sdl2
 - **vulkan** : A program that display one triangle on a dark blue background. Used to demonstate how to initialze painlessly a Vulkan Window, Instance and a (platform specific) Surface object with cpp-sdl2

## Building

This diretory is one project that contains all the mentioned examples

To build **general** example

```cd examples/
mkdir build
cd build/
cmake ..
cmake --build .
```

To build **opengl** example

```...
cmake .. -DCPP_SDL2_ENABLE_OPENGL=ON
...
```

To build **vulkan** example

```...
cmake .. -DCPP_SDL2_ENABLE_VULKAN=ON
...
```

