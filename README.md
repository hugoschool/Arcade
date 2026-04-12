# Arcade

A gaming platform that loads graphic libraries and games at runtime using UNIX Dynamic Libraries.

## Build

Building the project is done using GNU Make.

Dependencies to build the core:
- None

Dependencies to build the game libraries:
- None

Dependencies to build the graphical libraries:
- SDL2 (SDL2_ttf, SDL2_image needed)
- NCurses
- SFML v3 (see `.github/workflows/build.yml` on how to download SFML v3)
- raylib v5.5 and raylib-cpp (same as above, see `.github/workflows/build.yml`)

```sh
make -j`nproc` # To build everything

make core # Only to build the core

make games # Only to build the game libraries

make graphicals # Only to build the graphical libraries
```

## Documentation

Please see the `./doc` folder for documentation on how to implement new graphics libraries or game libraries compatible with our core Arcade.

## In collaboration with

This project was done with other students from Epitech Lyon.

The interface done for this project was called the [cacarcade](https://github.com/freaky-family/cacarcade).

3 groups were sharing this interface:
- 1st group: Hugo ARNAL (hugo.arnal@epitech.eu) & Esteban HAZANAS (esteban.hazanas@epitech.eu)
- 2nd group: Sacha DEFOSSEZ (sacha.defossez@epitech.eu) & Bastien SUKIENNIK (bastien.sukiennik@epitech.eu)
- 3rd group: Nadal BERTHELON (nadal.berthelon@epitech.eu), Alexandre KUBIACZYK (alexandre.kubiaczyk@epitech.eu) & Hector CORDAT-BOURSIAC (hector.cordat-boursiac@epitech.eu)
