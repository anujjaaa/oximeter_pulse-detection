import serial                                                              #Serial imported for Serial communication
import time                                                                #Required to use delay functions
from statistics import mean
from datetime import datetime
ArduinoUnoSerial = serial.Serial('/dev/ttyACM0', 115200)       #Create Serial port object called ArduinoUnoSerialData time.sleep(2)                                                             #wait for 2 secounds for the communication to get established
value=input("enter any value")
print(bytes(int(value)))
ArduinoUnoSerial.write(bytes(int(value)))
print("Arduino code has started")
# print(ArduinoUnoSerial.readline())                            #read the serial data and print it as line
print("Place your finger now")
time.sleep(5)

HR_list = []
o2_list = []

start_time = datetime.now()
while 1:

    file1 = open(" analysis.txt", 'a')

    #Do this forever
    var = ArduinoUnoSerial.readline()
    var1 = var.decode('ASCII')
    # print(var1)
    if len(HR_list) < 5:
        if var1[:3] =='HR:':
            if var1[3:] > str(50.00):
                HR_list.append(float(var1[3:]))
            # print(HR_list)

        elif var1[:5] == 'SpO2:':
            if var1[5:] > str(80.00):
                o2_list.append(float(var1[5:]))
            # print(o2_list)
        else:
            continue
            #print('-------')



    else:
        end_time = datetime.now()
        total_time= (end_time - start_time).total_seconds()

        print('Duration: {}'.format(total_time))
        if var1 == 'Remove your finger':
            print(var1)
        file1.write(str(HR_list))
        file1.write("\n")
        file1.write(str(o2_list))
        file1.write("\n")
        HeartRate = mean(HR_list)
        spo2 = max(o2_list)
        file1.write("*****************************")
        # print("******************************")
        print("heartrate:  ", HeartRate )
        print('\n')
        print("spo2:  ", spo2)
        print('\n')
        file1.write("Heart Rate :")
        file1.write(str(HeartRate))
        file1.write("\n ")
        file1.write("SPO2:")
        file1.write(str(spo2))
        file1.write("\n ")
        file1.write("Total time : ")
        file1.write(str(total_time))
        file1.write("\n ")
        file1.write("End")
        file1.write("\n \n \n")
        # time.sleep(5)
        HR_list=[]
        o2_list=[]
        print("now you can place the finger")
        time.sleep(5)
        print("Hope you have placed your finger!")
        start_time = datetime.now()

        file1.close()
        break