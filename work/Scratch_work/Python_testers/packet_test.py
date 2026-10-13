
import serial
import time



with serial.Serial("COM7" , 115200 , timeout=2) as ser:

    time.sleep(2);

    packet = bytes( [0xAA , 0x04 , 0x00 , 0 , 0 , 0 , 255])


    ser.write(packet)
    ser.flush()

    print("SENT: ", packet)

    r1 = ser.readline().decode('utf-8').rstrip()
    r2 = ser.readline().decode('utf-8').rstrip()
    r3 = ser.readline().decode('utf-8').rstrip()
    r4 = ser.readline().decode('utf-8').rstrip()
    print(f"Received from Arduino: {r1}")
    print(f"Received from Arduino: {r2}")
    print(f"Received from Arduino: {r3}")
    print(f"Received from Arduino: {r4}")


