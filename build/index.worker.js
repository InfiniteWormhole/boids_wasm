var initializedJS=false;var Module={};function assert(condition,text){if(!condition)abort("Assertion failed: "+text)}function threadPrintErr(){var text=Array.prototype.slice.call(arguments).join(" ");console.error(text)}function threadAlert(){var text=Array.prototype.slice.call(arguments).join(" ");postMessage({cmd:"alert",text:text,threadId:Module["_pthread_self"]()})}var out=function(){throw"out() is not defined in worker.js."};var err=threadPrintErr;this.alert=threadAlert;Module["instantiateWasm"]=function(info,receiveInstance){var instance=new WebAssembly.Instance(Module["wasmModule"],info);receiveInstance(instance);Module["wasmModule"]=null;return instance.exports};function moduleLoaded(){}this.onmessage=function(e){try{if(e.data.cmd==="load"){Module["wasmModule"]=e.data.wasmModule;Module["wasmMemory"]=e.data.wasmMemory;Module["buffer"]=Module["wasmMemory"].buffer;Module["ENVIRONMENT_IS_PTHREAD"]=true;if(typeof e.data.urlOrBlob==="string"){importScripts(e.data.urlOrBlob)}else{var objectUrl=URL.createObjectURL(e.data.urlOrBlob);importScripts(objectUrl);URL.revokeObjectURL(objectUrl)}moduleLoaded()}else if(e.data.cmd==="objectTransfer"){Module["PThread"].receiveObjectTransfer(e.data)}else if(e.data.cmd==="run"){Module["__performance_now_clock_drift"]=performance.now()-e.data.time;Module["__emscripten_thread_init"](e.data.threadInfoStruct,0,0);var max=e.data.stackBase;var top=e.data.stackBase+e.data.stackSize;assert(e.data.threadInfoStruct);assert(top!=0);assert(max!=0);assert(top>max);Module["establishStackSpace"](top,max);Module["PThread"].receiveObjectTransfer(e.data);Module["PThread"].threadInit();if(!initializedJS){Module["___embind_register_native_and_builtin_types"]();initializedJS=true}try{var result=Module["invokeEntryPoint"](e.data.start_routine,e.data.arg);Module["checkStackCookie"]();if(Module["keepRuntimeAlive"]()){Module["PThread"].setExitStatus(result)}else{Module["PThread"].threadExit(result)}}catch(ex){if(ex==="Canceled!"){Module["PThread"].threadCancel()}else if(ex!="unwind"){if(typeof Module["_emscripten_futex_wake"]!=="function"){err("Thread Initialisation failed.");throw ex}if(ex instanceof Module["ExitStatus"]){if(Module["keepRuntimeAlive"]()){err("Pthread 0x"+Module["_pthread_self"]().toString(16)+" called exit(), staying alive due to noExitRuntime.")}else{err("Pthread 0x"+Module["_pthread_self"]().toString(16)+" called exit(), calling threadExit.");Module["PThread"].threadExit(ex.status)}}else{Module["PThread"].threadExit(-2);throw ex}}else{err("Pthread 0x"+Module["_pthread_self"]().toString(16)+" completed its pthread main entry point with an unwind, keeping the pthread worker alive for asynchronous operation.")}}}else if(e.data.cmd==="cancel"){if(Module["_pthread_self"]()){Module["PThread"].threadCancel()}}else if(e.data.target==="setimmediate"){}else if(e.data.cmd==="processThreadQueue"){if(Module["_pthread_self"]()){Module["_emscripten_current_thread_process_queued_calls"]()}}else{err("worker.js received unknown command "+e.data.cmd);err(e.data)}}catch(ex){err("worker.js onmessage() captured an uncaught exception: "+ex);if(ex&&ex.stack)err(ex.stack);throw ex}};