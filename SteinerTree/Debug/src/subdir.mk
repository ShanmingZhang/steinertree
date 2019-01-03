################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/SteinerTree.cpp \
../src/bfs-demo.cpp \
../src/bfs-dfs.cpp \
../src/dijkstra.cpp \
../src/graph.cpp \
../src/prim.cpp \
../src/queue.cpp 

OBJS += \
./src/SteinerTree.o \
./src/bfs-demo.o \
./src/bfs-dfs.o \
./src/dijkstra.o \
./src/graph.o \
./src/prim.o \
./src/queue.o 

CPP_DEPS += \
./src/SteinerTree.d \
./src/bfs-demo.d \
./src/bfs-dfs.d \
./src/dijkstra.d \
./src/graph.d \
./src/prim.d \
./src/queue.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


