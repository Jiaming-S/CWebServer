
const wrapper = document.querySelector('.item-wrapper');
const rowNum = Object.keys(collegeInfo).length;

(function generatePlaceholders() {
  const placeholders = document.querySelectorAll('.item-wrapper > .placeholder');
  const placeholderNum = placeholders.length;
  if (placeholderNum !== rowNum) {
    if (placeholderNum > rowNum) {
      for (let i = rowNum; i < placeholderNum; i++) wrapper.removeChild(placeholders[i]);
    }
    if (placeholderNum < rowNum) {
      for (let i = placeholderNum; i < rowNum; i++) wrapper.insertBefore(placeholders[1].cloneNode(true), placeholders[1]);
    }
  }
})();

setTimeout(function populateCollegeInfo() {
  const updatedPlaceholders = document.querySelectorAll('.item-wrapper > .placeholder');

  for (let i = 0; i < rowNum; i++) {
    const curKey = Object.keys(collegeInfo)[i];
    const curInfo = {
      name: collegeInfo[curKey].name,
      id: collegeInfo[curKey].id,
      appType: collegeInfo[curKey].appType,
      dateSubmitted: collegeInfo[curKey].dateSubmitted,
      dateResponse: collegeInfo[curKey].dateResponse,
      collegePortalWebsite: collegeInfo[curKey].collegePortalWebsite,
    }
    const curRow = updatedPlaceholders[i];
    curRow.classList.remove('placeholder');
    curRow.id = `${curInfo.id}`;
  
    curRow.querySelector('.college-name').textContent = curInfo.name;
    curRow.querySelector('.college-submission-date').textContent = `Submitted ${curInfo.dateSubmitted}`;
    curRow.querySelector('.college-application-type').textContent = curInfo.appType;
    curRow.querySelector('.progress').id = `${curInfo.id}-progress`;
  }
}, populateInfoDelay);

setTimeout(function updateProgressBars() {
  const allProgressBars = document.querySelectorAll('.progress');
  for (let i = 0; i < rowNum; i++) {
    const curKey = Object.keys(collegeInfo)[i];
    const curInfo = {
      dateSubmitted: collegeInfo[curKey].dateSubmitted,
      dateResponse: collegeInfo[curKey].dateResponse,
    }
    const curBar = allProgressBars[i];

    const dateSubmitted = new Date(curInfo.dateSubmitted);
    const dateResponse = new Date(curInfo.dateResponse);
    const now = new Date();
    const total = dateResponse - dateSubmitted;
    const elapsed = now - dateSubmitted;

    let percent = Math.round(100 * elapsed / total);
    if (percent >= 100) {
      percent = 100;
      curBar.querySelector('.progress-bar').classList.add('complete');
    }
    
    if (percent < 0) percent = 0;

    curBar.querySelector('.progress-bar').style.width = `${percent}%`;
    curBar.querySelector('.progress-bar > p').textContent = `${percent}%`;

    curBar.parentElement.querySelector('.progress-labels > p:nth-child(1)').textContent = dateSubmitted.toLocaleDateString();
    curBar.parentElement.querySelector('.progress-labels > p:nth-child(2)').textContent = "~" + dateResponse.toLocaleDateString();
  };
}, updateProgressDelay);

setTimeout(function populateTwitterTimelines() {
  const allEmbedWindows = document.querySelectorAll('.item-big');

  for (let i = 0; i < rowNum; i++) {
    const curKey = Object.keys(collegeInfo)[i];
    const curInfo = {
      collegeTwitterFeed: collegeInfo[curKey].collegeTwitterFeed,
    }
    if (!(curInfo.collegeTwitterFeed)) continue;
    const curWindow = allEmbedWindows[i];
    const feed = document.createElement('a');
    feed.classList.add('twitter-timeline');
    feed.setAttribute('data-tweet-limit', '5');
    feed.setAttribute('data-height', '300');
    feed.setAttribute('data-theme', 'light');
    feed.href = curInfo.collegeTwitterFeed;
    feed.textContent = "Purdue Tweets";
    curWindow.appendChild(feed); 
  }
}, populateTwitterDelay);


