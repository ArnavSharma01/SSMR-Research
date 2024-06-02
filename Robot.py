import RPi.GPIO as GPIO
import pygame
import os
import time

#from picamera2 import Picamera2
#from libcamera import controls
from datetime import datetime

#camera = Picamera2()
#camera.preview_configuration.enable_lores()
#camera.preview_configuration.main.size = (1920,1080)
#camera.preview_configuration.lores.size = (480,360)
#camera.configure("preview")
#os.system("v4l2-ctl --set-ctrl wide_dynamic_range=0 -d /dev/v4l-subdev0")

screen = pygame.display.set_mode([240,160])

GPIO.setmode(GPIO.BOARD)
GPIO.setup(7,GPIO.OUT)
GPIO.setup(11,GPIO.OUT)
GPIO.setup(13,GPIO.OUT)
GPIO.setup(15,GPIO.OUT)
GPIO.setup(29,GPIO.OUT)

for x in range(1,5):
    GPIO.output(29,False)
    time.sleep(0.5)
    GPIO.output(29,True)
    time.sleep(0.5)

try:
    while True:
        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN:
                
                if event.key == pygame.K_q:
                    pygame.quit()

                elif event.key == pygame.K_p:
                    os.system("sudo shutdown now")

                elif event.key == pygame.K_r:
                    #camera.start(show_preview=True)
                    #camera.set_controls({"AfMode": controls.AfModeEnum.Continuous})
                    print("Streaming starting")

                elif event.key == pygame.K_t:
                    #camera.stop_preview()
                    #camera.stop()
                    print("Streaming stopped")

                elif event.key == pygame.K_d:
                    print("Turning Right")
                    GPIO.output(7, True)
                    GPIO.output(11, False)
                    GPIO.output(13, False)
                    GPIO.output(15, True)

                elif event.key == pygame.K_a:
                    print("Turning Left")
                    GPIO.output(7, False)
                    GPIO.output(11, True)
                    GPIO.output(13, True)
                    GPIO.output(15, False)

                elif event.key == pygame.K_w:
                    print("Moving Forward")
                    GPIO.output(7, False)
                    GPIO.output(11, True)
                    GPIO.output(13, False)
                    GPIO.output(15, True)

                elif event.key == pygame.K_s:
                    print("Moving Backward")
                    GPIO.output(7, True)
                    GPIO.output(11, False)
                    GPIO.output(13, True)
                    GPIO.output(15, False)
                

            elif event.type == pygame.KEYUP:
                GPIO.output(7, False)
                GPIO.output(11, False)
                GPIO.output(13, False)
                GPIO.output(15, False)

finally:
    GPIO.cleanup()
                
                
                
