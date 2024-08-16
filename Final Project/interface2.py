import tkinter as tk
import serial
import threading

class SerialInterface(tk.Tk):
    def __init__(self):
        super().__init__()

        self.title("Interface Serial Arduino")
        self.geometry("300x230")

        try:
            self.serial_port = serial.Serial("COM10", 9600, timeout=1)  # Substitua "COM10" pela porta serial correta
        except serial.SerialException as e:
            print("Failed to open serial port:", e)
            self.destroy()  # Close the application if serial port cannot be opened
            return

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

        self.protocol("WM_DELETE_WINDOW", self.on_closing)  # Call on_closing when window is closed

    def read_serial(self):
        with open("received_data.txt", "a") as file:  # Abrir arquivo para adicionar
            while True:
                if self.serial_port.in_waiting > 0:
                    received_data = self.serial_port.readline().decode('latin-1').strip()  # Decodificar a string recebida e remover espaços em branco
                    file.write(received_data + '\n')  # Escrever a string lida no arquivo
                    file.flush()  # Forçar a gravação dos dados no arquivo
                    self.textbox_receive.insert(tk.END, received_data + '\n')
                    self.textbox_receive.see(tk.END)  # Rolagem automática para exibir o texto mais recente


    def extract_data(self, received_data):
        return received_data.strip()

    def send_data(self):
        data = self.entry.get()
        if data:
            self.serial_port.write(data.encode())
            self.entry.delete(0, tk.END)  # Limpa o campo de entrada após o envio

    def on_closing(self):
        if hasattr(self, 'serial_port') and self.serial_port.is_open:
            self.serial_port.close()
        self.destroy()

if __name__ == "__main__":
    app = SerialInterface()
    app.mainloop()