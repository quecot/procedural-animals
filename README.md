# Procedural animals

This project demonstrates a simple procedural animation technique to create a snake-like movement using [raylib](https://www.raylib.com/) and C. The project is inspired by a video on procedural animation by argonaut, which can be found [here](https://www.youtube.com/watch?v=qlfh_rv6khY).

## Features

- **Procedural animation**: A simple and effective technique to animate a snake.
- **Mouse and touch control**: Move the snake by moving the mouse cursor or by touching the screen.
- **Web port**: The project is also ported to WebAssembly using [zozlib.js](https://github.com/tsoding/zozlib.js).

## Getting Started

### Prerequisites

- [raylib](https://github.com/raysan5/raylib)
- A C compiler (e.g., GCC, Clang)

### Building and Running

1. Clone the repository:

```sh
git clone https://github.com/quecot/procedural-animals.git
cd procedural-animals
```

2. Get raylib from the [releases page](https://github.com/raysan5/raylib/releases/) (adapt to your OS)

```sh
wget https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_macos.tar.gz
tar -xf raylib-5.0_macos.tar.gz
```

3. Compile the project:

```sh
mkdir build && cd build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE
cmake --build .
```

4. Run the executable:

```sh
./main
```

### Controls

- **Mouse and touch**: Move the snake by moving the mouse cursor or touching and dragging the screen on mobile.
- **Spacebar**: Pause or resume the snake's movement.

## Web Version

The project is also available as a WebAssembly (WASM) application.
A live version can be found here [snake.fvila.dev](https://snake.fvila.dev).

To build and run the web version:

1. Compile the project for WebAssembly using [nob.h](https://github.com/tsoding/nobuild):

```sh
cd web
clang -o nob nob.c
```

2. Serve the generated WASM and access it in [http://localhost:8000](http://localhost:8000)

```sh
python3 -m http.server
```

## Code Overview

The main animation logic is implemented in the main.c file. The key components include:

- **Initialization**: Setting up the window, colors, and initial positions of the snake's head and body.
- **Update Loop**: Handling user input, updating the snake's position and body segments, and applying constraints to ensure smooth movement.
- **Drawing Loop**: Rendering the snake, its eyes, and the mouse cursor.

## Contributing

Contributions are welcome! Feel free to open issues or submit pull requests with improvements, new features, or bug fixes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE.md) file for details.

## Acknowledgements

- [argonaut](https://www.youtube.com/watch?v=qlfh_rv6khY), for the inspiration video on procedural animation.
- [raysan5](https://github.com/raysan5/raylib), for the raylib library.
- [tsoding](https://github.com/tsoding), for the zozlib.js and nobuild libraries and many helpful videos about Recreational Programming.
