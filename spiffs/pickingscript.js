//Carousel
const carouselSlide = document.querySelector('.carousel-slide');
const carouselImages = document.querySelectorAll('.carousel-slide img');
const prevBtn = document.querySelector('#prevBtn');
const nextBtn = document.querySelector('#nextBtn');
let counter = 1;
const size = carouselImages[0].naturalWidth;


carouselSlide.style.transform = 'translateX(' + (-size * counter) + 'px)';

carouselSlide.addEventListener('transitionend',() => {
    if(carouselImages[counter].id === 'lastClone'){
        carouselSlide.style.transition = "none";
        counter = carouselImages.length - 2;
        carouselSlide.style.transform = 'translateX(' + (-size * counter) + 'px)';
    }
    if(carouselImages[counter].id === 'firstClone'){
        carouselSlide.style.transition = "none";
        counter = carouselImages.length - counter;
        carouselSlide.style.transform = 'translateX(' + (-size * counter) + 'px)';
    }
});

function carousel_next(){
    console.log("carousel_next works")
    if (counter >= carouselImages.length - 1) return;
    carouselSlide.style.transition = "transform 0.5s ease-in-out";
    counter++;
    carouselSlide.style.transform = 'translateX(' + (-size * counter) + 'px)';
}

function carousel_prev(){
    if (counter <= 0) return;
    carouselSlide.style.transition = "transform 0.5s ease-in-out";
    counter--;
    carouselSlide.style.transform = 'translateX(' + (-size * counter) + 'px)';
}

function getpickingarray(){
    //printf("get picked image button pressed!");
    const pickingarray = new Array(64).fill(0);
    if(counter == 1){ //face
        pickingarray[0] = "green";
        pickingarray[1] = "green";
        pickingarray[2] = "green";
        pickingarray[3] = "green";
        pickingarray[4] = "green";
        pickingarray[5] = "green";
        pickingarray[6] = "green";
        pickingarray[7] = "green";

        pickingarray[8] = "green";
        pickingarray[9] = "green";
        pickingarray[10] = "green";
        pickingarray[11] = "green";
        pickingarray[12] = "green";
        pickingarray[13] = "green";
        pickingarray[14] = "green";
        pickingarray[15] = "green";

        pickingarray[16] = "green";
        pickingarray[17] = "green";
        pickingarray[18] = "Red";
        pickingarray[19] = "green";
        pickingarray[20] = "green";
        pickingarray[21] = "red";
        pickingarray[22] = "green";
        pickingarray[23] = "green";

        pickingarray[24] = "green";
        pickingarray[25] = "green";
        pickingarray[26] = "green";
        pickingarray[27] = "green";
        pickingarray[28] = "red";
        pickingarray[29] = "green";
        pickingarray[30] = "green";
        pickingarray[31] = "green";

        

        pickingarray[32] = "green";
        pickingarray[33] = "red";
        pickingarray[34] = "green";
        pickingarray[35] = "green";
        pickingarray[36] = "green";
        pickingarray[37] = "green";
        pickingarray[38] = "red";
        pickingarray[39] = "green";

        pickingarray[40] = "green";
        pickingarray[41] = "red";
        pickingarray[42] = "red";
        pickingarray[43] = "red";
        pickingarray[44] = "red";
        pickingarray[45] = "red";
        pickingarray[46] = "red";
        pickingarray[47] = "green";

        pickingarray[48] = "green";
        pickingarray[49] = "green";
        pickingarray[50] = "green";
        pickingarray[51] = "green";
        pickingarray[52] = "green";    
        pickingarray[53] = "green";
        pickingarray[54] = "green";
        pickingarray[55] = "green";

        pickingarray[56] = "green";
        pickingarray[57] = "green";
        pickingarray[58] = "green";
        pickingarray[59] = "green";
        pickingarray[60] = "green";
        pickingarray[61] = "green";
        pickingarray[62] = "green";
        pickingarray[63] = "green";
    }
    else if(counter == 2){ //Pacman
        pickingarray[0] = "green";
        pickingarray[1] = "green";
        pickingarray[2] = "green";
        pickingarray[3] = "red";
        pickingarray[4] = "red";
        pickingarray[5] = "red";
        pickingarray[6] = "red";
        pickingarray[7] = "green";
        pickingarray[8] = "green";
        pickingarray[9] = "green";
        pickingarray[10] = "red";
        pickingarray[11] = "green";
        pickingarray[12] = "green";
        pickingarray[13] = "green";
        pickingarray[14] = "green";
        pickingarray[15] = "red";

        pickingarray[16] = "green";
        pickingarray[17] = "red";
        pickingarray[18] = "green";
        pickingarray[19] = "green";
        pickingarray[20] = "green";
        pickingarray[21] = "green";
        pickingarray[22] = "red";
        pickingarray[23] = "green";

        pickingarray[24] = "green";
        pickingarray[25] = "red";
        pickingarray[26] = "green";
        pickingarray[27] = "green";
        pickingarray[28] = "green";
        pickingarray[29] = "red";
        pickingarray[30] = "green";
        pickingarray[31] = "green";
        
        pickingarray[32] = "green";
        pickingarray[33] = "red";
        pickingarray[34] = "green";
        pickingarray[35] = "green";
        pickingarray[36] = "green";
        pickingarray[37] = "red";
        pickingarray[38] = "green";
        pickingarray[39] = "green";

        pickingarray[40] = "green";
        pickingarray[41] = "red";
        pickingarray[42] = "green";
        pickingarray[43] = "green";
        pickingarray[44] = "green";
        pickingarray[45] = "green";
        pickingarray[46] = "red";
        pickingarray[47] = "green";

        pickingarray[48] = "green";
        pickingarray[49] = "green";
        pickingarray[50] = "red";
        pickingarray[51] = "green";
        pickingarray[52] = "green";
        pickingarray[53] = "green";
        pickingarray[54] = "green";
        pickingarray[55] = "red";

        pickingarray[56] = "green";
        pickingarray[57] = "green";
        pickingarray[58] = "green";
        pickingarray[59] = "red";
        pickingarray[60] = "red";
        pickingarray[61] = "red";
        pickingarray[62] = "red";
        pickingarray[63] = "green";

       
    }
    else if(counter == 3){ //pacman monster
        pickingarray[0] = "red";
        pickingarray[1] = "red";
        pickingarray[2] = "green";
        pickingarray[3] = "green";
        pickingarray[4] = "green";
        pickingarray[5] = "green";
        pickingarray[6] = "red";
        pickingarray[7] = "red";

        pickingarray[8] = "red";
        pickingarray[9] = "green";
        pickingarray[10] = "green";
        pickingarray[11] = "green";
        pickingarray[12] = "green";
        pickingarray[13] = "green";
        pickingarray[14] = "green";
        pickingarray[15] = "red";

        pickingarray[16] = "green";
        pickingarray[17] = "red";
        pickingarray[18] = "red";
        pickingarray[19] = "green";
        pickingarray[20] = "red";
        pickingarray[21] = "red";
        pickingarray[22] = "green";
        pickingarray[23] = "green";

        pickingarray[24] = "green";
        pickingarray[25] = "green";
        pickingarray[26] = "red";
        pickingarray[27] = "green";
        pickingarray[28] = "green";
        pickingarray[29] = "red";
        pickingarray[30] = "green";
        pickingarray[31] = "green";

        pickingarray[32] = "green";
        pickingarray[33] = "green";
        pickingarray[34] = "green";
        pickingarray[35] = "green";
        pickingarray[36] = "green";
        pickingarray[37] = "green";
        pickingarray[38] = "green";
        pickingarray[39] = "green";

        pickingarray[40] = "green";
        pickingarray[41] = "green";
        pickingarray[42] = "green";
        pickingarray[43] = "green";
        pickingarray[44] = "green";
        pickingarray[45] = "green";
        pickingarray[46] = "green";
        pickingarray[47] = "green";

        pickingarray[48] = "green";
        pickingarray[49] = "green";
        pickingarray[50] = "green";
        pickingarray[51] = "green";
        pickingarray[52] = "green";
        pickingarray[53] = "green";
        pickingarray[54] = "green";
        pickingarray[55] = "green";

        pickingarray[56] = "green";
        pickingarray[57] = "green";
        pickingarray[58] = "red";
        pickingarray[59] = "green";
        pickingarray[60] = "green";
        pickingarray[61] = "red";
        pickingarray[62] = "green";
        pickingarray[63] = "green";
    }
    else{ //alien dudes
        pickingarray[0] = "green";
        pickingarray[1] = "green";
        pickingarray[2] = "green";
        pickingarray[3] = "red";
        pickingarray[4] = "red";
        pickingarray[5] = "red";
        pickingarray[6] = "red";
        pickingarray[7] = "red";

        pickingarray[8] = "green";
        pickingarray[9] = "red";
        pickingarray[10] = "green";
        pickingarray[11] = "red";
        pickingarray[12] = "red";
        pickingarray[13] = "red";
        pickingarray[14] = "red";
        pickingarray[15] = "red";

        pickingarray[16] = "green";
        pickingarray[17] = "green";
        pickingarray[18] = "green";
        pickingarray[19] = "red";
        pickingarray[20] = "red";
        pickingarray[21] = "green";
        pickingarray[22] = "green";
        pickingarray[23] = "green";

        pickingarray[24] = "red";
        pickingarray[25] = "green";
        pickingarray[26] = "red";
        pickingarray[27] = "red";
        pickingarray[28] = "red";
        pickingarray[29] = "green";
        pickingarray[30] = "red";
        pickingarray[31] = "green";

        pickingarray[32] = "green";
        pickingarray[33] = "green";
        pickingarray[34] = "green";
        pickingarray[35] = "red";
        pickingarray[36] = "red";
        pickingarray[37] = "green";
        pickingarray[38] = "green";
        pickingarray[39] = "green";

        pickingarray[40] = "red";
        pickingarray[41] = "green";
        pickingarray[42] = "red";
        pickingarray[43] = "red";
        pickingarray[44] = "red";
        pickingarray[45] = "red";
        pickingarray[46] = "green";
        pickingarray[47] = "red";

        pickingarray[48] = "red";
        pickingarray[49] = "green";
        pickingarray[50] = "red";
        pickingarray[51] = "red";
        pickingarray[52] = "red";
        pickingarray[53] = "red";
        pickingarray[54] = "green";
        pickingarray[55] = "red";

        pickingarray[56] = "green";
        pickingarray[57] = "red";
        pickingarray[58] = "green";
        pickingarray[59] = "red";
        pickingarray[60] = "red";
        pickingarray[61] = "green";
        pickingarray[62] = "red";
        pickingarray[63] = "green";
    }
    document.getElementById("pickingforminput").value = pickingarray;
    document.getElementById("pickingform").submit();
    sessionStorage.setItem("pickingarray",JSON.stringify(pickingarray));
}