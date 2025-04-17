## 📦 Internet Packet Flow Simulator  
Visualize and simulate internet packet routing using graph-based Max-Flow algorithms. Built using **React + React Flow** for frontend and **C++** for high-performance backend algorithms served via **Flask** `server.py`

---

### 🔧 Setup Instructions

#### 1. Clone the Repository  
```bash
git clone https://github.com/AtharvaKulkarniIT/internet-packet-flow-simulator.git
cd internet-packet-flow-simulator
```

---

#### 2. Compile C++ Algorithms  
Make sure you have `g++` installed.

```bash
g++ backend/dinic.cpp -o bin/dinic
g++ backend/edmonds_karp.cpp -o bin/edmonds_karp
g++ backend/goldberg_tarjan.cpp -o bin/goldberg_tarjan
g++ backend/mcmf.cpp -o bin/mcmf
```

---

#### 3. Install React Frontend  
```bash
cd frontend
npm install
```

---

#### 4. Start the Flask Server  
Make sure Python and Flask are installed.

```bash
cd server
python server.py
```

---

#### 5. Start the Frontend  
In another terminal:

```bash
cd frontend
npm run dev
```

---

### 🖥️ Recommended Split Terminal Setup

Split your terminal into 3 panes:

```bash
# Pane 1 - Flask Server
cd server
python server.py

# Pane 2 - Compile C++ once
cd backend
g++ dinic.cpp -o ../bin/dinic && g++ edmonds_karp.cpp -o ../bin/edmonds_karp
g++ goldberg_tarjan.cpp -o ../bin/goldberg_tarjan && g++ mcmf.cpp -o ../bin/mcmf

# Pane 3 - React App
cd frontend
npm run dev
```

---

## 🚀 Project Overview

- Interactive UI to **add/delete routers (nodes)** and **connections (edges)**
- Select **source/destination nodes**
- Run simulations using **Dinic’s**, **Edmonds-Karp**, **Goldberg-Tarjan**, or **MCMF**
- **Visual flow animation** on the React Flow canvas
- Backed by **high-performance C++** for real-time simulation

---

## ⚙️ Algorithms

| Algorithm         | Description                                  | Time Complexity            | Space Complexity         |
|------------------|----------------------------------------------|----------------------------|--------------------------|
| **Dinic’s**       | Uses BFS + layered DFS to send flow          | `O(V^2 * E)`               | `O(V + E)`               |
| **Edmonds-Karp**  | BFS-based Ford-Fulkerson                     | `O(V * E^2)`               | `O(V + E)`               |
| **Goldberg-Tarjan (Push-Relabel)** | Uses height + excess flow for max flow | `O(V^2 * sqrt(E))`         | `O(V^2)`                 |
| **MCMF**          | Finds min-cost max-flow using SPFA/Dijkstra | `O(F * E * logV)`          | `O(V + E)`               |

---

## 📁 Project Structure

```bash
internet-packet-simulator/
├── backend/
│   ├── dinic.cpp
│   ├── edmonds_karp.cpp
│   ├── goldberg_tarjan.cpp
│   ├── mcmf.cpp
│   ├── *.h
├── bin/                 # Compiled binaries
│   ├── dinic
│   └── ...
├── server/
│   └── server.py
├── frontend/
│   ├── src/
│   ├── public/
│   ├── index.html
│   └── vite.config.js
```

---

## ✅ To-Do / Enhancements

- [ ] Add Orlin’s algorithm 
- [ ] Save/load graph topology
- [ ] Dark mode toggle
- [ ] Edge weight slider

---

![Goldberg-Tarjan](https://drive.google.com/uc?export=view&id=1EO94TVLGvs_zFypz4iOCCLIHQD0z7iFZ) 
