import cv2
import numpy as np

img = cv2.imread("Dec13.png")
img2 = cv2.imread("May13.png")

sensitivity = 8;
lower = [60 - sensitivity, 100, 50]
upper = [60 + sensitivity, 255, 255]

lower = np.array(lower, dtype = "uint8")
upper = np.array(upper, dtype = "uint8")


dst = cv2.inRange(img, lower, upper)
no_blue = cv2.countNonZero(dst)
output = cv2.bitwise_and(img, img, mask = dst)
print('The number of green pixels in "Dec13.png" is: ' + str(no_blue))

dst2 = cv2.inRange(img2, lower, upper)
no_blue2 = cv2.countNonZero(dst2)
output2 = cv2.bitwise_and(img2, img2, mask = dst2)
print('The number of green pixels in "May13.png" is: ' + str(no_blue2))

threshold = 3000
print('The threshold is ' + str(threshold))
if ((no_blue2 - no_blue) >= threshold):
    print ('This area exceeded the threshold and should be checked out.')
else:
    print('This area did not exceed the threshold and is doing fine.')


cv2.namedWindow("opencv")
cv2.imshow("images", np.hstack([img, output]))
cv2.imshow("images", np.hstack([img2, output2]))
cv2.waitKey(0)
