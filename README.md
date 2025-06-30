# Low-Light-Video-Enhancement-Using-a-Retinex-Based-Fast-Algorithm

## Description
This project is an extended version of the [Low-Light-Enhancement-Using-a-Retinex-Based-Fast-Algorithm](https://github.com/Vaishu-999/Low-Light-Image-Enhancement-Using-a-Retinex-Based-Fast-Algorithm), originally implemented in Python and MATLAB for image enhancement under low-light conditions.

In this extended version, the algorithm has been re-implemented and optimized in **C++**, and further enhanced to support:
- **Real-time low-light video enhancement**
- **Device-oriented output handling** for potential hardware integration

The algorithm has been successfully tested on the **Motorola Solutions V200 body-worn camera device**, and has shown significant improvement in image quality, making it suitable for practical, real-world low-light video scenarios.

This version was developed as part of a hackathon initiative, focusing on speed, efficiency, and real-time enhancement performance on edge devices.

---

## Repository Structure
```
├── Model
│ ├── c++_version
│ ├── python_version
│
├── resources
│ └── input
| └── output
│
├── README.md
├── requirements.txt

```


## Installation and Setup

### 🧱 Prerequisites
- C++17 or later
- OpenCV installed (`>= 4.x`)
- CMake
- Visual Studio / VS Code with C++ extensions

### 📺 C++ Setup Video Tutorial (Recommended)
Watch this to learn how to set up a complete C++ + OpenCV + CMake project in VS Code:  
📹 [https://www.youtube.com/watch?v=CnXUTG9XYGI](https://www.youtube.com/watch?v=CnXUTG9XYGI)

## ⚙️ Build and Run (VS Code Terminal Example)
  
  cmake -B .\build\
  cmake --build .\build\
  .\build\Debug\RBFA_project.exe
  # To clean build directory (if needed):
  Remove-Item -Recurse -Force .\build\
## Reference
This project extends the original work:
Low-Light-Enhancement-Using-a-Retinex-Based-Fast-Algorithm
Implemented in Python and MATLAB for image enhancement.

## Helpful Resources
Here are some helpful resources for setting up and working with C++, OpenCV, and CMake:

- 📺 [C++ OpenCV + CMake + VS Code Setup](https://www.youtube.com/watch?v=CnXUTG9XYGI)
- 📺 [Installing OpenCV with Visual Studio](https://www.youtube.com/watch?v=m9HBM1m_EMU)
- 🌐 [OpenCV Releases](https://opencv.org/releases/)
- 🌐 [CMake Downloads](https://cmake.org/download/)
- 📘 [GeeksForGeeks – Read and Display Image in OpenCV](https://www.geeksforgeeks.org/cpp/reading-and-displaying-an-image-in-opencv-using-c/)
- 📘 [OpenCV SRF – Load and Display Image](https://www.opencv-srf.com/2017/11/load-and-display-image.html)

