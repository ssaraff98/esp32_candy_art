let penColor = 'white';

function setPenColor(chosen_color){
    penColor = chosen_color;
}
function setPixelColor(pixel){
    pixel.style.backgroundColor = penColor;
    
}

function clearCanvas(){
    let pixels = document.getElementById("pixelcanvas").getElementsByClassName("pixel");
    for(i = 0; i < pixels.length; i++){
        pixels[i].style.backgroundColor = 'white';
    }
}

function getPixelArray(){
    let pixelarray = [];
    let pixels = document.getElementById("pixelcanvas").getElementsByClassName("pixel");
    for(i = 0; i < pixels.length; i++){
        if(!pixels[i].style.backgroundColor){
            pixels[i].style.backgroundColor = 'white';
        }
        pixelarray.push(pixels[i].style.backgroundColor);
    }
    console.log(pixelarray);
    document.getElementById("forminput").value = pixelarray;
    document.getElementById("drawingform").submit();
    sessionStorage.setItem("drawingarray",JSON.stringify(pixelarray));
    
    return;
}