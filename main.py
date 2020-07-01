# v1.1.0
from SerialController import *
from flask import Flask, render_template
import qrcode
import socket


def main():
    manager = Manager()
    if len(manager.devices) < 2:
        print('No devices connected.')
        return -1
    i = 0
    for name in manager.devices:
        print(i, ' - ', name)
        i += 1
    print('\nEnter the id to connect:')
    i = int(input('Serial port '))
    while i < 1 or i >= len(manager.devices):
        i = int(input('Invalid, try again: '))
    manager.run(i)

    i = 0
    adds = socket.gethostbyname_ex(socket.gethostname())[2]
    for name in adds:
        print(i, ' - ', name)
        i += 1
    print('\nEnter the id to select proper IP:')
    i = int(input('Number '))
    while i < 0 or i > len(adds):
        i = int(input('Invalid, try again: '))
    ip = adds[i]

    url = 'http://' + str(ip) + ':5000/fromcchqr'
    qr = qrcode.QRCode(version=1)
    qr.add_data(url)
    qr.make(fit=True)
    img = qr.make_image(fill='black', back_color='white')
    img.save('static/index.png')

    web = Flask(__name__)

    @web.route('/<path:arg>')
    def index(arg: str):
        if arg == 'fromcchqr':
            return render_template('index.html')
        elif arg == 'qr':
            return render_template('qr.html')
        elif arg == 'controller':
            return render_template('controller.html')
        elif arg == 'verify':
            return 'This is actually the CCH WebUI.'
        elif arg.split('/')[0] == 'capture':
            manager.invoke(arg.split('/')[1])
            return 'raw:succeed'
        return 'Error: please scan the QR code to begin.'
    web.run(host='0.0.0.0')


if __name__ == '__main__':
    main()
