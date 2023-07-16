# ChiTypeTrainer
Typetrainer written with C++ and wxwidgets
## Windows installation
For the correct work of the application, you need:
- Packet manager [vcpkg](https://vcpkg.io/)
- Tool [doxygen](https://www.doxygen.nl/)
- Library `nlohmann-json` that can be installed using `vcpkg`:
`vcpkg install nlohmann-json:<triplet for your system>`
- Library `wxwidgets` that can be installed using `vcpkg`:
`vcpkg install wxwidgets:<triplet for your system>`
## Building
1. Clone the repository
'git clone https://github.com/zgushonkaK/ChiTypeTrainer.git'
2. Build and install with cmake:
`cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=<your vcpkg root>/scripts/buildsystems/vcpkg.cmake`
`cmake --build build --config Release`
`cmake --install ./build --prefix ./cards`