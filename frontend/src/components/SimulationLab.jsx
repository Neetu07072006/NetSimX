import{useState}from"react";
import{runSimulation}from"../api/simulatorApi";

export default function SimulationLab({onComplete}){
const[running,setRunning]=useState("");
const[message,setMessage]=useState("");

const actions=[
["full","Run Full Simulation","Execute the complete NetSimX simulation."],
["tcp","TCP Traffic","Generate and analyze TCP traffic."],
["udp","UDP Traffic","Generate and analyze UDP traffic."],
["icmp","ICMP Traffic","Generate and analyze ICMP traffic."],
["firewall","Firewall Test","Test firewall allow and deny rules."],
["ids","IDS Attack Test","Generate traffic patterns for IDS detection."],
["failure","Link Failure Test","Simulate network failure and recovery."],
["queue","Queue Test","Simulate router queue behavior."]
];

async function execute(type){
if(running)return;
setRunning(type);
setMessage("");
try{
const result=await runSimulation(type);
setMessage(result.message||"Simulation completed successfully.");
if(result.snapshot&&onComplete)await onComplete(result.snapshot);
}catch(error){
setMessage(error.message||"Simulation failed.");
}finally{
setRunning("");
}
}

return <section className="simulation-lab">
<div className="lab-header">
<div>
<span className="section-label">EXPERIMENTATION</span>
<h2>Simulation Lab</h2>
<p>Run individual network scenarios and observe the effect on NetSimX.</p>
</div>
<div className="lab-status"><span></span>{running?"Simulation Running":"Engine Ready"}</div>
</div>
<div className="lab-grid">
{actions.map(([id,title,description])=><div className="lab-card" key={id}>
<div className="lab-card-icon">{id==="full"?"▶":id==="tcp"?"T":id==="udp"?"U":id==="icmp"?"I":id==="firewall"?"F":id==="ids"?"S":id==="failure"?"⚡":"Q"}</div>
<div className="lab-card-content">
<h3>{title}</h3>
<p>{description}</p>
</div>
<button onClick={()=>execute(id)} disabled={!!running}>{running===id?"Running...":"Run"}</button>
</div>)}
</div>
{message&&<div className="lab-message">{message}</div>}
</section>;
}