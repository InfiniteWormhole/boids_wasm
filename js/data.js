

// var ptr = Module._malloc(myTypedArray.length * myTypedArray.BYTES_PER_ELEMENT);
// Module.HEAP8.set(myTypedArray, ptr); //HEAPF64 is the storage location of double * in WebAssembly
// Module.setValue(ptr * 8, myTypedArray.length); //8 for sizeof(double)
var settings;
document.getElementById('filebutton')
.addEventListener('change', function() {
  
var fr=new FileReader();
fr.onload=function(){
    // document.getElementById('output')
    //         .textContent=fr.result;
    // alert(fr.result);
    // var myTypedArray = fr.result;
    // var ptr = Module._malloc(myTypedArray.length * myTypedArray.BYTES_PER_ELEMENT);
    // Module.HEAP8.set(myTypedArray, ptr); //HEAPF* is the storage location of char * in WebAssembly
    // Module.importData(ptr * 1, myTypedArray.byteLength); //8 for sizeof(char)
	settings = JSON.parse(fr.result)[0];
	opt.rboids = settings.boidCount;
	opt.rsize = settings.size;
	opt.rvision = settings.vis;
	opt.ralignment = settings.align
	opt.rcohesion = settings.cohere;
	opt.rseparation = settings.seperate;
	opt.particle = settings.particles;
	opt.rmaxForce = settings.maxForce.toFixed(2);
	opt.rdrag = settings.drag.toFixed(5);
	opt.rmaxSpeed = settings.maxSpeed;
	Module._setValues(opt.vision, opt.maxSpeed, opt.maxForce, opt.alignment, opt.cohesion, opt.separation, opt.boids, opt.size, opt.drag, opt.paused, opt.particle);
	console.log(settings);

	Module.importData(fr.result);
	// let settings = JSON.parse(fr.result);
	// opt.size = settings.size * 40;
	// console.log(settings);
}
  
fr.readAsText(this.files[0]);
document.getElementById("filebutton").value = "";
})

function download(filename, text) {
	var element = document.createElement('a');
	element.setAttribute('href', 'data:application/x-boids,' + encodeURIComponent(Module.exportData()));
	element.setAttribute('download', filename);
  
	element.style.display = 'none';
	document.body.appendChild(element);
  
	element.click();
  
	document.body.removeChild(element);
  }