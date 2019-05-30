function myFunction()
{
    var element = document.getElementById("knapp");
    if(element.innerHTML == "Hej")
    {
        element.innerHTML = "Klicka här";
    }
    else{
        element.innerHTML = "Hej";
    }
}