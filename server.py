from flask import Flask, request, jsonify
from flask_cors import CORS
import subprocess
import os

app = Flask(__name__)
CORS(app)

@app.route('/run-algo', methods=['POST'])
def run_algo():
    data = request.get_json()
    algo = data.get('algo', 'dinic')
    input_text = data.get('input', '')
    
    with open('input.txt', 'w') as f:
        f.write(input_text)
    
    exe_path = os.path.join('backend', 'flow_solver.exe')
    try:
        result = subprocess.run(
            [exe_path, algo],
            stdin=open('input.txt', 'r'),
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        output = result.stdout
    except Exception as e:
        output = str(e)
    
    return jsonify({'output': output})

if __name__ == '__main__':
    app.run(port=5000)
