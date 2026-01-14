import os
import tkinter as tk
from tkinter import ttk
import pandas as pd
import struct
import math
from IPDummyDriver import IPDIWrapperController
import struct

# Build the file path relative to this script's location
script_dir = os.path.dirname(os.path.abspath(__file__))
file_path = os.path.abspath(os.path.join(script_dir, "../ApiDataBase/AutoGenCode.xlsx"))

# Read the merged Excel file
data = pd.read_excel(file_path)

# Clean the data by removing rows with any '***'
cleaned_data = data[~data.apply(lambda row: row.astype(str).str.contains(r'\*\*\*').any(), axis=1)]

# Generate structured info with parameters and integrated descriptions
structured_info = {}
param_cols = ['P1_t', 'P2_t', 'P3_t', 'P4_t', 'P5_t']

for qt_label, group in cleaned_data.groupby('QT Label'):
    structured_info[qt_label] = []
    for _, row in group.iterrows():
        api_name = row["Callback"]
        description = row.get("API Description", "Description not found.")
        params = {col: row[col] for col in param_cols if pd.notna(row[col]) and row[col] != 'None'}
        structured_info[qt_label].append({
            "Callback": api_name,
            "Opcode": row["HEX OPCODE"],
            "Description": description,
            "Parameters": params
        })

class MainWindow(tk.Tk):
    def __init__(self,ipdiInstance):
        super().__init__()
        self.ipdiInstance = ipdiInstance
        self.title("LIMEGUI - LMS7002M")
        self.geometry("700x600")
        self.resizable(False, False)

        style = ttk.Style(self)
        style.theme_use('clam')
        style.configure('TLabel', font=('Arial', 12))
        style.configure('TButton', font=('Arial', 12, 'bold'), foreground='white', background='#0078D7')

        ttk.Label(self, text="QT Label:").pack(padx=10, pady=5, anchor="w")
        self.cmb_qt_label = ttk.Combobox(self, values=list(structured_info.keys()), state="readonly")
        self.cmb_qt_label.pack(fill='x', padx=10)
        self.cmb_qt_label.bind("<<ComboboxSelected>>", self.update_api_names)

        ttk.Label(self, text="Callback:").pack(padx=10, pady=5, anchor="w")
        self.cmb_api_name = ttk.Combobox(self, state="readonly")
        self.cmb_api_name.pack(fill='x', padx=10)
        self.cmb_api_name.bind("<<ComboboxSelected>>", self.show_details)

        self.lbl_opcode = ttk.Label(self, text="Opcode: ", foreground="blue")
        self.lbl_opcode.pack(padx=10, pady=5, anchor="w")

        self.lbl_description = ttk.Label(self, text="Descripción:", wraplength=560)
        self.lbl_description.pack(padx=10, pady=5, anchor="w")

        self.param_frame = ttk.Frame(self)
        self.param_frame.pack(padx=10, pady=10, fill='x', anchor="w")

        self.btn_send = ttk.Button(self, text="Enviar", command=self.send_data)
        self.btn_send.pack(pady=10)

        self.cmb_qt_label.current(0)
        self.update_api_names(None)

    def update_api_names(self, event):
        qt_label = self.cmb_qt_label.get()
        api_list = [api['Callback'] for api in structured_info[qt_label]]
        self.cmb_api_name.config(values=api_list)
        self.cmb_api_name.current(0)
        self.show_details(None)


    def _to_bytes_be(self,value):
        """Devuelve lista de bytes (0..255) en BIG-ENDIAN, sin padding."""
        if value is None:
            return [0]

        if isinstance(value, bool):
            return [1 if value else 0]

        if isinstance(value, (bytes, bytearray)):
            return list(value)

        if isinstance(value, int):
            return list(value.to_bytes(4, "big", signed=True))   # int32 BE

        if isinstance(value, float):
            return list(struct.pack(">d", value))                 # double 8B BE

        if isinstance(value, str):
            v = value.strip()
            low = v.lower()
            if low == "true":  return [1]
            if low == "false": return [0]
            if len(v) == 1:    return [ord(v) & 0xFF]            # 'A' -> [0x41]
            # ¿numérica?
            try:
                if "." not in v and "e" not in low:
                    as_int = int(v, 10)
                    return list(as_int.to_bytes(4, "big", signed=True))
                as_float = float(v)
                return list(struct.pack(">d", as_float))          # double
            except ValueError:
                return [ord(c) & 0xFF for c in v]

        # Fallback: texto
        return [ord(c) & 0xFF for c in str(value)]

    def _pad4_left(self,b):
        """Padding con ceros a la IZQUIERDA hasta múltiplo de 4 bytes."""
        need = (-len(b)) % 4
        return ([0]*need) + b

    def send_params_in_4byte_words_big_endian(self, opcode, params):
        """
        Envía: OPCODE (int32 BE) + cada parámetro (BE), alineando cada valor
        a 4 bytes con padding a la IZQUIERDA. Se escribe en palabras de 4 bytes.
        """
        stream = []

        # 1) OPCODE (int32 BE)
        stream += self._to_bytes_be(int(opcode))  # 4 bytes ya

        # 2) Parámetros (cada valor -> BE, luego pad-left a múltiplo de 4)
        for v in params.values():
            b = self._to_bytes_be(v)
            stream += self._pad4_left(b)

        # 3) Emitir en palabras de 4 bytes
        hex_words = []
        u32_words = []
        for i in range(0, len(stream), 4):
            word = stream[i:i+4]
            if len(word) < 4:
                word = self._pad4_left(word)
            hx = ''.join(f'{x:02X}' for x in word)  # your line
            hex_words.append(hx)
            u32_words.append(int(hx, 16))           # <-- uint32 from hex
        print(hex_words)
        print(u32_words)
        ipdiInstance.writeData(u32_words)

    def show_details(self, event):
        # Remove any previous widgets from the parameter frame
        for widget in self.param_frame.winfo_children():
            widget.destroy()

        # Define enum mappings for specific parameter types.
        # Keys are the enum type names (without the 'const' prefix) and values are a mapping of display text to their actual values.
        enum_mappings = {
            "LMS7002M_dir_t": {"LMS_TX": 1, "LMS_RX": 2},
            "LMS7002M_chan_t": {"LMS_CHA": "A", "LMS_CHB": "B", "LMS_CHAB": "C"},
            "LMS7002M_port_t": {"LMS_PORT1": 1, "LMS_PORT2": 2}
        }

        qt_label = self.cmb_qt_label.get()
        api_name = self.cmb_api_name.get()
        api_info = next((api for api in structured_info[qt_label] if api['Callback'] == api_name), None)

        if api_info:
            self.lbl_opcode.config(text=f"Opcode: 0x{api_info['Opcode']}")
            self.lbl_description.config(text=f"Descripción: {api_info['Description']}")

            self.param_entries = {}
            for idx, (param, param_type) in enumerate(api_info.get('Parameters', {}).items()):
                ttk.Label(self.param_frame, text=f"{param} ({param_type}):").grid(row=idx, column=0, sticky="w", padx=5)
                # If the type contains 'double', use a slider as before.
                if 'double' in param_type:
                    slider = ttk.Scale(self.param_frame, from_=0, to=1000, orient='horizontal')
                    slider.grid(row=idx, column=1, padx=5, pady=2, sticky='ew')
                    value_label = ttk.Label(self.param_frame, text="000.00")
                    slider.config(command=lambda val, l=slider, lbl=value_label: lbl.config(text=f"{float(val):.2f}".zfill(6)))
                    value_label.grid(row=idx, column=2, padx=10, pady=5, sticky='ew')
                    ttk.Label(self.param_frame, text="Escala:").grid(row=idx, column=3, padx=5)
                    unit = ttk.Combobox(self.param_frame, values=['', 'K', 'M', 'G'], width=4)
                    unit.grid(row=idx, column=4, padx=5)
                    unit.current(0)
                    self.param_entries[param] = (slider, unit, "slider")
                # If the parameter type contains one of our enum types, create a combobox with the corresponding options.
                elif any(enum_key in param_type for enum_key in enum_mappings):
                    matching_enum = None
                    for enum_key in enum_mappings:
                        if enum_key in param_type:
                            matching_enum = enum_key
                            break
                    if matching_enum:
                        options = list(enum_mappings[matching_enum].keys())
                        combo = ttk.Combobox(self.param_frame, values=options, state="readonly")
                        combo.grid(row=idx, column=1, padx=5, pady=2, sticky='ew')
                        combo.current(0)
                        # Store the combobox along with the matching enum type key and a flag "enum"
                        self.param_entries[param] = (combo, matching_enum, "enum")
                else:
                    # For any other type, use a standard entry widget.
                    entry = ttk.Entry(self.param_frame)
                    entry.grid(row=idx, column=1, padx=5, pady=2, sticky='ew')
                    self.param_entries[param] = entry

            self.param_frame.columnconfigure(1, weight=1)

    def send_data(self):
        qt_label = self.cmb_qt_label.get()
        api_name = self.cmb_api_name.get()
        api_info = next((api for api in structured_info[qt_label] if api['Callback'] == api_name), None)

        # Define the same enum mappings to convert display text to their corresponding value.
        enum_mappings = {
            "LMS7002M_dir_t": {"LMS_TX": 1, "LMS_RX": 2},
            "LMS7002M_chan_t": {"LMS_CHA": "A", "LMS_CHB": "B", "LMS_CHAB": "C"},
            "LMS7002M_port_t": {"LMS_PORT1": 1, "LMS_PORT2": 2}
        }

        if api_info:
            params = {}
            for param, widget_info in self.param_entries.items():
                # Check if widget_info is a tuple (indicating slider or enum)
                if isinstance(widget_info, tuple):
                    # For slider widgets
                    if widget_info[-1] == "slider":
                        slider, unit, _ = widget_info
                        val = slider.get()
                        multiplier = {'': 1, 'K': 1e3, 'M': 1e6, 'G': 1e9}.get(unit.get(), 1)
                        params[param] = round(val * multiplier, 2)
                    # For enum combobox widgets
                    elif widget_info[-1] == "enum":
                        combo, p_type, _ = widget_info
                        selected = combo.get()
                        params[param] = enum_mappings[p_type][selected]
                else:
                    # For standard entry widgets
                    params[param] = widget_info.get()
            print(f"Opcode: 0x{api_info['Opcode']}, Parameters: {params}")
            opcode = int(f"0x{api_info['Opcode']}",16)
            self.send_params_in_4byte_words_big_endian(opcode,params)


if __name__ == "__main__":
    ipdiInstance = IPDIWrapperController()
   # Select = [0x00000010]
   # print('Write configuration register: CCONFIG')
   # aip.writeConfReg('CCONFIG', Select, 1, 0)
   # print(f'Select Data: {[f"{x:08X}" for x in Select]}\n')
    app = MainWindow(ipdiInstance)
    app.mainloop()
    ipdiInstance.finish()
