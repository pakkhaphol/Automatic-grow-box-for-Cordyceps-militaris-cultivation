# import the opencv library
import numpy as np
import cv2
from datetime import datetime
import time
import os
#from firebase import Firebase
import pyrebase
import serial
from serial.serialutil import Timeout
from imutils import paths
import argparse

timeforsaveimage = 1


#setup firebase
firebaseConfig = {
    'apiKey': "AIzaSyA3VOpnive9vivtiHpi5mN1sVls-TzE-x8",
    'authDomain': "mysaveimage-e0e76.firebaseapp.com",
    'projectId': "mysaveimage-e0e76",
    'storageBucket': "mysaveimage-e0e76.appspot.com",
    'messagingSenderId': "376702317439",
    'appId': "1:376702317439:web:c162e3389d07afc37ba04a",
    'measurementId': "G-2C1C9NPKVR",
    "databaseURL" : ""
  }
firebase = pyrebase.initialize_app(firebaseConfig) 


#storage
storage = firebase.storage()



# Image path
image_path = r'/home/pi/Desktop/saveimage'
#for save in computer
# Image directory
directory = r'/home/pi/Desktop/saveimage'
#diretory2="C:/Users/sai/Desktop/saveimages/"
# define a video capture object
#vid.set(cv2.CAP_PROP_A0UTOFOCUS,255)


while(True):
    
   #"""Opening of the serial port"""
    try:

        relay = serial.Serial("/dev/ttyACM0",9600,timeout=1) #relay and dht22
        time.sleep(3)

    except:
        pass


    maxtemp=20
    mintemp=17
    relay.write(b'%s'%str(maxtemp).encode())

    time.sleep(3)
    relay.write(b'%s'%str(mintemp).encode())
    
    time.sleep(3)
###########################################

    
    #growlightcontrol(start_hour,start_minute,end_hour,end_minute)
    #time.sleep(3)
    relay.write('oncnc\n'.encode())
    time.sleep(15)
    relay.write('offlightfortree\n'.encode())
    time.sleep(3)

    
   # relay.write('onlightforcam\n'.encode())
    ##
    
    print("state 1")
    
    i=1

      
    while i <=9 :
        vid = cv2.VideoCapture(-1) #, cv2.CAP_DSHOW
        vid.set(3,1280)
        vid.set(4,720)  
        
        
        print(i)

        relay.write(b'%s'%str(i).encode())
        ##
        time.sleep(15)
        print("cnc on")
          

        while True:
            try :
                 relay = serial.Serial("/dev/ttyACM0",9600,timeout=1)
                 time.sleep(3)
#         
            except :
                 pass

            ret, frame = vid.read()                
            def variance_of_laplacian(frame):
                return cv2.Laplacian(frame, cv2.CV_64F).var()
            gray = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
            #gray=cv2.imread(frame, cv2.IMREAD_GRAYSCALE)
            fm = variance_of_laplacian(gray)
            print(fm)

            if fm>15 :
                break
            else :
                print('non focus')
                relay.write('onlightforcam\n'.encode()) #12v
                time.sleep(3)
                
                start_time = time.time() 
                while time.time() - start_time < 2: 
                    ret, frame = vid.read()           
 
                relay.write('offlightforcam\n'.encode())
                
                time.sleep(3)
                start_time = time.time() 
                while time.time() - start_time < 7:
                    ret, frame = vid.read()
            
        now =datetime.now()
        #os.chdir(diretory2+"b"+str(i))
        os.chdir(directory)
        filename = 'bottle0%d_%d-%d-%d-%d-%d-%d.jpg'%( i,now.day ,now.month ,now.year ,now.hour ,now.minute,now.second) 
        print(filename)
        frame2=cv2.transpose(frame);
        cv2.imwrite(filename,frame2)
        time.sleep(timeforsaveimage)
        storage.child("bottle0%d/%s" %(i,filename)).put("/home/pi/Desktop/saveimage/%s" %filename)
        #storage.child("bottle0%d/%s" %(i,filename)).put("C:/Users/sai/Desktop/saveimages/b%d/%s" %(i,filename)
        time.sleep(5)
        print('done') 
        
        
       # print(str(relay.readline()))
        i+=1
        vid.release()
        cv2.destroyAllWindows()
       # relay.close()

        
   
#     try :
#         relay = serial.Serial("/dev/ttyACM0",9600,timeout=1)
#         
#     except :
#         pass
    a=0
    relay.write( b'%s\n'%str(a).encode())
    #
    time.sleep(15)
    
   # print(relay.readline().decode())
    relay.write( 'offcnc\n'.encode())
    time.sleep(3)
    

    
    print("state 2")
    
    #relay.close()
    #relay.close()
    #vid.release()
    cv2.destroyAllWindows()
  #  print(relay.readline().decode('utf-8'))
    start_time = time.time() 
    while time.time() - start_time < 900:
        
        
        now=datetime.now()
        start_hour=0
        start_minute=00
    #################
        end_hour =22
        end_minute=20
        
        if (now.hour >= start_hour ) and (now.hour<= end_hour)   :
            print('on')
            relay.write('onlightfortree\n'.encode()) #12v
        #relay.flush()
            time.sleep(3)
        else  :
            print('off')
            relay.write('offlightfortree\n'.encode()) #12v
        #relay.flush()
            time.sleep(3)
        



        
               
# After the loop release the cap object
vid.release()
# Destroy all the windows
cv2.destroyAllWindows()
print('off')
bottle='0'
#arduino2.write( relay.encode())
#relay.write( bottle.encode())
time.sleep(100)
#relay.close()

