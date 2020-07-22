import serial
import time

def update_rf_amount(comport, num_treats):
    if comport.isOpen():
        command = 'CB' + str(num_treats) + 'E'
        time.sleep(2)
        comport.write(command.encode('ascii') + b'13' + b'10')
    return comport.read()

def dispense_treats(comport, num_treats):
    if comport.isOpen():
        command = 'CD' + str(num_treats) + 'E'
        time.sleep(2)
        comport.write(command.encode('ascii') + b'13' + b'10')
    return comport.read()

ser = serial.Serial('COM31', baudrate=9600, timeout=0)
update_rf_amount(ser, 2)

while True:
    cmd = input("Enter command or 'exit': ")
    if cmd == 'exit':
        ser.close()
        exit()
    else:
        # command + \r\n
        print(cmd)
        dispense_treats(ser, cmd)
        out = ser.read()
        if out == b'\x01':
            print('Invalid command, please retry.')
        if out == b'\x00':
            print('Successful command.')
        ser.flush()
