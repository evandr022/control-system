import tkinter as tk
import serial
import threading
import numpy as np

class SerialInterface(tk.Tk):
    def __init__(self):
        super().__init__()

        self.title("Interface Serial Arduino")
        self.geometry("300x230")

        self.serial_port = serial.Serial("COM10", 9600, timeout=1)  # Substitua "COM10" pela porta serial correta
        self.serial_thread = threading.Thread(target=self.read_serial)
        self.serial_thread.daemon = True
        self.serial_thread.start()

        self.label_receive = tk.Label(self, text="Dados recebidos do Arduino:")
        self.label_receive.pack(pady=5)

        self.textbox_receive = tk.Text(self, height=5, width=30)
        self.textbox_receive.pack(pady=5)

        self.label_send = tk.Label(self, text="Envie dados para o Arduino:")
        self.label_send.pack(pady=5)

        self.entry = tk.Entry(self)
        self.entry.pack(pady=5)

        self.send_button = tk.Button(self, text="Enviar", command=self.send_data)
        self.send_button.pack(pady=5)

    def read_serial(self):
        while True:
            if self.serial_port.in_waiting > 0:
                received_data = self.serial_port.readline().decode('latin-1').strip()  # Altere para a codificação correta
                self.textbox_receive.insert(tk.END, received_data + '\n')
                
                self.textbox_receive.see(tk.END)  # Rolagem automática para exibir o texto mais recente

    def send_data(self):
        data = self.entry.get()
        if data:
            self.serial_port.write(data.encode())
            self.entry.delete(0, tk.END)  # Limpa o campo de entrada após o envio

if __name__ == "__main__":
    app = SerialInterface()
    app.mainloop()

np.savetxt()