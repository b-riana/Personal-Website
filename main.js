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