
import cv2
import numpy as np
import serial
import time

#def nothing(x):
#    pass

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
ser.reset_input_buffer()

# Create a black image, a window
cv2.namedWindow('Trackbar window')

#Open webcam, choose the nbr change '0' to nbr of your webcam
capture = cv2.VideoCapture(0)

targetX = 240
targetY = 320

while(1):
    _, frame = capture.read()
    #cv2.imshow('Trackbar window', np.zeros((1,512,3), np.uint8))
    
    _f = cv2.medianBlur(frame, 15)
    #_f = cv2.GaussianBlur(frame, (9, 9), 0)
    _f = cv2.cvtColor(_f, cv2.COLOR_BGR2HSV) #To HSV
    
    
    
    mask = cv2.inRange(_f, (174,60,0), (187,255,255))
    filtered = cv2.morphologyEx(mask, cv2.MORPH_OPEN, np.ones((3, 3), np.uint8))
    moments = cv2.moments(filtered)
   # Sum = np.sum(mask)
    ##print(Sum)
    if(moments["m00"] != 0.0):
        cX = int(moments["m10"]/moments["m00"])
        cY = int(moments["m01"]/moments["m00"])
    else:
        cX = -1
        cY = -1
    if(cX >= 0 and cY >=0):
        xdiff = cX - targetX
        ydiff = cY - targetY
    else:
        xdiff = 1000
        ydiff = 1000
    
    data = "%d, %d\n"%(xdiff, ydiff)
    ser.write((b"%d, %d\n"%(xdiff, ydiff)))
    #print(data)
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()
        print(line)
    #bit of debug/dev code, remove for final version
    #print(cX, cY)
    cv2.circle(filtered,(cX, cY), 5, (255, 0, 0), -1)
    
    #detect any green in the picture and tell motors to stop
    #count = cv2.countNonZero(mask)
    #print(count)
    
    #if(count > 200):
    #    ser.write(b"stop\n")
    #else:
    #    ser.write(b"go\n")
    #ser.write(b"test\n")
    #if ser.in_waiting > 0:
    
    #read and print serial messages from microprocessor
    #line = ser.readline().decode('utf-8').rstrip()
    #print(line)
    
    #Comment the one you won't need	
    #cv2.imshow('frame',frame)
    cv2.imshow('mask',filtered)
    #time.sleep(0.5)

    k = cv2.waitKey(1) & 0xFF
    if k == 27: #press escape to exit
        break
    
capture.release()   #Release the camera
cv2.destroyAllWindows() #Close all windows
