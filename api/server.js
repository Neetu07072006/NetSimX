const express=require("express");
const cors=require("cors");
const fs=require("fs");
const path=require("path");
const{execFile}=require("child_process");

const app=express();
const PORT=5000;
const ROOT_PATH=path.resolve(__dirname,"..");
const EXE_PATH=process.platform==="win32"
?path.join(ROOT_PATH,"netsim.exe")
:path.join(ROOT_PATH,"netsim");
const SNAPSHOT_PATH=path.join(ROOT_PATH,"netsim_snapshot.json");
let simulationRunning=false;

app.use(cors());
app.use(express.json());

function readSnapshot(){
if(!fs.existsSync(SNAPSHOT_PATH))throw new Error("netsim_snapshot.json not found");
return JSON.parse(fs.readFileSync(SNAPSHOT_PATH,"utf8"));
}

app.get("/",(req,res)=>{
res.json({name:"NetSimX API",status:"running",snapshot:"/api/snapshot"});
});

app.get("/api/health",(req,res)=>{
res.json({status:"ok",service:"NetSimX API",simulationRunning});
});

app.get("/api/snapshot",(req,res)=>{
try{
res.json(readSnapshot());
}catch(error){
res.status(404).json({error:error.message});
}
});

app.get("/api/metrics",(req,res)=>{
try{
const data=readSnapshot();
res.json({
health:data.health,
packets:data.packets,
latency:data.latency,
queue:data.queue,
failures:data.failures
});
}catch(error){
res.status(404).json({error:error.message});
}
});

app.get("/api/security",(req,res)=>{
try{
const data=readSnapshot();
res.json({security:data.security,alerts:data.alerts});
}catch(error){
res.status(404).json({error:error.message});
}
});

app.get("/api/topology",(req,res)=>{
try{
const data=readSnapshot();
res.json({nodes:data.nodes,links:data.links});
}catch(error){
res.status(404).json({error:error.message});
}
});

app.get("/api/routing",(req,res)=>{
try{
const data=readSnapshot();
res.json({routes:data.routes});
}catch(error){
res.status(404).json({error:error.message});
}
});

app.post("/api/run-simulation",(req,res)=>{
if(simulationRunning){
return res.status(409).json({success:false,error:"Simulation is already running."});
}
if(!fs.existsSync(EXE_PATH)){
return res.status(404).json({success:false,error:"netsim.exe not found.",path:EXE_PATH});
}

const type=req.body?.type||"full";
const allowedTypes=["full","tcp","udp","icmp","firewall","ids","failure","queue"];

if(!allowedTypes.includes(type)){
return res.status(400).json({success:false,error:"Invalid simulation type.",allowedTypes});
}

const args=type==="full"?[]:[type];
simulationRunning=true;

execFile(EXE_PATH,args,{cwd:ROOT_PATH,windowsHide:true},(error,stdout,stderr)=>{
simulationRunning=false;

if(error){
console.error("Simulation error:",error);
console.error(stderr);
return res.status(500).json({
success:false,
type,
error:error.message,
output:stderr||stdout
});
}

try{
const snapshot=readSnapshot();
res.json({
success:true,
type,
message:`${type.toUpperCase()} simulation completed successfully.`,
snapshot
});
}catch(readError){
res.status(500).json({
success:false,
type,
error:readError.message
});
}
});
});

app.get("/api/simulation-status",(req,res)=>{
res.json({
running:simulationRunning,
executable:fs.existsSync(EXE_PATH),
snapshot:fs.existsSync(SNAPSHOT_PATH)
});
});

app.listen(PORT,()=>{
console.log("=================================");
console.log("NetSimX API");
console.log("=================================");
console.log("Server: http://localhost:"+PORT);
console.log("Executable: "+EXE_PATH);
console.log("Snapshot: "+SNAPSHOT_PATH);
console.log("=================================");
});