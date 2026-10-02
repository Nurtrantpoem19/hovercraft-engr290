

## HERE is LIst of everything we need to do and understan

# REMINDER: MAKE SMALL PRs, makes it easier to review and test, do not have huge commits, one function, one feature, one bug fix, etc...

### TASK 1. Set up infrared sensor
- we need to undrstand how ADC works so that we can read sensor data
- we need to understand how UART works so we can setup and transmit data
- we need to know which registers we need to configure so that PC0 is used as an ANALOG pin
- configure ADMUX register to the ncessary so that we are reading ADC0 pin (see chapter 28 of atmel datasheet)
- also various other registers need to be configured so that ADC reading will work (see chapter 28, such as register A or register B etc...)
- work in include/sensors_IR.h and src/sensors_IR.c, start a test_sensorsIR.c so that we can be sure that the readings are correct

### TASK 2. CMAKE
- just make sure all the source and include files are able to work properly
- also add tests if necessary to the cmake


### TASK 3. set up ultrasonic sensor
- make an adc switch api that switches the adc reader to the specific pin, or else we will be stuck reading from only one pin the whole time
- see TASK 1 for details, but the two are similar
- read the datasheet for the US sensor so that the readings are done correctly

### TASK 4. set up interrupts and the ISRs
- I think this is tied to both task 1 and 3. maybe the ISR will handle the ADCore switching logic, so that each time a new value comes in, the interrupt will switch the ad core?
- 
