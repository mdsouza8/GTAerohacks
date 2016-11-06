import cv2
import numpy as np

img = cv2.imread("May13.png")

sensitivity = 8;
lower = [60 - sensitivity, 100, 50]
upper = [60 + sensitivity, 255, 255]

lower = np.array(lower, dtype = "uint8")
upper = np.array(upper, dtype = "uint8")


dst = cv2.inRange(img, lower, upper)
no_blue = cv2.countNonZero(dst)
print('The number of green pixels is: ' + str(no_blue))
cv2.namedWindow("opencv")
cv2.imshow("opencv",img)
cv2.waitKey(0)
