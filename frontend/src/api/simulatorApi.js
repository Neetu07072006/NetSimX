const API_URL=import.meta.env.VITE_API_URL||"http://localhost:5000/api";
async function request(endpoint,options={}){
const response=await fetch(`${API_URL}${endpoint}`,options);
const data=await response.json().catch(()=>({}));
if(!response.ok)throw new Error(data.error||"API request failed");
return data;
}
export async function getSimulatorSnapshot(){return request("/snapshot");}
export async function getHealth(){return request("/health");}
export async function getMetrics(){return request("/metrics");}
export async function getSecurity(){return request("/security");}
export async function getTopology(){return request("/topology");}
export async function getRouting(){return request("/routing");}
export async function runSimulation(type="full"){
return request("/run-simulation",{
method:"POST",
headers:{"Content-Type":"application/json"},
body:JSON.stringify({type})
});
}
export async function getSimulatorSnapshot(){return request("/snapshot");}