# Digital Image Basics

## Preface
This README goes over fundamental concepts related to digital images. 

## Structure of an Image 

An image - also referred to as a "frame" in the context of video - is composed of __lines__. Lines are made up of __pixels__. A pixel is the smallest unit of a digital image, and is essentially a coloured dot. 

__Image resolution__ is simply the number of pixels per line multiplied by the number of lines. For a `VGA` frame format for example, the number of pixels per line is 640, and the number of lines captured is 480. Thus, VGA has a resolution of 640 x 480 pixels.

A video then, is simply a succession of images, or __frames__.

<br />

## Pixel Formats 

__Pixel format__ refers to how image data from a camera is represented. Common ones specific to the `OV7670` camera module will be covered. See the OV7670_Summary.md or an available OV7670 data sheet for more details about the supported data output formats.

### Monochrome

In the __monochrome__ pixel format, each pixel is stored as a byte (8 bits), and represents _grey scale_ levels from 0 to 255. A value of 0 denotes black, a value of 255 means white, and values in between are different shades of grey. 

### RGB

The RBG colour model decomposes colours into varying intensities of red, green, and blue (R, G, and B). Therefore, in the __RGB__ pixel format, different intensities of red, green, and blue constitute a pixel. The format is typically expressed as `RGBxyz`, where _x_ is the number of bits assigned to the red channel, _y_ the green channel, and _z_ the blue channel. `RGB888` is the most common format; however, the OV7670 uses RGB565, RGB555, and RGB444.

The advantage of using smaller channels is a lower memory footprint. However, formats with a lower number of assigned bits per channel are limited to the number of available colours that can be visually represented.

### YCbCr

The __YCbCr__ pixel format has two components: (1) the _luminance_ component (Y), and (2) the _chroma_ component (Cb and Cr). 

The luminance component is related to the amount of white light in a colour. It defines the "grey scale levels" of the image. The chroma component, which is comprised of both Cb and Cr, encodes the blue (Cb) and red (Cr) levels of the image. 

The YCbCr format stores each channel (Y, Cb, and Cr) as one byte (8-bits). The OV7670 uses the `YCbCr422` format. In terms of chroma subsampling, this means that in a conceptual sampling region 4 pixels wide and 2 pixels high, there are 2 chroma or chrominance samples in the first row, and 2 changes in the number of chroma or chrominance samples between the first and second row (and successive rows thereafter).

Figure 1 shows pixel output data stored as 32-bit words containing 2 luminance and 2 chroma components per word. Each channel is equivalent to 8-bits.

<br />

|------- | Byte 0 | Byte 1 | Byte 2 | Byte 3 |
|--------|--------|--------|--------|--------|
| Word 0 | Cb0    |   Y0   | Cr0    |  Y1    |
| Word 1 | Cb2    |   Y2   | Cr2    |  Y3    |
| Word 2 | Cb4    |   Y4   | Cr4    |  Y5    |

<p align="center"><b>Figure 1: Pixel output data stored as words</b></p>

From Figure 1, notice how the index of the chroma changes by 2 each row (0 to 2 then to 4). Figure 2 shows the arrival order of the data.

<br />

| Position | 1   | 2   | 3   | 4   | 5   | 6   | 7   | 8  |  9  | 10  | 11  | 12  |
|----------|-----|-----|-----|-----|-----|-----|-----|----|-----|-----|-----|-----|
| Byte     | Cb0 | Y0  | Cr0 | Y1  | Cb2 | Y2  | Cr2 | Y3 | Cb4 | Y4  | Cr4 | Y5  |

<p align="center"><b>Figure 2: Arrival order of luminance and chroma data, YCbCr422 pixel format</b></p>


As may be observed from Figure 2, the arrival order of the data is word by word, byte by byte. The actual pixels are grouped according to the index. Pixel 0, for example, would be `Y0Cb0Cr0`; pixel 1 would be `Y1Cb0Cr0`, and pixel 2 would be `Y2Cb2Cr2`.
