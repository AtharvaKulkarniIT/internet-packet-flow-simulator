import React, { useState, useCallback } from 'react';
import ReactFlow, {
  addEdge,
  useNodesState,
  useEdgesState,
  MiniMap,
  Controls,
  Background,
} from 'reactflow';
import 'reactflow/dist/style.css';
import axios from 'axios';
import './RouterGraph.css';

function RouterGraph() {
  const [nodes, setNodes, onNodesChange] = useNodesState([]);
  const [edges, setEdges, onEdgesChange] = useEdgesState([]);
  const [src, setSrc] = useState("");
  const [sink, setSink] = useState("");
  const [algo, setAlgo] = useState('dinic');
  const [simOutput, setSimOutput] = useState("");

  const addRouter = () => {
    const id = `router-${nodes.length}`;
    const newNode = {
      id,
      data: { label: `Router ${nodes.length}` },
      position: { x: Math.random() * 500, y: Math.random() * 500 },
    };
    setNodes(nds => nds.concat(newNode));
  };

  const onNodeClick = useCallback((event, node) => {
    setNodes(nds => nds.filter(n => n.id !== node.id));
    setEdges(eds => eds.filter(e => e.source !== node.id && e.target !== node.id));
  }, [setNodes, setEdges]);

  const loadExample = (type) => {
    if (type === "simple") {
      setNodes([
        { id: "router-0", data: { label: "Router 0" }, position: { x: 100, y: 100 } },
        { id: "router-1", data: { label: "Router 1" }, position: { x: 300, y: 100 } },
        { id: "router-2", data: { label: "Router 2" }, position: { x: 500, y: 100 } }
      ]);
      setEdges([
        { id: "e0-1", source: "router-0", target: "router-1", label: "10" },
        { id: "e1-2", source: "router-1", target: "router-2", label: "10" }
      ]);
      setSrc("router-0");
      setSink("router-2");
    } else if (type === "complex") {
      setNodes([
        { id: "router-0", data: { label: "Router 0" }, position: { x: 100, y: 100 } },
        { id: "router-1", data: { label: "Router 1" }, position: { x: 300, y: 50 } },
        { id: "router-2", data: { label: "Router 2" }, position: { x: 300, y: 150 } },
        { id: "router-3", data: { label: "Router 3" }, position: { x: 500, y: 50 } },
        { id: "router-4", data: { label: "Router 4" }, position: { x: 500, y: 150 } },
        { id: "router-5", data: { label: "Router 5" }, position: { x: 700, y: 100 } },
      ]);
      setEdges([
        { id: "e0-1", source: "router-0", target: "router-1", label: "15" },
        { id: "e0-2", source: "router-0", target: "router-2", label: "10" },
        { id: "e1-3", source: "router-1", target: "router-3", label: "10" },
        { id: "e2-4", source: "router-2", target: "router-4", label: "15" },
        { id: "e3-5", source: "router-3", target: "router-5", label: "10" },
        { id: "e4-5", source: "router-4", target: "router-5", label: "10" },
        { id: "e1-4", source: "router-1", target: "router-4", label: "5" },
      ]);
      setSrc("router-0");
      setSink("router-5");
    } else if (type === "bottleneck") {
      setNodes([
        { id: "router-0", data: { label: "Router 0" }, position: { x: 100, y: 150 } },
        { id: "router-1", data: { label: "Router 1" }, position: { x: 300, y: 50 } },
        { id: "router-2", data: { label: "Router 2" }, position: { x: 300, y: 250 } },
        { id: "router-3", data: { label: "Router 3" }, position: { x: 500, y: 150 } },
        { id: "router-4", data: { label: "Router 4" }, position: { x: 700, y: 150 } },
      ]);
      setEdges([
        { id: "e0-1", source: "router-0", target: "router-1", label: "20" },
        { id: "e0-2", source: "router-0", target: "router-2", label: "20" },
        { id: "e1-3", source: "router-1", target: "router-3", label: "1" },
        { id: "e2-3", source: "router-2", target: "router-3", label: "20" },
        { id: "e3-4", source: "router-3", target: "router-4", label: "20" },
      ]);
      setSrc("router-0");
      setSink("router-4");
    }
  };

  const simulateFlow = () => {
    const n = nodes.length;
    const srcIdx = parseInt(src.split('-')[1], 10);
    const sinkIdx = parseInt(sink.split('-')[1], 10);
    const graphEdges = edges.map(edge => {
      const u = parseInt(edge.source.split('-')[1], 10);
      const v = parseInt(edge.target.split('-')[1], 10);
      const cap = Number(edge.label) || 10;
      return [u, v, cap];
    });

    let inputText = `${n} ${srcIdx} ${sinkIdx}\n`;
    graphEdges.forEach(edge => {
      inputText += `${edge[0]} ${edge[1]} ${edge[2]}\n`;
    });

    axios.post('http://localhost:5000/run-algo', { algo, input: inputText })
      .then(res => {
        try {
          const outputData = JSON.parse(res.data.output);
          setSimOutput(JSON.stringify(outputData, null, 2));
          const updatedEdges = edges.map(edge => {
            const edgeData = outputData.edgeFlows.find(ef =>
              parseInt(edge.source.split('-')[1], 10) === ef.u &&
              parseInt(edge.target.split('-')[1], 10) === ef.v
            );
            if (edgeData) {
              const ratio = edgeData.cap > 0 ? edgeData.flow / edgeData.cap : 0;
              const strokeWidth = 2 + ratio * 8;
              const strokeColor = ratio > 0.5 ? '#2ecc71' : '#3498db';
              return {
                ...edge,
                label: `${edgeData.flow}/${edgeData.cap}`,
                style: { stroke: strokeColor, strokeWidth: strokeWidth },
                animated: ratio > 0.3,
                markerEnd: {
                  type: 'arrowclosed',
                  color: strokeColor,
                },
              };
            }
            return edge;
          });
          setEdges(updatedEdges);
        } catch (e) {
          setSimOutput("Error parsing simulation output.");
        }
      })
      .catch(err => {
        setSimOutput("Simulation failed: " + err);
      });
  };

  return (
    <div className="graph-wrapper">
      <div className="controls">
        <button onClick={addRouter}>Add Router</button>
        <input type="text" placeholder="Source (router-x)" value={src} onChange={(e) => setSrc(e.target.value)} />
        <input type="text" placeholder="Sink (router-x)" value={sink} onChange={(e) => setSink(e.target.value)} />
        <select value={algo} onChange={e => setAlgo(e.target.value)}>
          <option value="dinic">Dinic</option>
          <option value="edmonds_karp">Edmonds-Karp</option>
          <option value="goldberg_tarjan">Goldberg-Tarjan</option>
          <option value="mcmf">MCMF</option>
        </select>
        <button onClick={simulateFlow}>Simulate</button>
        <button onClick={() => loadExample("simple")}>Simple</button>
        <button onClick={() => loadExample("complex")}>Complex</button>
        <button onClick={() => loadExample("bottleneck")}>Bottleneck</button>
      </div>

      <div className="graph-area">
        <ReactFlow
          nodes={nodes}
          edges={edges}
          onNodesChange={onNodesChange}
          onEdgesChange={onEdgesChange}
          onConnect={(params) => setEdges(eds => addEdge(params, eds))}
          onNodeClick={onNodeClick}
          fitView
        >
          <MiniMap />
          <Controls />
          <Background />
        </ReactFlow>
      </div>

      <div className="output-box">
        <h3>Simulation Output:</h3>
        <pre>{simOutput}</pre>
      </div>
    </div>
  );
}

export default RouterGraph;
