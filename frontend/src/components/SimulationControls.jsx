import {useState} from "react";
import {runSimulation} from "../api/simulatorApi";

export default function SimulationControls({onComplete}){
const [running,setRunning]=useState(false);
const [message,setMessage]=useState("");

async function handleRun(){
if(running)return;
setRunning(true);
setMessage("Running C++ simulation...");
try{
const result=await runSimulation();
setMessage(result.message||"Simulation completed.");
if(onComplete)await onComplete(result.snapshot);
}catch(error){
setMessage(error.message||"Simulation failed.");
}finally{
setRunning(false);
}
}

return <section className="simulation-controls">
<div className="control-header">
<div>
<h2>Simulation Control</h2>
<p>Execute the NetSimX C++ engine and refresh the dashboard.</p>
</div>
<button className="run-btn" onClick={handleRun} disabled={running}>
{running?"Running...":"Run Simulation"}
</button>
</div>
<div className="simulation-status">
<span className={running?"status-running":"status-ready"}></span>
{running?"C++ simulation is running":"C++ engine ready"}
{message&&<span className="simulation-message">{message}</span>}
</div>
</section>;
}