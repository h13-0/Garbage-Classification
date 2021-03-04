import cv2 as cv
import numpy as np

cap = cv.VideoCapture(0)

while True:
    ret, src = cap.read()
    cv.imshow("src", src)

    # threshold
    ret, thre = cv.threshold(src, 100, 255, cv.THRESH_TOZERO)
    #ret, thre = cv.threshold(src, 150, 255, cv.THRESH_TOZERO)

    cv.imshow("thre", thre)

    # Method 1
    ## erode
    kernel = np.ones((5,5),np.uint8) 
    erosion = cv.erode(thre, kernel, iterations = 1)

    cv.imshow("erosion", erosion)

    # Method 2
    ## gray
    gray = cv.cvtColor(erosion, cv.COLOR_BGR2GRAY)

    ## findContours
    contours, hierarchy = cv.findContours(gray, cv.RETR_TREE, cv.CHAIN_APPROX_NONE)

    blank = np.zeros(gray.shape, dtype=np.uint8)

    ## output
    output = blank.copy()
    for contour in contours:
        if(cv.contourArea(contour) > 10):
            output = cv.drawContours(blank, [contour], -1, (255, 255, 255), 2)
    
    cv.imshow("findContours", output)


    cv.waitKey(10)
