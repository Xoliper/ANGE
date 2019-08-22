![Version 0.1](https://img.shields.io/badge/Version-0.1-brightgreen.svg) ![Build passed](https://img.shields.io/badge/Build-Passed-brightgreen.svg)
---------------------------
# Ange [Another GUI Engine]

The easy, fast, multiplatform ![Windows](https://img.shields.io/badge/Windows-blue.svg) ![Linux](https://img.shields.io/badge/Linux-blue.svg) and free GUI framework for C++ language! Library is compatibile with all popular ![C++17](https://img.shields.io/badge/C++-17-brightgreen.svg) compilers ![MSVC](https://img.shields.io/badge/MSVC-blue.svg) ![GCC](https://img.shields.io/badge/GCC-blue.svg) ![CLANG](https://img.shields.io/badge/CLANG-blue.svg).

<p align="center">
 <img src="https://github.com/Xoliper/ANGE/blob/master/preview.jpg" />
 <br><br>
 <img src="https://xoliper.github.io/ANGE/Ange/Documentation/Images/51.png" />
</p>

## Compilation

### Download & configure
```
git clone https://https://github.com/Xoliper/ANGE/
cd ANGE
premake5 <your IDE/make>
```

### Compile
```
//Via make
make config=release

//Or, open your IDE and build Ange project.
```

### Use
```
1. Make sure you run "INSTALL" project, then copy from "INSTALL" folder; "include" and "lib" somewhere.
2. Create project in your IDE/make and add paths to that folders.
3. Remember to link everything from "lib" folder.
```

## FAQ 

###  Where is the documentation? Are there any guides for this?

Everything you need is [here](https://xoliper.github.io/ANGE/Ange/Documentation/index.html). You can also download all that documentation (as HTML subpages) from "Ange/Documentation" folder.

### Any example code?

Sure, [here](https://xoliper.github.io/ANGE/Ange/Documentation/1.%20Usage%20example.-Guide.html)!

### Any applications based on this library?

As far as i know - at the moment, no. But I'm currently writing a scene editor for this library. So it will probably be the first app.

## Implemented widgets
| Name &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; | Status &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; | Documentation &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; |
|:--------------------------|:-------------|:----|
| Window                    | Done         | Yes |
| Background/Rectangle2D    | Done         | Yes |
| Image                     | Done         | Yes |
| Text                      | Done         | Yes |
| SimpleButton              | Done         | Yes |
| SimpleInput               | Done         | Yes |
| Task                      | Done         | Yes |
| VerticalScroller          | Done         | Yes |
| CustomWidget              | Done         | Yes |
| ContextMenu               | Done         | Yes |
| ProgressBar               | Done         | Yes |
| Checkbox                  | Done         | Yes |
| Ratio		                | Done         | Yes |

## Roadmap
<p align="center">
 <img src="https://github.com/Xoliper/ANGE/blob/master/roadmap.png" />
</p>

## Dependences 
| Library &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; | Version &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; |
|:--------------|:-----------|
| OpenGL        | 3.3        |
| GLFW          | 3.2.1      |
| GLM           | 0.9.9 rev3 |
| FreeType      | 2.7.1      |
| libjpeg       | 9c         |
| libpng        | 1.6.37     |
| zlib          | 1.2.11     |
| glad          | 0.1.30     |

## License
MIT License

Copyright (c) 2019 Xoliper

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
