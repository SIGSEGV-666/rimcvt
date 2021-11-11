# rimcvt
A simple little converter (actually two!) written in C that can convert most common image file formats (i.e. PNG, JPEG, Targa) to my .rim file format and vice versa with the help of stb_image and stb_image_write.
# Building
Simply clone this repo and for ...
 - Microsoft Windows with MinGW(-w64) or DJGPP:
    Run `compile_windows_mingw.bat` to generate `img2rim.exe` and `rim2img.exe`
 - Linux (or any *NIX technically) with gcc or clang:
    In your favorite shell or terminal emulator, do:
        - `chmod 777 compile_linux.sh`
        - For gcc: `export compiler="gcc"` or for clang: `export compiler="clang"`
        - `./compile_linux.sh`
    And if all goes well, then you will get two new executables in the current directory, 
    `img2rim` and `rim2img`.
# Running and usage
`img2rim` converts from any image file formats supported by stb_image to a .rim file.
`rim2img` converts any non-indexed .rim file to either PNG, JPEG, or Targa (.tga)

The two applications are CLI-only (meaning there's no GUI), so the list of command line options for each are as follows:

- `img2rim <input image file> <output .rim file>`
- `rim2img <input .rim file> <output image file> [ [-jpg_quality <JPEG quality integer value between 1 and 100, defaults to 25>] [-png_complevel <PNG compression level integer value, defaults to 8>] [-tga_rle <write RLE compressed .tga file (0 for no RLE, 1 for RLE), defaults to 1>] ]`

Examples.
- `img2rim texture.png texture.rim`
- `rim2img original.rim converted.png`
- `rim2img original.rim converted_with_rle.tga`
- `rim2img original.rim converted.png -png_complevel 2`
- `rim2img original.rim converted_no_rle.tga -tga_rle 0`

# See also
- My original .rim repo from 2020: https://github.com/SIGSEGV-666/librim

