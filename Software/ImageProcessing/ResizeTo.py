import cv2 as cv
import numpy as np

def ResizeTo(src, size):

    input_height = src.shape[0]
    input_width  = src.shape[1]
    input_channels = src.shape[2]

    if(input_height > input_width):
        # 如果高大于宽
        # resize到最大边为width
        output_width = int(input_width * size / input_height)
        src = cv.resize(src,(output_width,size))
        
        mask = np.zeros([size + 2, size + 2, 1], np.uint8)
        background = np.zeros((size, size, input_channels), np.uint8)

        cv.floodFill(background, mask, (0, 0), (100, 100, 100), cv.FLOODFILL_MASK_ONLY)

        delta = int((size - output_width) / 2)
        np.copyto(background[0:size, delta:delta + output_width], src)
        src = background


    elif(input_height < input_width):
        # 宽大于高
        output_height = int(input_height * size / input_width)
        src = cv.resize(src,(size, output_height))
        
        mask = np.zeros([size + 2, size + 2, 1], np.uint8)
        background = np.zeros((size, size, input_channels), np.uint8)

        cv.floodFill(background, mask, (0, 0), (100, 100, 100), cv.FLOODFILL_MASK_ONLY)

        delta = int((size - output_height) / 2)
        np.copyto(background[delta:delta + output_height, 0:size], src)
        src = background

    elif(input_height == input_width):
        # 宽高相等
        src = cv.resize(src,(size,size))

    return src