# Mandelbrot Set Visualization
This project is a **C++ based application** that generates and explores the Mandelbrot set using the **SFML library** for graphical rendering. It allows users to interact with the fractal by zooming in and out, moving across the plane, and adjusting the resolution for a detailed and customizable visual experience.

![Mandelbrot Set](mandelbrotsec2.png)
## Features

- **Interactive Visualization**: Explore the Mandelbrot set by zooming and moving across the fractal plane.
- **Adjustable Detail**: Use the mouse scroll to change the maximum number of iterations for more detailed fractal rendering.
- **Customizable Colors**: The color scheme is adjustable, providing beautiful visual effects and enhancing the fractal's details.

## Installation

For now, to run this project, you'll need to have **SFML** and a **C++ compiler** installed on your machine.

### Prerequisites

- C++17 or later
- [SFML](https://www.sfml-dev.org/download.php) (Simple and Fast Multimedia Library)

### Clone the Repository

```bash
git clone https://github.com/marielalvarez/Mandelbrot-Set/.git
```
### Build Instructions
Make sure you have SFML installed and properly linked with your C++ compiler. Then, to compile and run the project:
```bash
g++ -std=c++17 -o Mandelbrot Mandelbrot.cpp -lsfml-graphics -lsfml-window -lsfml-system
./Mandelbrot
```
### How to Use
- Zoom: Click the left mouse button to zoom in and the right mouse button to zoom out.
- Move: Use the arrow keys (up, down, left, right) to move across the fractal plane.
- Adjust Iterations: Scroll the mouse wheel to increase or decrease the number of maximum iterations for more or less detail in the fractal rendering.
- Resolution: Press the + key to increase resolution, and the - key to decrease resolution.

