# importing required libraries
import cv2
import numpy as np
from PIL import Image  
# taking the input from webcam
vid = cv2.VideoCapture(0)
  
# running while loop just to make sure that
# our program keep running until we stop it

# sliders to help determine mask values
cv.createTrackbar('LH','mask',0,255,nothing)
cv.createTrackbar('LS','mask',0,255,nothing)
cv.createTrackbar('LV','mask',0,255,nothing)

cv.createTrackbar('UH','mask',0,255,nothing)
cv.createTrackbar('US','mask',0,255,nothing)
cv.createTrackbar('UV','mask',0,255,nothing)

while True:
  
    # capturing the current frame
    ret, frame = vid.read()
    if not ret:
      print("Can't receive frame (stream end?). Exiting ...")
      break
    # displaying the current frame
    # cv2.imshow("frame", frame) 

    # converting camera input from BGR to HSV
    hsv = cv2.cvtColor(frame, cv.COLORBGR2HSV)
    
    #getting slider values
    lowerH = cv.getTrackbarPos('LH', 'mask')
    lowerS = cv.getTrackbarPos('LS', 'mask')
    lowerV = cv.getTrackbarPos('LV', 'mask')

    upperH = cv.getTrackbarPos('UH', 'mask')
    upperS = cv.getTrackbarPos('US', 'mask')
    upperV = cv.getTrackbarPos('UV', 'mask')

    lowerGreen = np.array([lowerH, lowerS, lowerV])
    upperGreen = np.array([upperH, upperS, upperV])

    mask = cv.inRange(hsv, lowerGreen, upperGreen)
    filtered = cv.bitwise_and(frame, frame, mask)
    # im = Image.open('frame1.jpg') 
    # pix = im.load()
    cv.imshow('frame', frame)
    cv.imshow('mask' mask)
    cv.imshow('filtered', filtered)
    
    ''' this will be section of code to find where our mark is located
    for x in range(0,640): # might need to change dimension
      for y in range(0,480): # might need to change dimension
        print pix[x,y]
    '''
# convert to hsv
# hardcode distance of led from frame's center - accurate alignment
# find center-ish of tape so we can align properly
# test with single frame
# test while moving camera 
# make some kind of indication we found the tape - box? but then need to coordintae w dummy motor code and motors

# motor code - how to check/scan the area for the tape
