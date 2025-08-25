# Tile converter

The tile converter is a simple program to convert 256-Color Bitmap files to J'nanin TIL files.

To convert a Bitmap to a tile, type:  
```converter --tile [input.bmp] [output.til]```

To convert a Tile to a Bitmap, type:  
```converter --bmp [input.til] [output.bmp]```

IMPORTANT: All inputed BMP files ***must*** be 256 color bitmaps, else it will not work.  
           This also means all outputted bitmaps will be in 256 color format.