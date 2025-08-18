// Mobile Menu
document.addEventListener('DOMContentLoaded', () =>{
    const hamburgerButton = document.querySelector('.hamburger-button');
    const mobileMenu = document.querySelector('.mobile-menu');

    hamburgerButton.addEventListener('click', () =>
        mobileMenu.classList.toggle('active')
    );

    // If user clicks off sidebar -> closes
    const heroArea = document.querySelector('.hero');
    console.log(heroArea);
    if (heroArea != null){
        heroArea.addEventListener('click', () =>
                mobileMenu.classList.remove('active')
            );
    }
    

    const projectArea = document.querySelector('.project');
    if (projectArea != null){
        projectArea.addEventListener('click', () =>
                mobileMenu.classList.remove('active')
            );
    }

    const aboutMeArea = document.querySelector('.about-me');
    console.log(aboutMeArea);
    if (aboutMeArea != null){
        aboutMeArea.addEventListener('click', () =>
                mobileMenu.classList.remove('active')
            );
    }


});

// Project filtering
document.addEventListener('DOMContentLoaded', () =>{
    const allType = document.querySelector('.type.all');
    const pythonType = document.querySelector('.type.python');
    const engType = document.querySelector('.type.engineering');
    const javaType = document.querySelector('.type.java');

    const pythonCard = document.querySelector('.card.python');
    const engCard = document.querySelector('.card.engineering');
    const javaCard = document.querySelector('.card.java');

    
    const typeContainer = document.querySelector('.types');

    typeContainer.addEventListener('click', (e) => {
        const typePick = e.target.closest('.type');

        if (!typePick) return; // does nothing if header is not the thing clicked

        // Toggle visibility of body
        typePick.classList.toggle('active');

        // Unselects other types
        const otherTypes = typeContainer.querySelectorAll('.type');
        otherTypes.forEach((otherType) => {
            if(otherType != typePick){
                otherType.classList.remove('active');
            }
        });

        const allCards = document.querySelectorAll('.card');
        allCards.forEach((thisCard) => {
            if (typePick.classList.contains('python')){
                if (thisCard.classList.contains('python')){
                    thisCard.classList.add('active');
                } else {    
                    thisCard.classList.remove('active');
                }
            } else if (typePick.classList.contains('all')){
                if (thisCard.classList.contains('all')){
                    thisCard.classList.add('active');
                } else {    
                    thisCard.classList.remove('active');
                }
            } else if (typePick.classList.contains('engineering')){
                if (thisCard.classList.contains('engineering')){
                    thisCard.classList.add('active');
                } else {    
                    thisCard.classList.remove('active');
                }
            } else {
                if (thisCard.classList.contains('java')){
                    thisCard.classList.add('active');
                } else {    
                    thisCard.classList.remove('active');
                }
            }

        });
        
    })

});