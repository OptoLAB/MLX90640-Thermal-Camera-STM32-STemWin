# MLX90640-Thermal-Camera-STM32-STemWin
 Demo for Melexis MLX90640 sensor using mikromedia 7 for STM32F7 board
## Demo features:
Demo is built with following features:
- Adjustable scale for temperature range (-40,300) with **autoscale** option
- Multiple **color pallettes** for thermal image drawing. Pallettes are implemented as lookup tables, generated usign [python script] (https://). Following pallettes are implemented:

<p align="center">
<img src="https://github.com/OptoLAB/MLX90640-Thermal-Camera-STM32-STemWin/blob/main/pallette/pallettes.png" width="500"/>
</p>

- Realtime image interpolation using **bilinear interpolation**. Following samples ilustrate raw (left) and interpolated image (right):

<p align="center">
<img src="https://github.com/OptoLAB/MLX90640-Thermal-Camera-STM32-STemWin/blob/main/interpolation%20samples/IMG1.BMP" width="288"/>
<img src="https://github.com/OptoLAB/MLX90640-Thermal-Camera-STM32-STemWin/blob/main/interpolation%20samples/IMG0.BMP" width="288"/>
</p>

- **Capturing** bimap images on SD card. Bellow are samples captured using different color pallettes:
