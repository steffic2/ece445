
import cv2
import numpy as np

def nothing(x):
    pass

# Create a black image, a window
cv2.namedWindow('Trackbar window')

#Open webcam, choose the nbr change '0' to nbr of your webcam
capture = cv2.VideoCapture(0)


while(1):
    _, frame = capture.read()
    #cv2.imshow('Trackbar window', np.zeros((1,512,3), np.uint8))
    
    _f = cv2.medianBlur(frame, 15)
    _f = cv2.cvtColor(_f, cv2.COLOR_BGR2HSV) #To HSV
    
    frame = cv2.inRange(_f, (36,25,25), (70,255,255))
    
    
    #Comment the one you won't need
    cv2.imshow('frame',frame)
    #cv2.imshow('mask',mask)


    k = cv2.waitKey(1) & 0xFF
    if k == 27: #press escape to exit
        break

    
capture.release()   #Release the camera
cv2.destroyAllWindows() #Close all windows
