import serial
import serial.tools.list_ports
import _thread


class Hardware:
    s = serial.Serial()

    def __init__(self, port: str):
        self.s.port = port
        self.s.baudrate = 115200
        self.s.open()

    def invoke(self, data: str):
        self.s.write(data.encode())

    @classmethod
    def find(cls) -> list:
        availables = ["请选择设备"]
        for device in serial.tools.list_ports.comports():
            availables.append(device.device)
        return availables


class Manager:
    arduino = None
    port = None
    devices = []

    def __init__(self):
        self.devices = Hardware.find()

    def run(self, port: int):
        self.port = self.devices[port]
        _thread.start_new_thread(self.arduinoLoop, ())

    def arduinoLoop(self):
        try:
            self.arduino = Hardware(port=self.port)
        except:
            exit(-2)

    def invoke(self, data: str):
        self.arduino.invoke(data)
