import cv2 as cv
import numpy as np

input = cv.imread("./1.jpg")
mask = cv.imread("./mask.png")

end = cv.bitwise_and(input, mask)

cv.imshow("end", end)
cv.waitKey(0)
