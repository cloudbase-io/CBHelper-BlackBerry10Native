################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CBHelperDemo.cpp \
../src/main.cpp 

CPP_DEPS += \
./src/CBHelperDemo.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: QCC Compiler'
	qcc  -o $@ $< -V4.6.3,gcc_ntox86_cpp -w1 -I"/Users/stef/Documents/Projects/cloudbase/helper/CBHelper-BlackBerry10Native/CBHelper" -I/Users/stef/Documents/Projects/cloudbase/helper/CBHelper-BlackBerry10Native/CBHelper/src -I/Users/stef/Documents/Projects/cloudbase/helper/CBHelper-BlackBerry10Native/CBHelper/src/DataCommands -I/Users/stef/Documents/Projects/cloudbase/helper/CBHelper-BlackBerry10Native/CBHelper/src/GeoDataStream -I/Users/stef/Documents/Projects/cloudbase/helper/CBHelper-BlackBerry10Native/CBHelper/src/yajl -I/Users/stef/Documents/Projects/cloudbase/helper/CBHelper-BlackBerry10Native/CBHelper/src/yajl/api -I/Applications/Momentics.app/target_10_2_0_339/qnx6/usr/include/bb/communications/push -I/Applications/Momentics.app/target_10_2_0_339/qnx6/usr/include/qt4/QtGui -I/Applications/Momentics.app/target_10_2_0_339/qnx6/usr/include/bb/cascades/pickers -I/Applications/Momentics.app/target_10_2_0_339/qnx6/usr/include/bb/device -I/Applications/Momentics.app/target_10_2_0_339/qnx6/usr/include/qt4/QtConnectivity -I/Applications/Momentics.app/target_10_2_0_339/qnx6/usr/include/qt4/QtCore -c -O0 -Wp,-MMD,$(basename $@).d -Wp,-MT,$@
	@echo 'Finished building: $<'
	@echo ' '


