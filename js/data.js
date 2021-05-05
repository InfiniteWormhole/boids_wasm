

// var ptr = Module._malloc(myTypedArray.length * myTypedArray.BYTES_PER_ELEMENT);
// Module.HEAP8.set(myTypedArray, ptr); //HEAPF64 is the storage location of double * in WebAssembly
// Module.setValue(ptr * 8, myTypedArray.length); //8 for sizeof(double)

document.getElementById('filebutton')
.addEventListener('change', function() {
  
var fr=new FileReader();
fr.onload=function(){
    // document.getElementById('output')
    //         .textContent=fr.result;
    // alert(fr.result);
    var myTypedArray = fr.result;
    var ptr = Module._malloc(myTypedArray.length * myTypedArray.BYTES_PER_ELEMENT);
    Module.HEAP8.set(myTypedArray, ptr); //HEAPF* is the storage location of char * in WebAssembly
    Module.importData(ptr * 1, myTypedArray.length); //8 for sizeof(char)
}
  
fr.readAsArrayBuffer(this.files[0]);
})