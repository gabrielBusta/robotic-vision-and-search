import vrep
import sys
import time
import numpy as np
import math
import matplotlib.pyplot as plt
import cv2

PI = math.pi

vrep.simxFinish(-1) # just in case, close all opened connections

clientID = vrep.simxStart('127.0.0.1',19999,True,True,5000,5)

if clientID != -1:  #check if client connection successful
    print('Connected to remote API server')

else:
    print('Connection not successful')
    sys.exit('Could not connect')

steerR = 0
steerL = 0
# prevx = 0

#retrieve motor  handles
errorCode,left_motor_handle = vrep.simxGetObjectHandle(clientID,'Pioneer_p3dx_leftMotor',vrep.simx_opmode_oneshot_wait)
errorCode,right_motor_handle = vrep.simxGetObjectHandle(clientID,'Pioneer_p3dx_rightMotor',vrep.simx_opmode_oneshot_wait)

# Save camera reference
_, camHandle = vrep.simxGetObjectHandle(clientID, 'cam1', vrep.simx_opmode_oneshot_wait)

# Initiate camera and wait 1 second for buffer to fill
_, resolution, image = vrep.simxGetVisionSensorImage(clientID, camHandle, 0, vrep.simx_opmode_streaming)
time.sleep(1)

velocity = 0.35 # Velocity of the motors
velocity2 = 0.15

sensor_h = [] #empty list for handles
sensor_val = np.array([]) #empty array for sensor measurements

# orientation of all the sensors:
sensor_loc = np.array([-PI/2, -50/180.0*PI,-30/180.0*PI,-10/180.0*PI,10/180.0*PI,30/180.0*PI,50/180.0*PI,PI/2,PI/2,130/180.0*PI,150/180.0*PI,170/180.0*PI,-170/180.0*PI,-150/180.0*PI,-130/180.0*PI,-PI/2])

# for loop to retrieve sensor arrays and initiate sensors
for x in range(1,16+1):
        errorCode,sensor_handle = vrep.simxGetObjectHandle(clientID,'Pioneer_p3dx_ultrasonicSensor'+str(x),vrep.simx_opmode_oneshot_wait)
        sensor_h.append(sensor_handle) #keep list of handles
        errorCode,detectionState,detectedPoint,detectedObjectHandle,detectedSurfaceNormalVector = vrep.simxReadProximitySensor(clientID,sensor_handle,vrep.simx_opmode_streaming)
        sensor_val = np.append(sensor_val,np.linalg.norm(detectedPoint)) #get list of values

t = time.time()

while (time.time()-t) < 600:
    #Loop Execution
    sensor_val = np.array([])
    for x in range(1,16+1):
        errorCode, detectionState, detectedPoint, detectedObjectHandle, detectedSurfaceNormalVector = vrep.simxReadProximitySensor(clientID, sensor_h[x-1], vrep.simx_opmode_buffer)
        sensor_val = np.append(sensor_val, np.linalg.norm(detectedPoint)) #get list of values

    #controller specific
    sensor_sq = sensor_val[0:8] * sensor_val[0:8] #square the values of front-facing sensors 1-8

    min_ind = np.where(sensor_sq == np.min(sensor_sq))
    min_ind = min_ind[0][0]

    if sensor_sq[min_ind] < 0.2:
        steer = -1/sensor_loc[min_ind]
    else:
        steer = 0

    v = 1	#forward velocity
    kp = 0.5	#steering gain
    vl = v + kp * steer
    vr = v - kp * steer
    print("V_l =", vl)
    print("V_r =", vr)

    #Save the camera frame, rotate, and convert to BGR
    _, resolution, image = vrep.simxGetVisionSensorImage(clientID, camHandle, 0, vrep.simx_opmode_buffer)
    img = np.array(image, dtype = np.uint8)
    img.resize([resolution[0], resolution[1], 3])
    img = np.rot90(img,2)
    img = np.fliplr(img)
    img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)

    # Convert image to HSV and detect the color
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    lowGreen = np.array([49,50,50], dtype = np.uint8)
    highGreen = np.array([80, 255, 255], dtype = np.uint8)
    mask = cv2.inRange(hsv, lowGreen, highGreen)

    # -- Crop the image to view only the bottom border for tracking the path ( 10 pixels from the bottom - height)
    cropimg = mask[118:128]
    pmoments = cv2.moments(cropimg)
    area = pmoments['m00']

    # if area == 0:
        # if (prevx > 64):
            #steerL += 1
        #else:
            #steerR += 1

    if area > 0:
        x = int(pmoments['m10']/pmoments['m00'])
        y = int(pmoments['m01']/pmoments['m00'])
        cv2.rectangle(cropimg, (x, y), (x+2, y+2),(0,0,255), 2)
        print("x = ", x)
        prevx = x

        if abs(x-64) < 5:
            steerR = 0
            steerL = 0
            print("Going Straight")

        # Moves the motors in the appropriate directions
        elif x > 69:
            steerL += 1
            print("Turn right")
        elif x < 59:
            steerR += 1
            print("Turn left")

        vl = vl + .15 * steerL
        vr = vr + .15 * steerR
        vrep.simxSetJointTargetVelocity(clientID, left_motor_handle,vl,vrep.simx_opmode_streaming)
        vrep.simxSetJointTargetVelocity(clientID, right_motor_handle,vr,vrep.simx_opmode_streaming)


    cv2.imshow('Image', img)
    cv2.imshow('Mask', mask)

    cv2.imshow('Cropped Image', cropimg)

    key = cv2.waitKey(5) & 0xFF
    if key == 27:
        break

    ''' TODO: 1)Enable identification of the color green.
              2) Follow the green path. '''

    errorCode = vrep.simxSetJointTargetVelocity(clientID, left_motor_handle,vl, vrep.simx_opmode_streaming)
    errorCode = vrep.simxSetJointTargetVelocity(clientID, right_motor_handle,vr, vrep.simx_opmode_streaming)

    time.sleep(0.2) #loop executes once every 0.2 seconds (= 5 Hz)

#Post ALlocation
errorCode = vrep.simxSetJointTargetVelocity(clientID,left_motor_handle,0, vrep.simx_opmode_streaming)
errorCode = vrep.simxSetJointTargetVelocity(clientID,right_motor_handle,0, vrep.simx_opmode_streaming)
