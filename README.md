# My Raylib game

- 2D top down, player is a square
- map is cod nuketown
- Friendslop
- Shooting
    - guns have a conus that gets smaller the more you hold right click, if it gets too small you lose focus
    - the conus gets a little bigger depending on the recoil each time you shoot
    - recoil
    - invincibility dash
    - arms race

1. player movement
2. basic map loading, find a format

it might make it very fun to add a camera like in League of Legeneds, that you have to move it with the mouse
make it so the player can go through 2 block gaps easily
the mouse should also interpolate
install fake-virtual-space extension
add a second camera on the texture,


## if you wanna run it

### MACOS 

no idea

### WINDOWS

install w64devkit-x64-X.X.X.7z.exe from https://github.com/skeeto/w64devkit  
into `C:\w64devkit`
append `C:\w64devkit\bin` to PATH

add this into your vscode settings.json
```
    "terminal.integrated.defaultProfile.windows": "busybox",
    "terminal.integrated.profiles.windows": {
        "busybox": {
            "path": "C:\\\\w64devkit\\\\bin\\\\busybox.exe",
            "args": ["sh"],
            "icon": "terminal-bash"
        }
    },
```

for clang formatting what is needed is:
in busybox run `winget install --id=LLVM.LLVM -e` to install `clangd`
append `C:\Program Files\LLVM\bin` to PATH
install the vscode extensions: `clangd`, `Clang-Format`, `C/C++ Extension pack`
make sure the `Clang-Format` points to the projects' `.clang-format`
its like 700 mb lol, maybe don't install all of llvm.