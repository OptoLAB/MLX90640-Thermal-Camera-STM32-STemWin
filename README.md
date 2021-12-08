# MLX90640-Thermal-Camera-STM32-STemWin
 Demo for Melexis MLX90640 sensor using mikromedia 7 for STM32F7 board
## About project
This is a demo project for [Melexis MLX90640](https://www.melexis.com/en/product/mlx90640/far-infrared-thermal-sensor-array) thermal image sensor using [IR GRID 3 Click](https://www.mikroe.com/ir-grid-3-click) board from Mikroelektronika. Project was built using following ecosystem:

 - [mikromedia 7 for STM32F7](https://www.mikroe.com/mikromedia-7-stm32f7) development board by Mikroelektronika with STM32F746ZG mcu.
 - [mikroProg](https://www.mikroe.com/mikroprog-stm32) v1.5. Mikromedia 7 board has onboard mikroProg programmer and debugger.
 - [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html?sc=stm32cubemx) 5.6.0 initialization code generator by STMicroelectronics.
 - [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) 1.3.0 Integrated Development Environment for STM32.
 - [STemWin](https://www.st.com/en/embedded-software/stemwin.html) GUI library (SEGGER's emWin library for STM32)
 
**Notice:** MLX90640 sensor respond on all supported refresh rates (0.5-64Hz)

More info can be found [here](https://www.optolab.ftn.uns.ac.rs/index.php/education/project-base/260-mlx90640-thermal-camera)

Demo video on [youtube](https://youtu.be/8_YB1Ve56do) 

## Demo features:
Demo is built with following features:
- Adjustable scale for temperature range (-40,300) with **autoscale** option
- Multiple **color pallettes** for thermal image drawing. Pallettes are implemented as lookup tables, generated usign [python script] (https://github.com/OptoLAB/Color-Pallette-Lookup-Table-Generator). Following pallettes are implemented:

<p align="center">
<img src="https://github.com/OptoLAB/MLX90640-Thermal-Camera-STM32-STemWin/blob/main/pallette/color_pallettes.png" width="500"/>
</p>

- Realtime image interpolation using **bilinear interpolation**. Following samples ilustrate raw (left) and interpolated image (right):

<p align="center">
<img src="https://github.com/OptoLAB/MLX90640-Thermal-Camera-STM32-STemWin/blob/main/interpolation%20samples/IMG1.BMP" width="288"/>
<img src="https://github.com/OptoLAB/MLX90640-Thermal-Camera-STM32-STemWin/blob/main/interpolation%20samples/IMG0.BMP" width="288"/>
</p>

- **Recording** bimap images on SD card. Bellow are samples (interpolated) captured using different color pallettes:

<p align="center">
<img src="https://github.com/OptoLAB/MLX90640-Thermal-Camera-STM32-STemWin/blob/main/samples/IMG0.BMP" width="144"/>
<img src="https://github.com/OptoLAB/MLX90640-Thermal-Camera-STM32-STemWin/blob/main/samples/IMG1.BMP" width="144"/>
<img src="https://github.com/OptoLAB/MLX90640-Thermal-Camera-STM32-STemWin/blob/main/samples/IMG2.BMP" width="144"/>
<img src="https://github.com/OptoLAB/MLX90640-Thermal-Camera-STM32-STemWin/blob/main/samples/IMG3.BMP" width="144"/>
<img src="https://github.com/OptoLAB/MLX90640-Thermal-Camera-STM32-STemWin/blob/main/samples/IMG4.BMP" width="144"/>
<img src="https://github.com/OptoLAB/MLX90640-Thermal-Camera-STM32-STemWin/blob/main/samples/IMG5.BMP" width="144"/>
<img src="https://github.com/OptoLAB/MLX90640-Thermal-Camera-STM32-STemWin/blob/main/samples/IMG6.BMP" width="144"/>
<img src="https://github.com/OptoLAB/MLX90640-Thermal-Camera-STM32-STemWin/blob/main/samples/IMG7.BMP" width="144"/>
<img src="https://github.com/OptoLAB/MLX90640-Thermal-Camera-STM32-STemWin/blob/main/samples/IMG8.BMP" width="144"/>
<img src="https://github.com/OptoLAB/MLX90640-Thermal-Camera-STM32-STemWin/blob/main/samples/IMG9.BMP" width="144"/>
<img src="https://github.com/OptoLAB/MLX90640-Thermal-Camera-STM32-STemWin/blob/main/samples/IMG10.BMP" width="144"/>
<img src="https://github.com/OptoLAB/MLX90640-Thermal-Camera-STM32-STemWin/blob/main/samples/IMG11.BMP" width="144"/>
</p>

- Other paramaters such as pixel size, image position, number of interpolation points, new color pallettes, MLX90640 data aqusition configuratio (i.e. refresh rate) can be set inside source code.
