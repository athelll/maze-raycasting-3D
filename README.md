Personal Project used to make a 3d maze game using raycasting
similar to popular retro FPS 3D game wolfheinstein 3D.

#### Compile and execute on linux (Debian/Ubuntu based distros)
ensure Make, SDL2, and g++ is installed then run:
```bash
make
```
after excute the executable:
```bash
./raycaster
```
raycaster runs at about 60FPS on average (average has not been measured but from logs thats what i observed)

#### Current issues with raycaster:
1) At some angles, navigating the player forward or backwards causes some sudden frame skip-ish effects although rotation works, to my standards, somewhat flawlessly
