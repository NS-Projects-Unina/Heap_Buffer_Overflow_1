import socket
import struct

# Configurazione
VITTIMA_IP = "192.168.161.128"
VITTIMA_PORT = 4444

def test_base():
    print("[*] Connessione al server...")
    
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((VITTIMA_IP, VITTIMA_PORT))
    print("[*] Connesso!")

    # Costruisce il pacchetto TYPE=1, LENGTH=5, DATA='hello'
    tipo   = b'\x01'                  # TYPE: 1 byte
    length = struct.pack('>I', 5)     # LENGTH: 4 byte big-endian
    data   = b'hello'                 # DATA: 5 byte

    # Invia il pacchetto
    print("[*] Invio pacchetto: TYPE=1, LENGTH=5, DATA='hello'")
    s.send(tipo)
    s.send(length)
    s.send(data)

    print("[*] Pacchetto inviato con successo!")
    s.close()
    print("[*] Connessione chiusa")

test_base()
