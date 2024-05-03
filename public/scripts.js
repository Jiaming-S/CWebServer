
const topBar = document.querySelector('.top-bar');
const mainScroller = document.querySelector('.content-main');
mainScroller.addEventListener('scroll', () => {
  curScroll = mainScroller.scrollTop;
  if (curScroll > 30) {
    topBar.classList.add('scrolled');
  }
  else {
    topBar.classList.remove('scrolled');
  }
});

const downArrow = document.querySelector('#down-arrow');
downArrow.addEventListener('click', () => {
  mainScroller.scrollBy({
    top: window.innerHeight,
    behavior: 'smooth'
  });
});

const upArrow = document.querySelector('#up-arrow');
upArrow.addEventListener('click', () => {
  mainScroller.scrollBy({
    top: -window.innerHeight,
    behavior: 'smooth'
  });
});







