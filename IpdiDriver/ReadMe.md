
conda create -n ipdi312 python=3.12 -y
conda activate ipdi312
pip install /ruta/al/ipdi-1.3.4+ubuntu24.ip-cp312-cp312-linux_x86_64.whl  # o el .whl renombrado sin el '+ubuntu24.ip'
python -c "from ipdi.ip import aip, pyaip; print('OK', aip, pyaip)"

If you have this error 
export LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libstdc++.so.6

python main_IPM_PICORV32.py
