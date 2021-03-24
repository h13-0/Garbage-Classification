import cv2 as cv

cap = cv.VideoCapture(0)
cap.set(cv.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv.CAP_PROP_FRAME_HEIGHT, 480)

while True:
    ret, frame = cap.read()

    frame = cv.flip(frame, -1)

    cv.imshow("frame", frame)

    cv.waitKey(1)