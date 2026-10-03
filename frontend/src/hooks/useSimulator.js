import {useCallback,useEffect,useState} from "react";
import {getSimulatorSnapshot} from "../api/simulatorApi";
export function useSimulator(){
const [data,setData]=useState(null);
const [loading,setLoading]=useState(true);
const [error,setError]=useState("");
const load=useCallback(async()=>{
try{
setError("");
const result=await getSimulatorSnapshot();
setData(result);
}catch(e){
setError(e.message||"Unable to connect to NetSimX API");
}finally{
setLoading(false);
}
},[]);
useEffect(()=>{
load();
const timer=setInterval(load,3000);
return()=>clearInterval(timer);
},[load]);
return {data,loading,error,refresh:load};
}