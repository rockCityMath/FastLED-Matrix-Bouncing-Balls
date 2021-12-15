# How to use!
Simply install the FastLED library, define the matrix height and width along with the other defines at the top of the file and it should be ready for upload to the Arduino.    

**Note**    
By default the program will use pin 3 as an analog potentiometer input used for controlling the speed of the balls, but you can manually set the speed (delayAmount) at the bottom of the program.      

**The Ball object**   
The program provides a simple Ball object that contains the x & y coordinates along with the x & y velocity amounts, the class also provides the updateBall() method that moves the ball to its next position on the matrix.    
A ball should be created and initialized outside the main loop with:     
> Ball ballName(uint8_t x, uint8_t y, uint8_t vx, uint8_t vy)        

Inside the main loop you must:
- Call the updateBall() method on each ball object.
- Calculate the index of the ball with the XY Matrix
- Set the index to the desired ball color
- FastLED.show()
