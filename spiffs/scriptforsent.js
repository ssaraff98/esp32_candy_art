function resetbutton(){
    window.location.href='/reset.html';
    sessionStorage.clear();
}

function showpixelart(){
    let sent_pixelarray;
    let sent_pixelarray_string;
    let pixels = document.getElementById("pixelcanvas").getElementsByClassName("pixel");
    console.log("getting from session storage...");
    if(sessionStorage.getItem("drawingarray")){
        sent_pixelarray_string = sessionStorage.getItem("drawingarray");
        sent_pixelarray = JSON.parse(sent_pixelarray_string);
        console.log("drawing array exists");
        console.log("This is drawing array:", sent_pixelarray);
        
    }
    else{
        sent_pixelarray_string = sessionStorage.getItem("pickingarray");
        sent_pixelarray = JSON.parse(sent_pixelarray_string);
    }
    
    for (i = 0; i < pixels.length; i++){
        console.log("each sent pixel",sent_pixelarray[i]);
        pixels[i].style.backgroundColor = sent_pixelarray[i];
        console.log("each colored pixel",pixels[i]);
    }

}