//sample js code from website challenge
const modal=document.querySelector(".modal");
const previews=document.querySelectorAll(".gallery img");//preview img
const original=document.querySelector(".full-img");//full size img
const caption=document.querySelector(".caption");//img description

//click open original img
previews.forEach((preview)=>{
    preview.addEventListener("click",()=>{
        modal.classList.add("open");
        original.classList.add("open");

        //dupdate text and img
        const originalSrc=preview.getAttribute("data-original");
        original.src=originalSrc;
        const altText=preview.alt;
        caption.textContent=altText;
    });
});

//click close
modal.addEventListener("click",(e)=>{
    if(e.target.classList.contains("modal")){
        modal.classList.remove("open");
        original.classList.remove("open");
    }
});