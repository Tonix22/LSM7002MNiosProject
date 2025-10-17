
conda create -n ipdi312 python=3.12 -y
conda activate ipdi312
pip install /ruta/al/ipdi-1.3.4+ubuntu24.ip-cp312-cp312-linux_x86_64.whl  # o el .whl renombrado sin el '+ubuntu24.ip'
python -c "from ipdi.ip import aip, pyaip; print('OK', aip, pyaip)"

If you have this error 
```
ImportError: /home/tonix/anaconda3/envs/ipdi312/bin/../lib/libstdc++.so.6: version `GLIBCXX_3.4.32' not found (required by /home/tonix/anaconda3/envs/ipdi312/lib/python3.12/site-packages/ipdi/ip/_aip.cpython-312-x86_64-linux-gnu.so)
```
Install the library 
```
conda install -c conda-forge libstdcxx-ng
```
Or run this

```
export LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libstdc++.so.6
```

python main_IPM_PICORV32.py
